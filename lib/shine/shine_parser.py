#!/usr/bin/python
#
# A parser (based on http://christophe.delord.free.fr/sp/sp.html) for the Shine
# IDL DSL.
#
# Parses a *.shn file to an array of RPC and Message objects.

import sp

class FieldTypes:
  INT8 = 'INT8'
  INT16  = 'INT16'
  BOOL = 'BOOL'

def Return(*args):
  """Allows a static value to be returned when parsing an expression."""
  return lambda x: args[0]
 
class Field():
  """Represents a typed field in a message."""
  def __init__(self, _type, name, index):
    self.type_ = _type
    self.name_ = name
    self.index_ = index
    self.repeated_ = False

  def __repr__(self):
    return str(self.index_) + ': ' + str(self.type_) + ' ' + self.name_

class RepeatedField(Field):
  """Represents a repeated typed field in a message."""
  def __init__(self, _type, name, index):
    Field.__init__(self, _type, name, index)
    self.repeated_ = True

  def __repr__(self):
    return Field.__repr__(self) + ' (repeated)'

class Message():
  """Represents a named message, with an arbitrary number of fields."""
  def __init__(self, name, fields):
    self.name_ = name
    self.fields_ = fields
    self.num_fields_ = len(fields)
    self.num_scalar_fields_ = 0

    for field in fields:
      if not field.repeated_:
        self.num_scalar_fields_ += 1

  def __repr__(self):
    return self.name_ + ': ' + str(self.fields_)

class RPC():
  """Represents a named RPC declaration."""
  def __init__(self, name, _input, output, index):
    self.name_ = name
    self.input_ = _input
    self.output_ = output
    self.index_ = index

  def __repr__(self):
    return ('RPC ' + str(self.index_) + ': ' + self.name_ + 
        ' (' + self.input_ + ') ' + self.output_)

class RPCLite(RPC):
  """ Represents a named RPC declaration without an output."""
  def __init__(self, name, _input, index):
    RPC.__init__(self, name, _input, None, index)
   
ShineParser = sp.compile(r'''
    # Tokens.
    int8 = r'int8' : `Return(FieldTypes.INT8)`;
    bool = r'bool' : `Return(FieldTypes.BOOL)` ;
    int16 = r'int16' : `Return(FieldTypes.INT16)` ;

    # variableName in this declaration is just sugar for error messages.
    variable = variableName.r'[a-zA-Z][a-zA-Z0-9_-]*';
    number = number.r'\d+' : `int` ;

    # Define what the parser treats as a separator.
    comment = r'//.*';
    separator: r'\s+'|comment;

    # Grammar. Methods after :: are passed the matched tokens and return an
    # object to pass up the AST.
    !file = (message | rpc | rpc_lite )+;
    message = 'message' variable '{' expression+ '}' :: `Message`;
    expression = (field | repeated_field);
    field = type variable '=' number ';' :: `Field`; 
    repeated_field = 'repeated' type variable '=' number ';' :: 
        `RepeatedField`;
    rpc_lite = 'RPC' variable '(' variable ')' '=' number ';' :: `RPCLite`;
    rpc = 'RPC' variable '(' variable ')' variable '=' number ';' :: `RPC`;
    type = int8 | bool | int16;
''')

if __name__ == '__main__':
  # Parse the protocol stored in protocol.shn
  f = open('protocol.shn', 'r')
  print ShineParser(f.read())
  f.close()
