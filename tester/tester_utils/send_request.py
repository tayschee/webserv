import sys, getopt, os, socket, time

def client(txt, argv) :
	try :
		sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM) #cree une socket pour communiquer
	except :
		return ""
	txt2 = ""
	try :
		sock.connect((argv[0], int(argv[1]))) #destinataire de la com
		sock.send(txt) #envoie du message
		while 1:
			txt1 = sock.recv(4096)
			if txt1 == "" :
				break
			txt2 += txt1

		sock.close() #arrete la socket
		return txt2
	except :
		print "Server error"
		sock.close() #arrete la socket
		return ""

def main(argv) :
	try :
		fd = open(argv[0], "r") # ouvre le fichier en parametre
		txt1 = fd.read() # l'affiche
		txt2 = client(txt1, argv[1:])
		print(txt2)
	except :
		print("ERROR : fd")

if len(sys.argv) < 4 :
	print("ERROR : need an argument")
else :
	main(sys.argv[1:])