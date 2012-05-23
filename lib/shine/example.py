import jinja2
import serial
import shine_parser
import time

class SerialTransport():
  def __init__(self, device, baud):
    self.serial_ = serial.Serial(device, baud)
    
  def ReadByte(self):
    value = self.serial_.read(1)
    return value

  def ReadUInt16(self):
    high = self.ReadByte()
    low = self.ReadByte()
    return (ord(high) << 8) + ord(low)

  def ReadUInt32(self):
    high = self.ReadUInt16()
    low = self.ReadUInt16()
    return (high << 16) + low

  def SendByte(self, value):
    if not type(value) == str:
      value = chr(value)
    self.serial_.write(value)

  def SendUInt16(self, value):
    self.SendByte((value >> 8) & 0xFF)
    self.SendByte(value & 0xFF)

  def SendUInt32(self, value):
    self.SendByte((value >> 8) & 0xFF)
    self.SendByte(value & 0xFF)

class Shine:
  def __init__(self, port, baud, filename):
    self.transport_ = SerialTransport(port, baud)
    
    # Sleep 2 seconds while the Arduino resets.
    time.sleep(2)

    f = open(filename, 'r')
    ast = shine_parser.ShineParser(f.read())
    
    messages = []
    rpcs = [] 
    for entry in ast:
      if entry.__class__ == shine_parser.Message:
        messages.append(entry)
      else:
        rpcs.append(entry)

    template_file = open('shine_python.tmpl', 'r')
    template = jinja2.Template(template_file.read())
    output = template.render(messages = messages, rpcs = rpcs)  
    f = open('out.py', 'w')
    f.write(output)
   
    print output
    exec(output, globals())
    print Input

  def GetEarlyScalarValues(self, message):
    # Maximum number of bytes to send.
    limit = 0
    current_size = 0
    max_index = -1
    for i in range(0, message.num_fields()):
      current_size += self.GetFieldSize(message.field(i))
      if current_size < limit:
        max_index = i
    return max_index + 1

  def GetFieldSize(self, field_type):
    if field_type == shine_parser.FieldTypes.INT8:
      return 1
    elif field_type == shine_parser.FieldTypes.INT16:
      return 2
    elif field_type == shine_parser.FieldTypes.INT32:
      return 4
    elif field.type() == shine_parser.FieldTypes.BOOL:
      return 1
    return 0
  
  def CallRPC(self, index):
    self.SendCommand('c')
    self.transport_.SendByte(index)

  def SendCommand(self, command):
    # All command begin with !
    self.transport_.SendByte('!')
    self.transport_.SendByte(command)

  def ProcessCommands(self):
    running = True
    while running:
      byte = self.transport_.ReadByte()
      if byte is not None:
        if not byte == '!':
          print byte
        else: 
          running = self.HandleCommand(self.transport_.ReadByte())

  def HandleCommand(self, command):
    if command == 'r':
      self.HandleRequestForField()
      return True
    elif command == 's':
      self.HandleReceivedField()
      return True
    elif command == 'c':
      self.HandleRPCCall()
      return True
    elif command == 'p':
      self.HandlePrint()
      return True
    elif command == 'x':
      # RPC complete.
      return False 
    else:
      raise 'Unknown Shine Command', command

  def HandlePrint(self):
    string = ''
    byte = self.transport_.ReadByte()
    while not ord(byte) == 0:
      string += byte
      byte = self.transport_.ReadByte()
    print 'Debug: ', string
 
  def HandleRequestForField(self): 
    field_index = ord(self.transport_.ReadByte())
    field_type = self.input_.field(field_index)
    if self.input_.repeated(field_index):
      # Read a 32 byte index for repeated fields.
      index = self.transport_.ReadUInt32()
      if index >= len(self.input_.value(field_index)):
        self.SendFieldValue(field_type, field_index, 0)
        return
      value = self.input_.value(field_index)[index]
      self.SendFieldValue(field_type, field_index, value)
    else:    
      value = self.input_.value(field_index)
      self.SendFieldValue(field_type, field_index, value)

  def SendField(self, field_type, field_index_value):
    # Initiate a field transfer.
    self.SendCommand('s')
    self.SendFieldValue(field_type, field_index, value)

  def SendFieldValue(self, field_type, field_index, value):
    self.transport_.SendByte(field_index)

    if field_type == shine_parser.FieldTypes.INT8:
      self.transport_.SendByte(value)
    elif field_type == shine_parser.FieldTypes.INT16:
      self.transport_.SendUInt16(value)
    elif field_type == shine_parser.FieldTypes.BOOL:
      self.transport_.SendByte(value)

  def HandleReceivedField(self): 
    field_index = self.transport_.ReadByte()
    field_type = self.input_.field(field_index)
    if self.input_.repeated(field_index):
      # Read a 32 byte index for repeated fields.
      index = self.transport_.ReadUInt32()
      value = self.ReadField(field_type)
      self.input_.value(field.index())[index] = value
    else:    
      value = self.ReadField(field_type)
      self.output_.set_value(field_index, value)

  def ReadField(self, field_type):
    if field_type == shine_parser.FieldTypes.INT8:
      return self.transport_.ReadByte()
    elif field_type == shine_parser.FieldTypes.INT16:
      return self.transport_.ReadUInt16()
    elif field_type == shine_parser.FieldTypes.BOOL:
      return self.transport_.ReadByte()

  def HandleRPCCall(self):
    rpc_index = self.transport_.ReadByte()
    # No Arduino -> PC RPC calls defined.

  def CreateRPC(self, name):
    return lambda x: globals()[name](self, x)
 
  def __getattr__(self, attr):
    # This is called when attempting to execute an RPC.

    # Check to see if it was defined. 
    expected_name = "Remote_" + attr
    if (expected_name) in globals():
      return self.CreateRPC(expected_name)
    else:
      print "Call to unknown RPC: " + attr

