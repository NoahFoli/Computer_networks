import socket

#socket server in python that listens on port 1234 and echoes back any data it receives
def main():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) #make a socket object
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)#allow reuse of the address
    s.bind(("127.0.0.1", 1234))#bind the socket to the address and port
    s.listen()#listen for incoming connections
    conn, addr = s.accept()#accept a connection
    data = conn.recv(1024)#receive data from the client
    print(f"Received data: {data.decode()}")#print the received data
    conn.send(data)#echo the data back to the client
    s.close()#close the socket


if __name__ == "__main__":
    main()