import struct
import sys

if len(sys.argv) == 1:
    print('Usage: python3 %s /Volumes/.../DATALOG.CSV' % sys.argv[0])
    print('It is not a CSV lol')
    exit(1)

filename = sys.argv[1]

fmt = struct.Struct('<Ifffffffffddd')

with open(filename, 'rb') as f:
    while True:
        chunk = f.read(fmt.size)
        row = fmt.unpack(chunk)
        time, ax, ay, az, rx, ry, rz, temp, pres, alt, gps_lat, gps_long, gps_alt = fmt.unpack(chunk)
        print(row)
