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
  NoCmd = 0x80
};

}