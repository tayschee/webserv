import sys, getopt, os, socket, time

def client(txt) :
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM) #cree une socket pour communiquer
	sock.connect(('localhost', 80)) #destinataire de la com
	sock.send(txt) #envoie du message
	txt2 = sock.recv(10000)
	sock.close() #arrete la socket
	return txt2

def main(argv) :
	fd = open(argv[0], "r") # ouvre le fichier en parametre
	txt1 = fd.read() # l'affiche
	print(txt1)
	txt2 = client(txt1)
	print("/////////////////////////////////////////////////////////////////")
	print(txt1)
	print("-----------------------------------------------------------------")
	print(txt2)
	print("/////////////////////////////////////////////////////////////////")

if len(sys.argv) < 2 :
	print("ERROR : need an argument")
else :
	main(sys.argv[1:])