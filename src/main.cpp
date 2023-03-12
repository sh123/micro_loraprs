#include <Arduino.h>
#include <SPI.h>
#include <DebugLog.h>
#include <RadioLib.h>
#include <avr/sleep.h>

#include "ax25_payload.h"
#include "kiss.h"

// app config
#define CFG_ENABLE_DEBUG
#define CFG_LOOP_DELAY_MS     20
#define CFG_SERIAL_BAUD_RATE  19200
#define CFG_MAX_PACKET_SIZE   255
#define CFG_MY_CALLSIGN       "N0CALL-11"

// lora pinouts
#define CFG_LORA_PIN_NSS      5
#define CFG_LORA_PIN_RST      4
#define CFG_LORA_PIN_DIO0     3
#define CFG_LORA_PIN_DIO1     2

// lora modulation
#define CFG_LORA_FREQ         433.750E6
#define CFG_LORA_SYNC_WORD    0x12
#define CFG_LORA_BW           31.25e3
#define CFG_LORA_SF           7
#define CFG_LORA_CR           6
#define CFG_LORA_PWR          10

// kiss parameters
#define DEFAULT_P             255
#define DEFAULT_SLOT_TIME     0

// do not use debugging in modem mode
#ifdef CFG_USE_MODEM
#pragma message("Configured for modem")
#undef CFG_ENABLE_DEBUG
#else
#pragma message("Configured for digirepeater")
#endif

// peripherals
SX1278 radio_ = new Module(CFG_LORA_PIN_NSS, CFG_LORA_PIN_DIO0, CFG_LORA_PIN_RST, CFG_LORA_PIN_DIO1);
AX25::Callsign myCallsign_(CFG_MY_CALLSIGN);
byte pktBufTx_[CFG_MAX_PACKET_SIZE];
byte pktBufRx_[CFG_MAX_PACKET_SIZE];
int pktBufTxIndex_ = 0;
volatile bool isLoraRxDataAvailable_ = false;
volatile bool isRxIsrEnabled_ = true;

Kiss::State kissState_ = Kiss::State::GetStart;
Kiss::Cmd kissCmd_;
long csmaSlotTime_ = DEFAULT_SLOT_TIME;
long csmaSlotTimePrev_ = 0;
byte csmaP_ = DEFAULT_P;

// incoming data isr callback
void isrRxDataAvailable(void) {
  if (isRxIsrEnabled_)
    isLoraRxDataAvailable_ = true;
}

// enter deep sleep until packet is received
void enterDeepSleep() {
  LOG_INFO(F("Enter sleep"));
  sleep_enable();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  sleep_cpu();
  digitalWrite(LED_BUILTIN, HIGH);
  LOG_INFO(F("Exit sleep"));
}

// arduino setup
void setup() {
  // setup serial
#if defined(CFG_ENABLE_DEBUG) || defined(CFG_USE_MODEM)
  Serial.begin(CFG_SERIAL_BAUD_RATE);
  while (!Serial);
#endif

  // setup logging
#ifdef CFG_ENABLE_DEBUG
  LOG_SET_LEVEL(DebugLogLevel::LVL_INFO);
  #pragma message("Logging is enabled")
#else
  LOG_SET_LEVEL(DebugLogLevel::LVL_NONE);
  #pragma message("Logging is disabled")
#endif  
  LOG_SET_OPTION(false, false, true);  // disable file, line, enable func

  // setup lora
  int state = radio_.begin((float)CFG_LORA_FREQ / 1e6, (float)CFG_LORA_BW / 1e3, 
    CFG_LORA_SF, CFG_LORA_CR, CFG_LORA_SYNC_WORD, CFG_LORA_PWR);
  if (state != RADIOLIB_ERR_NONE) {
    LOG_ERROR(F("Radio start failure"), state);
    while(true);
  }
  radio_.explicitHeader();
  radio_.setCRC(1);
  radio_.setDio0Action(isrRxDataAvailable);

  // start lora receive
  state = radio_.startReceive();
  if (state != RADIOLIB_ERR_NONE) {
    LOG_ERROR(F("Receive start error:"), state);
    while(true);
  }

  LOG_INFO(F("Started"));

#ifndef CFG_USE_MODEM
  enterDeepSleep();
#endif
}

// read packet to the buffer
size_t receiveLoraPacket() {

  // get packet size
  size_t packetLength = radio_.getPacketLength();
  if (packetLength == 0) {
    isLoraRxDataAvailable_ = false;
    return 0;
  }
  if (packetLength > CFG_MAX_PACKET_SIZE) {
    LOG_ERROR(F("Packet size is too large:"), packetLength);
    packetLength = 0;
  } else {
    // read packet
    int state = radio_.readData(pktBufRx_, packetLength);
    if (state != RADIOLIB_ERR_NONE) {
      LOG_ERROR(F("Read pkt error:"), state);
      packetLength = 0;
    }
  }

  isLoraRxDataAvailable_ = false;

  // start lora receive
  int state = radio_.startReceive();
  if (state != RADIOLIB_ERR_NONE) {
    LOG_ERROR(F("Receive start error:"), state);
  }

  return packetLength;
}

// write packet from the buffer
void sendLoraPacket(size_t packetLength) {
  isRxIsrEnabled_ = false;

  // send out 
  int state = radio_.transmit(pktBufTx_, packetLength);
  if (state != RADIOLIB_ERR_NONE) {
    LOG_ERROR(F("Failed to transmit"), state);
  }
  radio_.finishTransmit();

  isRxIsrEnabled_ = true;

  // start lora receive
  state = radio_.startReceive();
  if (state != RADIOLIB_ERR_NONE) {
    LOG_ERROR(F("Receive start error:"), state);
  }
}

