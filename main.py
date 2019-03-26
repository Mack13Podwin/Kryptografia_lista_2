import struct
from math import log, ceil
from sys import argv, stdout

from RC4 import RC4

# for n in [16, 64, 246]:
# 	for k in [40, 64, 128]:
# 		for d in range(4):
# 			rc4 = RC4(k, n, t=n)
# 			rc4_rs = RC4(k, n, 'rs', ceil(2*n*log(n)))
# 			rc4_sst = RC4(k, n, 'sst')
# 			bits = rc4.mdrop(k, pow(10, 7))
# 			bits2 = rc4_rs.mdrop(k, pow(10, 7))
# 			bits3 = rc4_sst.mdrop(k, pow(10, 7))
if len(argv) > 4:
	n, l, d, t, mode = int(argv[1]), int(argv[2]), int(argv[3]), int(argv[1]), ''
	if len(argv) == 5:
		mode = argv[4]
		t = ceil(2*n*log(n))
	print(n, l, d, t, mode)
	rc4 = RC4(l, n, mode, t)
	print(rc4.mdrop(d, 10))
else:
	rc4 = RC4(40, 16, t=16)
	val = rc4.mdrop(0, pow(10, 9))
	# file.write(bytes(val))
