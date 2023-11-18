#ifndef KISS_H
#define KISS_H

namespace Kiss {

enum Marker {
  Fend = 0xc0,
  Fesc = 0xdb,
  Tfend = 0xdc,
  Tfesc = 0xdd
};

enum State {
  GetStart = 0,
  GetEnd,
  GetCmd,
  GetData,
  GetP,
  GetSlotTime,
  Escape
};

enum Cmd {
  Data = 0x00,
  P = 0x02,
  SlotTime = 0x03,
  SetHardware = 0x06,
  NoCmd = 0x80
};

enum DataType {
  Raw = 0,
  Control,
  None = 0x80
};

#define CFG_KISS_SET_HARDWARE_SIZE  34

struct SetHardware {
  uint32_t freqRx;
  uint32_t freqTx;
  uint8_t modType;
  int16_t pwr;
  uint32_t bw;
  uint16_t sf;
  uint16_t cr;
  uint16_t sync;
  uint8_t crc;
  uint32_t fskBitRate;
  uint32_t fskFreqDev;
  uint32_t fskRxBw;
} __attribute__((packed));

} // Kiss

#endif // KISS_H