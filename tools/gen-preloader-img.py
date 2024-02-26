#!/usr/bin/env python

import struct
import sys

def read(path):
	with open(path, "rb") as f:
		return f.read()

def write(path, data):
	with open(path, "wb") as f:
		f.write(data)

def padding(data, size, pattern = '\0'):
	return data + pattern * (size - len(data))

def align(data, size, pattern = '\0'):
	return padding(data, (len(data) + (size - 1)) & ~(size - 1), pattern)

def gen_preloader(data):
	data = align(data, 512, '\xff')
	header = (padding(struct.pack("<12sII", "EMMC_BOOT", 1, 512), 512, '\xff') +
		  padding(struct.pack("<8sIIIIIIII", "BRLYT", 1, 2048, 2048 + len(data),
			0x42424242, 0x00010005, 2048, 2048 + len(data), 1) + '\0' * 140, 512, '\xff') +
		  '\0' * 1024)
	return header + data

def main(argv):
	if len(argv) != 3:
		print "Usage: %s preloader.bin preloader.img" % argv[0]
		exit(1)
	write(argv[2], gen_preloader(read(argv[1])))

if __name__ == "__main__":
	main(sys.argv)
