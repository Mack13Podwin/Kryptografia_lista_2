from math import ceil
from random import randint, getrandbits
from sys import stdout
import struct


class RC4:
	i = 0
	j = 0

	def __init__(self, l, n, mode='', t=16):
		self.length = l
		self.s = []
		self.n = n
		if mode == '':
			self.ksa(t)
		elif mode == 'rs':
			self.ksa_rs(t)
		elif mode == 'sst':
			self.ksa_sst()

	def prga(self):
		self.i = (self.i+1) % self.n
		self.j = (self.j+self.s[self.i]) % self.n
		self.s[self.i], self.s[self.j] = self.s[self.j], self.s[self.i]
		return self.s[(self.s[self.i]+self.s[self.j]) % self.n]

	def mdrop(self, d):
		val = 1
		if self.n == 16:
			val = 2
		elif self.n == 64:
			val = 3
		while True:
			if val==1:
				for i in range(d):
					self.prga()
				stdout.buffer.write(struct.pack('>I', self.prga()))
			elif val==2:
				for i in range(2*d):
					self.prga()
				out = self.prga()<<4 +self.prga()
				stdout.buffer.write(struct.pack('>I', out))
			elif val==3:
				result = 0
				for i in range(4*(d+1)):
					result=result<<6
					result+=self.prga()
				result = list(result.to_bytes(3*(d+1), byteorder='big'))
				for i in range(3*(d+1)):
					if i%(d+1)==d:
						stdout.buffer.write(struct.pack('>I',result[i]))


	def ksa(self, t):
		k = [randint(0, 256) for i in range(self.length)]
		s = [i for i in range(self.n)]
		j = 0
		for i in range(t):
			j = (j+s[i % self.n]+k[i % self.length]) % self.n
			s[i % self.n], s[j % self.n] = s[j % self.n], s[i % self.n]
		self.s = s

	def ksa_rs(self, t):
		k = [getrandbits(1) for i in range(self.length)]
		s = [i for i in range(self.n)]
		for r in range(t):
			top = []
			bottom = []
			for i in range(self.n):
				if k[(r*self.n + i) % self.length] == 0:
					top.append(s[i])
				else:
					bottom.append(s[i])
			s = top + bottom
		self.s = s

	def ksa_sst(self):
		k = [randint(0, 256) for i in range(self.length)]
		s = [i for i in range(self.n)]
		pi = [0 for i in range(self.n)]
		marked = 0
		ceiling = ceil((self.n-1)/2)
		i, j = 0, 0
		while marked < self.n:
			j = (j+s[i % self.n]+k[i % self.length]) % self.n
			s[i % self.n], s[j % self.n] = s[j % self.n], s[i % self.n]
			pi[i % self.n], pi[j % self.n] = pi[j % self.n], pi[i % self.n]
			if marked < ceiling:
				if pi[i % self.n] == pi[j % self.n] == 0:
					pi[i] = 1
					marked += 1
			elif (pi[i % self.n] != pi[j % self.n] and pi[i % self.n] == 0) or (pi[i % self.n] == 0 and i == j):
				pi[i] = 1
				marked += 1
			i = (i+1) % self.n
		self.s = s
