// Copyright James McGill, 2011
// Author: James McGill (jmcgill@plexer.net)

#ifndef SHINE_flash_shn_H_
#define SHINE_flash.shn_H_

#include "message.h"
#include "shine_types.h"
#include "bitpack.h"
#include "shine.h"


class Input : public Message {
public:
  // Types for each field.
  static FieldTypes types[2];

  // Bitpacked representation of which fields are repeated.
  static char repeated_[1];

  // Bitpacked representation of which fields have been downloaded.
  char has_fields_[1];

  // A pointer to the Shine service which initialized this message.
  ShineImpl* service_;

  // Fields
  
  
    int8 count_;
  
  
  
    int32 delays_length_;
  
  
 
  Input(ShineImpl* service) {
    // has_fields_[0] = 0;
    service_ = service;

    // Initialize length fields.
    
    
    
    
    delays_length_ = 0;
    
    
  }
 
  inline int8 num_fields() {
    return 2;
  }

  inline int8 num_scalar_fields() {
    return 1;
  }
  
  inline bool repeated(int8 index) {
    return ReadBitpackedBit(Input::repeated_, index);
  }

  inline FieldTypes type(int8 index) {
    return Input::types[index];
  }

  
  
  void set_count(int8 value) {
    SetBitpackedBit(has_fields_, 0);
    count_ = value;
  }

  int8 count() {
    if (!ReadBitpackedBit(has_fields_, 0)) {
      service_->RequestField(0); 
    }
    return count_;
  }
  
  
  
  int16 delays(int32 offset) {
    // Array fields are always accessed remotely.
    return *((int16 *)service_->RequestRepeatedField(1, offset));
  }

  void set_delays(int32 offset, int16 value) {
    // Array fields are always accessed remotely.
    service_->SendRepeatedField(
        INT16,
        1,
        offset,
        (void *)&value);
  }

  void add_delays(int16 value) {
    service_->SendRepeatedField(
        INT16,
        1,
        delays_length_,
        (void *)&value);
    delays_length_ += 1;
  }

  
  

  void set_value(int8 index, void* value) {
    switch (index) {
      
      
      case 0:
        set_count(*((int8*)value));
        break;
      
      
      
      
      default:
        // Do nothing.
        break;
    }
  }

  // TODO(jmcgill): How to pass types around correcty? I might actually want
  // an Int8 and an Int16 class etc that can all handle how to store and
  // send themselves.
  void * get_value(int8 index) {
    switch (index) {
      
      
      case 0:
        return (void *)&count_;
        break;
      
      
      
      
      default:
        // Do nothing.
        break;
    }
  }
};

class Output : public Message {
public:
  // Types for each field.
  static FieldTypes types[2];

  // Bitpacked representation of which fields are repeated.
  static char repeated_[1];

  // Bitpacked representation of which fields have been downloaded.
  char has_fields_[1];

  // A pointer to the Shine service which initialized this message.
  ShineImpl* service_;

  // Fields
  
  
    int16 success_;
  
  
  
    int32 other_length_;
  
  
 
  Output(ShineImpl* service) {
    // has_fields_[0] = 0;
    service_ = service;

    // Initialize length fields.
    
    
    
    
    other_length_ = 0;
    
    
  }
 
  inline int8 num_fields() {
    return 2;
  }

  inline int8 num_scalar_fields() {
    return 1;
  }
  
  inline bool repeated(int8 index) {
    return ReadBitpackedBit(Input::repeated_, index);
  }

  inline FieldTypes type(int8 index) {
    return Output::types[index];
  }

  
  
  void set_success(int16 value) {
    SetBitpackedBit(has_fields_, 0);
    success_ = value;
  }

  int16 success() {
    if (!ReadBitpackedBit(has_fields_, 0)) {
      service_->RequestField(0); 
    }
    return success_;
  }
  
  
  
  int8 other(int32 offset) {
    // Array fields are always accessed remotely.
    return *((int8 *)service_->RequestRepeatedField(1, offset));
  }

  void set_other(int32 offset, int8 value) {
    // Array fields are always accessed remotely.
    service_->SendRepeatedField(
        INT8,
        1,
        offset,
        (void *)&value);
  }

  void add_other(int8 value) {
    service_->SendRepeatedField(
        INT8,
        1,
        other_length_,
        (void *)&value);
    other_length_ += 1;
  }

  
  

  void set_value(int8 index, void* value) {
    switch (index) {
      
      
      case 0:
        set_success(*((int16*)value));
        break;
      
      
      
      
      default:
        // Do nothing.
        break;
    }
  }

  // TODO(jmcgill): How to pass types around correcty? I might actually want
  // an Int8 and an Int16 class etc that can all handle how to store and
  // send themselves.
  void * get_value(int8 index) {
    switch (index) {
      
      
      case 0:
        return (void *)&success_;
        break;
      
      
      
      
      default:
        // Do nothing.
        break;
    }
  }
};


void HandleRPCCall_Generated(ShineImpl* service);  

// Declare RPCs.


void Test(Input& input, Output* output);



#endif