import struct
import sys

if len(sys.argv) == 1:
    print('Usage: python3 %s /Volumes/.../DATALOG.CSV' % sys.argv[0])
    print('It is not a CSV lol')
    exit(1)

filename = sys.argv[1]

fmt = struct.Struct('<Ifffffffff')

with open(filename, 'rb') as f:
    while True:
        chunk = f.read(fmt.size)
        row = fmt.unpack(chunk)
        time, ax, ay, az, rx, ry, rz, temp, pres, alt = fmt.unpack(chunk)
        print(row)
