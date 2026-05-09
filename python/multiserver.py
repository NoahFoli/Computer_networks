from socket import *
import select

HEADER_LENGTH=10

def main():
    with socket(AF_INET, SOCK_STREAM) as s:
        s.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
        s.bind(("127.0.0.1", 1234))
        s.listen()

        socket_list = [s]
        clients = {}

        while True:
            read_sockets, _, exception_sockets = select.select(socket_list, [], socket_list)
            for notified_socket in read_sockets:
                if notified_socket == s:
                    # new client connection
                    client_socket, client_address = s.accept()
                    socket_list.append(client_socket)

                    header = int(client_socket.recv(HEADER_LENGTH).decode())
                    username = client_socket.recv(header).decode()
                    clients[client_socket] = username
                else:
                    # message from client
                    header = int(notified_socket.recv(HEADER_LENGTH))
                    message = notified_socket.recv(header).decode()
                    print(f"Received from {clients[notified_socket]}: {message}")

                    to_send = f"{clients[notified_socket]}: {message}"
                    to_send = f"{len(to_send):<{HEADER_LENGTH}}{to_send}".encode()
                    for client_socket in clients:
                        if client_socket != notified_socket:
                            client_socket.send(to_send)


if __name__ == "__main__":
    main()