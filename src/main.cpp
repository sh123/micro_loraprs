#include <Arduino.h>
#include <SPI.h>
#include <DebugLog.h>
#include <RadioLib.h>
#include <avr/sleep.h>

#include "ax25_payload.h"
#include "kiss.h"
#include "config.h"
#include "endian.h"

// app config
#define CFG_LOOP_DELAY_MS     20
#define CFG_SERIAL_BAUD_RATE  19200
#define CFG_MAX_PACKET_SIZE   255

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

// digirepater callsign
AX25::Callsign myCallsign_(CFG_MY_CALLSIGN);

// packet buffers
byte pktBufTx_[CFG_MAX_PACKET_SIZE];
byte pktBufRx_[CFG_MAX_PACKET_SIZE];
int pktBufTxIndex_ = 0;

// receive isr parameters
volatile bool isRxIsrEnabled_ = true;
volatile bool isLoraRxDataAvailable_ = false;

// kiss parameters
Kiss::State kissState_ = Kiss::State::GetStart;
Kiss::DataType kissDataType_ = Kiss::DataType::None;
Kiss::Cmd kissCmd_ = Kiss::Cmd::NoCmd;
long csmaSlotTime_ = DEFAULT_SLOT_TIME;
long csmaSlotTimePrev_ = 0;
byte csmaP_ = DEFAULT_P;

// incoming data isr callback
void isrRxDataAvailable(void) 
{
  if (isRxIsrEnabled_)
    isLoraRxDataAvailable_ = true;
}

// enter deep sleep until packet is received (for low power digirepeating)
void enterDeepSleep() 
{
  LOG_INFO(F("Enter sleep"));
  sleep_enable();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  sleep_cpu();
  digitalWrite(LED_BUILTIN, HIGH);
  LOG_INFO(F("Exit sleep"));
}

// configura radio with given parameters
void setupRadio(long loraFreq, long bw, int sf, int cr, int pwr, int sync, int crcBytes, bool isExplicit) 
{
  // setup lora
  int state = radio_.begin((float)loraFreq / 1e6, (float)bw / 1e3, sf, cr, sync, pwr);
  if (state != RADIOLIB_ERR_NONE) {
    LOG_ERROR(F("Radio start failure"), state);
    while(true);
  }
  if (isExplicit && sf > 6)
    radio_.explicitHeader();
  else
    radio_.implicitHeader(0xff);
  radio_.setCRC(crcBytes);
  radio_.setPreambleLength(CFG_LORA_PREAMBLE);
  radio_.setDio0Action(isrRxDataAvailable);

  // start lora receive
  state = radio_.startReceive();
  if (state != RADIOLIB_ERR_NONE) {
    LOG_ERROR(F("Receive start error:"), state);
    while(true);
  }
}

// configura radio with given parameters
void setupRadioFsk(long loraFreq, float bitRate, float freqDev, float rxBw, int pwr)
{
  // setup lora
  int state = radio_.beginFSK((float)loraFreq / 1e6, bitRate, freqDev, rxBw, pwr);
  if (state != RADIOLIB_ERR_NONE) {
    LOG_ERROR(F("Radio start failure"), state);
    while(true);
  }
  radio_.setDio0Action(isrRxDataAvailable);

  // start lora receive
  state = radio_.startReceive();
  if (state != RADIOLIB_ERR_NONE) {
    LOG_ERROR(F("Receive start error:"), state);
    while(true);
  }
}

// arduino default setup
void setup() 
{
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

  // setup radio with default parameters
#if CFG_MOD == CFG_MOD_LORA
  setupRadio(CFG_LORA_FREQ, CFG_LORA_BW, CFG_LORA_SF, 
    CFG_LORA_CR, CFG_LORA_PWR, CFG_LORA_SYNC_WORD, 
    CFG_LORA_CRC, CFG_LORA_EXPLICIT);
#elif CFG_MOD == CFG_MOD_FSK
  setupRadioFsk(CFG_LORA_FREQ, CFG_FSK_BIT_RATE, 
    CFG_FSK_FREQ_DEV, CFG_FSK_RX_BW, CFG_LORA_PWR);
#else
#error "Unknown modulation type"
#endif

  LOG_INFO(F("Started"));

  // save power by going sleep in digirepeater mode
#ifndef CFG_USE_MODEM
  enterDeepSleep();
#endif
}

