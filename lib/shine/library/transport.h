// Copyright James McGill, 2011
// Author: James McGill (jmcgill@plexer.net)

#ifndef SHINE_TRANSPORT_H_
#define SHINE_TRANSPORT_H_

#include "shine_types.h"

class Transport {
 public:
  virtual int8 ReadByte() = 0;
  virtual int16 ReadUInt16() = 0;
  virtual int32 ReadUInt32() = 0;
  virtual void SendByte(int8 data) = 0;
  virtual void SendUInt16(int16 data) = 0;
  virtual void SendUInt32(int32 data) = 0;
  virtual bool HasData() = 0;
};

#endif