// read lora packet and transfer to modem
void processLoraPacketAsModem() 
{
  size_t packetLength = receiveLoraPacket();
  if (packetLength == 0) {
    return;
  }

  Serial.write(Kiss::Marker::Fend);
  Serial.write(Kiss::Cmd::Data);

  for (size_t i = 0; i < packetLength; i++) {
    byte rxByte = pktBufRx_[i];

    if (rxByte == Kiss::Marker::Fend) {
      Serial.write(Kiss::Marker::Fesc);
      Serial.write(Kiss::Marker::Tfend);
    }
    else if (rxByte == Kiss::Marker::Fesc) {
      Serial.write(Kiss::Marker::Fesc);
      Serial.write(Kiss::Marker::Tfesc);
    }
    else {
      Serial.write(rxByte);
    }
  }
  Serial.write(Kiss::Marker::Fend);
}

// read serial packet and transfer to lora radio
void processSerialPacketAsModem() 
{
  while (Serial.available()) {
    
    // packet overflow
    if (pktBufTxIndex_ > CFG_MAX_PACKET_SIZE) {
      kissState_ = Kiss::State::GetStart;
      pktBufTxIndex_ = 0;
    }

    int rxResult = Serial.read();
    if (rxResult == -1) {
      break;
    }
    
    byte rxByte = (byte)rxResult;

    switch (kissState_) {
      // get kiss start
      case Kiss::State::GetStart:
        if (rxByte == Kiss::Marker::Fend) {
          kissState_ = Kiss::State::GetCmd;
        }
        break;
      // get kiss end
      case Kiss::State::GetEnd:
        if (rxByte == Kiss::Marker::Fend) {
          kissState_ = Kiss::State::GetStart;
        }
        break;
      // get command code
      case Kiss::State::GetCmd:
          switch (rxByte) {
          case Kiss::Cmd::Data:
            pktBufTxIndex_ = 0;
            kissState_ = Kiss::State::GetData;
            break;
          case Kiss::Cmd::P:
            kissState_ = Kiss::State::GetP;
            break;
          case Kiss::Cmd::SlotTime:
            kissState_ = Kiss::State::GetSlotTime;
            break;
          case Kiss::Marker::Fend:
            break;
          default:
            kissState_ = Kiss::State::GetEnd;
            break;
          }
        break;
      // get persistance
      case Kiss::State::GetP:
        csmaP_ = rxByte;
        kissState_ = Kiss::State::GetEnd;
        break;
      // get slot time
      case Kiss::State::GetSlotTime:
        csmaSlotTime_ = (long)rxByte * 10;
        kissState_ = Kiss::State::GetEnd;
        break;
      // get raw data
      case Kiss::State::GetData:
        switch (rxByte) {
          case Kiss::Marker::Fesc:
            kissState_ = Kiss::State::Escape;
            break;
          case Kiss::Marker::Fend:
            if (kissCmd_ == Kiss::Cmd::Data) {
              sendLoraPacket(pktBufTxIndex_);
              pktBufTxIndex_ = 0;
            }
            kissState_ = Kiss::State::GetStart;
            break;
          default:
            pktBufTx_[pktBufTxIndex_++] = rxByte;
            break;
        }
        break;
      // escape
      case Kiss::State::Escape:
        if (rxByte == Kiss::Marker::Tfend) {
          pktBufTx_[pktBufTxIndex_++] = Kiss::Marker::Fend;
          kissState_ = Kiss::State::GetData;
        }
        else if (rxByte == Kiss::Marker::Tfesc) {
          pktBufTx_[pktBufTxIndex_++] = Kiss::Marker::Fesc;
          kissState_ = Kiss::State::GetData;
        }
        else if (rxByte != Kiss::Marker::Fend) {
          kissState_ = Kiss::State::GetEnd;
        }
        break;
      default:
        kissState_ = Kiss::State::GetStart;
        break;

    } // kissState_

    yield();

  } // Serial.available()
}

// process incoming packet
void processLoraPacketAsRpt() {

  // read packet to the buffer
  size_t packetLength = receiveLoraPacket();
  if (packetLength == 0) return;

  // parse packet
  AX25::Payload payload(pktBufRx_, packetLength);
  if (!payload.IsValid()) {
    LOG_WARN(F("Failed to parse rx packet, not ax25"));
    return;
  }
  payload.Dump();

  // digireat
  if (!payload.Digirepeat(myCallsign_)) {
    LOG_INFO(F("Digirepeating skipped"));
    return;
  }

  // serialize to ax25 packet
  int txPacketLength = payload.ToBinary(pktBufRx_, CFG_MAX_PACKET_SIZE);
  if (txPacketLength <= 0) {
    LOG_WARN(F("Failed to convert to AX25 to binary"));
    return;
  }

  // send lora packet
  sendLoraPacket(txPacketLength);
}

// arduino loop
void loop() {
#ifdef CFG_USE_MODEM
  if (isLoraRxDataAvailable_) {
    processLoraPacketAsModem();
  } else {
    long currentTime = millis();
    if (currentTime > csmaSlotTimePrev_ + csmaSlotTime_ && random(0, 255) < csmaP_) {
      if (Serial.available()) {
        processSerialPacketAsModem();
      }
      csmaSlotTimePrev_ = currentTime;
    }
  }
#else
  if (isLoraRxDataAvailable_) {
    processLoraPacketAsRpt();
    enterDeepSleep();
  }
#endif
  delay(CFG_LOOP_DELAY_MS);
}