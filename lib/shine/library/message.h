// Copyright James McGill, 2011
// Author: James McGill (jmcgill@plexer.net)

#ifndef SHINE_MESSAGE_H_
#define SHINE_MESSAGE_H_

#include "shine_types.h"

class Message {
public:
  virtual int8 num_fields() = 0;
  virtual int8 num_scalar_fields() = 0;
  virtual bool repeated(int8 index) = 0;
  virtual FieldTypes type(int8 index) = 0;
  virtual void set_value(int8 index, void* value) = 0;
  virtual void* get_value(int8 index) = 0;
  virtual void set_size(int8 index, int32 size) = 0;
};

#endif
