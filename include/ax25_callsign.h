#ifndef AX25_CALLSIGN_H
#define AX25_CALLSIGN_H

#include <Arduino.h>

namespace AX25 {
  
class Callsign
{
public:
  Callsign();
  Callsign(const Callsign &callsign);
  Callsign& operator=(const Callsign &callsign);
  
  Callsign(const byte *rxPayload, int payloadLength);
  Callsign(const String &inputText);

  inline bool IsValid() const { return isValid_; };
  inline bool IsTrace() const { return call_.startsWith("TRACE"); }
  inline bool IsWide() const { return call_.startsWith("WIDE"); }
  inline bool IsPath() const { return IsWide(); }
  
  String ToString(bool isDigirepeater=false) const;
  bool ToBinary(byte *txPayload, size_t bufferLength) const;

  bool Digirepeat();
  
private:
  bool fromString(const String &callsign);
  bool fromBinary(const byte *rxPtr, size_t payloadLength);

private:
  const size_t CallsignSize = 7;
  
private:
  bool isValid_;
  String call_;
  byte ssid_;
};

} // AX25

#endif // AX25_CALLSIGN_H