// read packet to the buffer
size_t receiveLoraPacket() 
{
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
void sendLoraPacket(size_t packetLength) 
{
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

// set hardware from current buffer
void kissSetHardware(int packetSize) 
{
  if (packetSize == CFG_KISS_SET_HARDWARE_SIZE) {
    const struct Kiss::SetHardware * setHardware = reinterpret_cast<const struct Kiss::SetHardware*>(pktBufTx_);
    if (setHardware->modType == CFG_MOD_LORA) {
#if CFG_MOD == CFG_MOD_LORA
      setupRadio(
        be32toh(setHardware->freqRx),
        be32toh(setHardware->bw), 
        be16toh(setHardware->sf), 
        be16toh(setHardware->cr), 
        (int16_t)be16toh(setHardware->pwr),
        be16toh(setHardware->sync), 
        setHardware->crc ? CFG_LORA_CRC : 0,
        CFG_LORA_EXPLICIT);
#endif
    } else if (setHardware->modType == CFG_MOD_FSK) {
#if CFG_MOD == CFG_MOD_FSK
      setupRadioFsk(
        be32toh(setHardware->freqRx),
        (float)be32toh(setHardware->fskBitRate) / 1e3,
        (float)be32toh(setHardware->fskFreqDev) / 1e3,
        (float)be32toh(setHardware->fskRxBw) / 1e3,
        (int16_t)be16toh(setHardware->pwr));
#endif
    }
  }
}

// process one byte from serial, transmit to radio
void kissProcess(byte rxByte) 
{
  // packet overflow
  if (pktBufTxIndex_ > CFG_MAX_PACKET_SIZE) {
    kissState_ = Kiss::State::GetStart;
    pktBufTxIndex_ = 0;
  }
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
        // data, next packet is the payload
        case Kiss::Cmd::Data:
          pktBufTxIndex_ = 0;
          kissState_ = Kiss::State::GetData;
          kissDataType_ = Kiss::DataType::Raw;
          break;
        // persistence, next byte is a P
        case Kiss::Cmd::P:
          kissState_ = Kiss::State::GetP;
          break;
        // slot time, next byte is a slot time
        case Kiss::Cmd::SlotTime:
          kissState_ = Kiss::State::GetSlotTime;
          break;
        // next byte is the payload of control/set hardware type
        case Kiss::Cmd::SetHardware:
          pktBufTxIndex_ = 0;
          kissState_ = Kiss::State::GetData;
          kissDataType_ = Kiss::DataType::Control;
          break;
        // end byte ignore, no command specified
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
          // packet is completed send out
          if (kissDataType_ == Kiss::DataType::Raw) {
            sendLoraPacket(pktBufTxIndex_);
            pktBufTxIndex_ = 0;
          // set hardware command is completed, set it
          } else if (kissDataType_ == Kiss::DataType::Control) {
            kissSetHardware(pktBufTxIndex_);
            pktBufTxIndex_ = 0;
          }
          kissState_ = Kiss::State::GetStart;
          break;
        // new byte is read
        default:
          if (kissDataType_ == Kiss::DataType::Raw ||
              kissDataType_ == Kiss::DataType::Control)
            pktBufTx_[pktBufTxIndex_++] = rxByte;
          break;
      }
      break;
    // escape current byte
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
}

// read serial packet and transfer to lora radio
void processSerialPacketAsModem() 
{
  while (Serial.available()) {
    int rxResult = Serial.read();
    // no more data
    if (rxResult == -1) {
      break;
    }
    kissProcess((byte)rxResult);
    yield();
  } 
}

// process incoming packet
void processLoraPacketAsRpt() 
{
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
void loop() 
{
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