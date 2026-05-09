
import socket

def main():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(("127.0.0.1", 1234))
    s.send("Hello".encode())
    recvData = s.recv(1024)
    print(f"Received data: {recvData}")


if __name__ == "__main__":
    main()