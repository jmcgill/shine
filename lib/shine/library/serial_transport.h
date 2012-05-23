// Copyright James McGill, 2011
// Author: James McGill (jmcgill@plexer.net)

#ifndef SHINE_SERIAL_TRANSPORT_H_
#define SHINE_SERIAL_TRANSPORT_H_

#include "shine_types.h"
#include "transport.h"

#include "WProgram.h" // That's some windows style naming right there.

class SerialTransport : public Transport {
 public:
  int8 ReadByte() {
    //Serial.write('>');
    //Serial.write(data);
    //if (data == '!') Serial.write("BNG");
    while (Serial.available() == 0) {
      // Do nothing.
    }
    int8 data = Serial.read();
    return data;
  };

  int16 ReadUInt16() {
    int8 h = ReadByte();
    int8 l = ReadByte();
    return (h << 8) + l;
  };

  int32 ReadUInt32() {
    int16 h = ReadUInt16();
    int16 l = ReadUInt16();
    return (h << 16) + l;
  }

  void SendByte(int8 data) {
    Serial.write(data);
  }

  void SendUInt16(int16 data) {
    SendByte((data >> 8) & 0xFF);
    SendByte(data & 0xFF);
  }

  void SendUInt32(int32 data) {
    SendByte((data >> 24) & 0xFF);
    SendByte((data >> 16) & 0xFF);
    SendByte((data >> 8) & 0xFF);
    SendByte(data & 0xFF);
  }

  bool HasData() {
    return Serial.available();
  }
};

#endif
