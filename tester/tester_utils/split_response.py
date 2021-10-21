import sys

def main(argv):
	try :
		fd = open(argv[0], 'r')
	except :
	 	return 1
	
	try :
		header_fd = open(argv[1], 'w')
	except :
		fd.close()
		return 2

	try :
		body_fd = open(argv[2], 'w')
	except :
		header_fd.close()
		fd.close()
		return 3

	try :
		txt = fd.read()
		fd.close()
		txt = txt.split("\r\n\r\n", 1)
		if (len(txt) >= 1):
			txt[0] += "\r\n\r\n"
			header_fd.write(txt[0])
		header_fd.write("\n")

		if (len(txt) == 2):
			body_fd.write(txt[1])
		body_fd.write("\n")
	except :
		header_fd.close()
		body_fd.close()
		return 5
	header_fd.close()
	body_fd.close()
	return 0

if len(sys.argv) < 3 :
	print("ERROR : need 3 argument")
	sys.exit(1)
else :
	sys.exit(main(sys.argv[1:]))