// Copyright James McGill, 2011
// Author: James McGill (jmcgill@plexer.net)

#include "bitpack.h"

void SetBitpackedBit(char* data, int8 index) {
  int8 block = (index / 8);
  int8 sub_index = (index - (block * 8));
  data[block] |= (1 << index);
}

bool ReadBitpackedBit(char* data, int8 index) {
  int8 block = (index / 8);
  int8 sub_index = (index - (block * 8));
  return (data[block] & (1 << index));
}


