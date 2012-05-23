// Copyright James McGill, 2011
// Author: James McGill (jmcgill@plexer.net)

#include "flash.shn.h"

#include "shine_types.h"

// Static message variables.

FieldTypes Input::types[2] = {
  
    INT8,
  
    INT16,
  
};

char Input::repeated_[1] = {
  0x2
};

FieldTypes Output::types[2] = {
  
    INT16,
  
    INT8,
  
};

char Output::repeated_[1] = {
  0x2
};


void HandleRPCCall_Generated(ShineImpl* service, int8 rpc_index) {
  switch (rpc_index) {
    
    case 1: {
        Input input(service);
        service->input_ = &input;
        service->ReadBundledFields();

        
        Output output(service);
        Test(
            *((Input *)service->input_),
            &output);
        

        service->SendCommand('x');
        service->SendScalarFields(&output);
        break;
    }
    
    default:
      // Do nothing.
      break;
  }
}