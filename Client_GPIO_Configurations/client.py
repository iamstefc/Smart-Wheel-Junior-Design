import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
host ="10.24.0.207"
port =8865
s.connect((host,port))
print "Client is connected to Server"
def ts(str):
   s.send(str.encode())
   print "sent the string"
   data = ''
   #data = s.recv(1024).decode()
  # print (data)

while 2:
   r = raw_input('Send a Message: ')
   ts(r)

s.close ()
