// Copyright James McGill, 2011
// Author: James McGill (jmcgill@plexer.net)

#ifndef SHINE_ShineImpl_H
#define SHINE_ShineImpl_H

#include "transport.h"
#include "serial_transport.h"
#include "message.h"
#include "shine_types.h"

// TODO(jmcgill): Arduino -> PC calls.

// TOOD(jmcgill): How to build LARGE proto's to be sent to the PC in an
// Arduino -> PC call? Need an ID per buffer instance so I know which
// buffer to associate it with on the other side (Future Problems).


class ShineImpl {
 public:
   ShineImpl() {}

   ShineImpl(Transport* transport) : transport_(transport) {};
   
   void begin(Transport* transport);
   
   void begin(int16 baud);

   void SendCommand(int8 command);

   void loop();

   bool HandleCommand(int8 command);

   void HandleReceivedField();

   void* ReadField(FieldTypes type);

   void* SendField(FieldTypes type, int8 field_index, void* value);
   void* SendFieldValue(FieldTypes type, int8 field_index, void* value);
   void* SendScalarValue(FieldTypes type, int8 field_index, void* value);
   void* SendRepeatedField(
     FieldTypes type,
     int8 field_index,
     int32 offset,
     void* value);

   void ReadBundledFields();

   void RequestField(int8 index);

   void* RequestRepeatedField(int8 index, int32 offset);

   // NOTE(jmcgill): This must be generated per proto buffer. Consider breaking
   // out of the class.
   void HandleRPCCall();

   void HandleRPCStart();

   // Send scalar values to the PC. Called when returning from an RPC. 
   void SendScalarFields(Message* message);

   // Send no scalar values to the PC. Called when returning from an RPC with
   // no output.
   void SendNoScalarFields();

   void write(char* data);
   
   void Write_(char* data);

   Transport* transport_;
   int32 current_field_;
   Message* input_; 
   Message* output_;
};

void SetBitpackedBit(char* data, int8 index);
bool ReadBitpackedBit(char* data, int8 index);
void HandleRPCCall_Generated(ShineImpl* ShineImpl, int8 rpc_index);

class SerialTransport;

extern ShineImpl Shine;
extern SerialTransport SerialTransportInstance;

#endif
