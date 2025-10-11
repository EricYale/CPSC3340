import socket

server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_socket.bind(('', 7531))

while True:
    message, address = server_socket.recvfrom(1024)
    print(message.decode())
