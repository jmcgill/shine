// Copyright James McGill, 2011
// Author: James McGill (jmcgill@plexer.net)

#ifndef BITPACK_H_
#define BITPACK_H_

#include "shine_types.h"

void SetBitpackedBit(char* data, int8 index);
bool ReadBitpackedBit(char* data, int8 index);

#endif
