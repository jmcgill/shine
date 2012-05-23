// Copyright James McGill, 2011
// Author: James McGill (jmcgill@plexer.net)

#include "shine.h"
#include "shine_types.h"
#include "serial_transport.h"

#include "WProgram.h"

ShineImpl Shine;
SerialTransport SerialTransportInstance;

void ShineImpl::begin(Transport* transport) {
  transport_ = transport;
}

// public
void ShineImpl::begin(int16 baud) {
  Serial.begin(baud);
  transport_ = &SerialTransportInstance;
}

void ShineImpl::SendCommand(int8 command) {
  transport_->SendByte('!');
  transport_->SendByte(command);
}

// public
void ShineImpl::loop() {
  if (transport_->HasData()) { 
    int8 data = transport_->ReadByte();
    if (data == '!') {
      HandleCommand(transport_->ReadByte());
    }
  }
}

bool ShineImpl::HandleCommand(int8 command) {
  switch (command) {
    case 's':
      HandleReceivedField();
      return true;
    case 'c':
      HandleRPCCall();
      return true;
    case 'x':
      // RPC Complete.
      return false;
    default:
      break;
  }
}

void ShineImpl::HandleReceivedField() {
  int8 field_index = transport_->ReadByte();

  if (input_->repeated(field_index)) {
    // Only the size of repeated fields is sent in advance.
    int32 size = transport_->ReadUInt32();
    input_->set_size(field_index, size);
  } else {
    // The actual value is sent for scalar fields.
    FieldTypes field_type = input_->type(field_index);
    void* value = ReadField(field_type);
    input_->set_value(field_index, value);
  }
}

void* ShineImpl::ReadField(FieldTypes type) {
  // TODO(jmcgill): Deal with strings later.
  switch (type) {
    case INT8:
      current_field_ = transport_->ReadByte(); 
      break;
    case INT16:
      current_field_ = transport_->ReadUInt16(); 
      break;
    default:
      break;
  }
  return (void *)&current_field_;
}

void* ShineImpl::SendField(FieldTypes type, int8 field_index, void* value) {
  SendCommand('s');
  SendFieldValue(type, field_index, value);
}

void* ShineImpl::SendFieldValue(
    FieldTypes type, int8 field_index, void* value) {
  transport_->SendByte(field_index);
  SendScalarValue(type, field_index, value);
}

void* ShineImpl::SendRepeatedField(
    FieldTypes type, int8 field_index, int32 offset, void* value) {
  SendCommand('s');
  transport_->SendByte(field_index);
  transport_->SendUInt32(offset);
  SendScalarValue(type, field_index, value);
}

void* ShineImpl::SendScalarValue(
    FieldTypes type, int8 field_index, void* value) {
  switch (type) {
    case INT8:
      transport_->SendByte(*((int8 *)value)); 
      break;
    case INT16:
      transport_->SendUInt16(*((int16 *)value)); 
      break;
  }
  return (void *)&current_field_;
}

// NOTE(jmcgill): This must be generated per proto buffer. Consider breaking
// out of the class.
void ShineImpl::HandleRPCCall() {
  int8 rpc_index = transport_->ReadByte();
  HandleRPCCall_Generated(this, rpc_index);
}

void ShineImpl::ReadBundledFields() {
  int8 count = transport_->ReadByte();
  for (int i = 0; i < count; ++i) {
    HandleReceivedField();
  } 
}

void ShineImpl::write(char* data) {
  SendCommand('p');

  // TODO(jmcgill): Consider adding line numbers back in. Means Write must
  // be a macro.

  // Write the debug string.
  Write_(data);  
  transport_->SendByte(0);
}

void ShineImpl::Write_(char* data) {
  while (*data) {
    transport_->SendByte(*data);
    ++data;
  }
}

void ShineImpl::RequestField(int8 index) {
  SendCommand('r');
  transport_->SendByte(index);

  // Read back the value.
  HandleReceivedField();
}

void* ShineImpl::RequestRepeatedField(int8 index, int32 offset) {
  SendCommand('r'); 
  transport_->SendByte(index);
  transport_->SendUInt32(offset);
  
  // Read back the value.
  int8 field_index = transport_->ReadByte(); // Ignored
  FieldTypes field_type = input_->type(index);

  // TODO(jmcgill): Work out how this value survives on the stack. Might need
  // a RequestRepeatedField for each type (or size) of repeated field?
  return ReadField(field_type);
}

void ShineImpl::SendScalarFields(Message* message) {
  // Send the number of scalar fields which will be sent.
  transport_->SendByte(message->num_scalar_fields());

  // Send these fields.
  for (int8 i = 0; i < message->num_fields(); ++i) {
    if (!message->repeated(i)) {
      SendFieldValue(message->type(i), i, message->get_value(i));
    } 
  }
}

void ShineImpl::SendNoScalarFields() {
  transport_->SendByte(0);
}
