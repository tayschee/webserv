import sys, getopt, os, socket, time

def client(txt, argv) :
	sock = list(argv[3])
	list_txt = list(argv[3])
	try :
		for i in range(sock) :
			sock[i] = socket.socket(socket.AF_INET, socket.SOCK_STREAM) #cree une socket pour communiquer
	except :
		return ""
	try :
		for i in sock :
			i.connect((argv[0], int(argv[1]))) #destinataire de la com
		for i in sock :
			i.send(txt) #envoie du message
		for i in range(sock) :
			list_txt[i] = sock[i].recv(1000000)
		sock.close() #arrete la socket
		return txt2
	except :
		print "Server error"
		sock.close() #arrete la socket
		return ""

def main(argv) :
	fd = open(argv[0], "r") # ouvre le fichier en parametre
	if fd == -1 :
		print("ERROR : fd")

	txt1 = fd.read() # l'affiche
	txt2 = client(txt1, argv[1:])
	print(txt2)

if len(sys.argv) < 5 :
	print("ERROR : need an argument")
else :
	main(sys.argv[1:])