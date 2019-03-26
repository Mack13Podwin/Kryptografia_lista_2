from math import log, ceil
from sys import argv

from RC4 import RC4


if len(argv) > 4:
	n, l, d, t, mode = int(argv[1]), int(argv[2]), int(argv[3]), int(argv[1]), ''
	if len(argv) == 5:
		mode = argv[4]
		t = ceil(2*n*log(n))
	print(n, l, d, t, mode)
	rc4 = RC4(l, n, mode, t)
	print(rc4.mdrop(d, 10))
else:
	rc4 = RC4(40, 16, '', 16)
	rc4.mdrop(0)
