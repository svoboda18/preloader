import sys
import os

def valb_to_hex(valb):
	result = ''
	val_high = valb / 16
	val_low = valb % 16

	if val_high > 9:
		result += chr(ord('A') + val_high - 10)
	else:
		result += chr(ord('0') + val_high)

	if val_low > 9:
		result += chr(ord('A') + val_low - 10)
	else:
		result += chr(ord('0') + val_low)
		
	return result

def get_file_sizeb(file_path):
	if not os.path.isfile(file_path):
		return 0
	file_handle = open(file_path, "rb")
	file_handle.seek(0, 2)
	file_size = file_handle.tell()
	file_handle.close()
	return file_size

def usage():
	print "[USAGE] python bin_to_txt.py input.bin output.txt"
	return

def main():
	#parameter parsing
	if (len(sys.argv) > 3):
		usage()
	input_path = sys.argv[1]
	output_path = sys.argv[2]

	print "input_path: " + input_path
	print "output_path: " + output_path

	input_file_size = get_file_sizeb(input_path)

	input_file = open(input_path, 'rb')
	output_file = open(output_path, 'w')
	while(input_file_size > 0):
		ch = input_file.read(1)
		output_file.write(valb_to_hex(ord(ch)))
		input_file_size -= 1
	output_file.close()
	input_file.close()

	return

if __name__ == '__main__':
	main()
