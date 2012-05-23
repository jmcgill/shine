# Copyright James McGill, 2011
# Author: James McGill (jmcgill@plexer.net)

import shine

def main():
  client = shine.Shine(
      '/dev/tty.usbserial-A9007MGz',  # Serial port.
      9600,                           # Baud rate. 
      '/Users/plexer/flash.shn')      # Protocol file.

  count = int(raw_input("How many times to flash?: "))
 
  # A pattern of delays to send to the Arduino. The LED will flash faster each
  # time.
  delays = [1000, 900, 800, 700, 600, 500, 400, 300, 200, 100]
  input = shine.Input(count, delays)

  # Call the RPC. This will be executed on the arduino.
  client.Test(input)

if __name__ == '__main__':
  main()
