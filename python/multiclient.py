from socket import *
import errno
import sys

HEADER_LENGTH=10

def main():
    username = input(f"Enter username: ")
    #print(f"{len(username):<{HEADER_LENGTH}}{username}")
    #quit()
    with socket(AF_INET, SOCK_STREAM) as s:
        s.connect(("127.0.0.1", 1234))
        s.setblocking(False)#set socket to a non blocking state so we dont wait for recv() to return something
        s.send(f"{len(username):<{HEADER_LENGTH}}{username}".encode())

        while True:
            message = input(f"{username}> ")
            if message:
                #s.send(message.encode())
                s.send(f"{len(message):<{HEADER_LENGTH}}{message}".encode())

            
            try:
                while True:
                    recv_header = int(s.recv(HEADER_LENGTH).decode())
                    recv_message = s.recv(recv_header).decode()
                    print(f"{recv_message}")
                    #recv_message = s.recv(1024).decode()
                    #print(f"{recv_message}")
            except IOError as e:
                if e.errno != errno.EAGAIN and e.errno != errno.EWOULDBLOCK:
                    print(f"reading error: {str(e)}")
                    sys.exit()
                continue

if __name__ == "__main__":
    main()