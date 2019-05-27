import struct
import sys

filename = sys.argv[1]

fmt = struct.Struct('<Iffffff')

with open(filename, 'rb') as f:
    while True:
        chunk = f.read(fmt.size)
        print(fmt.unpack(chunk))
