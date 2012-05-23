import jinja2
import os
import shine_parser

def ctype(type):
  """Map Python types to C types."""
  types = {
    'INT8': 'int8',
    'INT16': 'int16',
    'BOOL': 'bool'
  }
  return types[type]

def bitpacked(fields):
  """ Returns the repeatedness of fields, bitpacked, in blocks of 8 bits """
  repeated = [len(fields) // 8]
  for i in range(0, len(fields)):
    if fields[i].repeated_:
      block = (i // 8)
      repeated[block] |= (1 << (i - (8 * block)))
  return ','.join([hex(x) for x in repeated])

if __name__ == '__main__':
  # Get from command line
  path = '/Users/plexer/flash.shn'
  output_path = "/Applications/Arduino.app/Contents/Resources/Java/libraries/Shine/"

  filename = os.path.basename(path)
  f = open(path, 'r')
  ast = shine_parser.ShineParser(f.read())
  env = jinja2.Environment(loader = jinja2.PackageLoader('compiler', '.'))

  # C/C++ Specific filters.
  env.filters['ctype'] = ctype
  env.filters['bitpacked'] = bitpacked
 
  messages = []
  rpcs = [] 
  for entry in ast:
    if entry.__class__ == shine_parser.Message:
      messages.append(entry)
    else:
      rpcs.append(entry)

  template = env.get_template('shine_arduino.tmpl')
  output = template.render(
      messages = messages,
      rpcs = rpcs,
      filename = filename)  
  f = open(os.path.join(output_path, filename + '.h'), 'w')
  f.write(output)

  template = env.get_template('shine_arduino_cpp.tmpl')
  output = template.render(
      messages = messages,
      rpcs = rpcs,
      filename = filename)
  f = open(os.path.join(output_path, filename + '.cpp'), 'w')
  f.write(output)
