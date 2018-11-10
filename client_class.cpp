#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <thread>

class Client {
    private:
        struct sockaddr_in _server;
        int _socket;
        int port;
        char message[5000];
        int send_message(int);
        int receive_message(int);
        int _connection;
    public:
        int make_connection(int argc, char* argv[]);
};

int Client::make_connection(int argc, char* argv[])
{
    if (argc != 2){
        std::cout << "You must enter port number" << std::endl;
        exit(0);
    }

    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket < 0){
        perror("error: the problem with makeing socket");
        return 1;
    }

    memset(&_server, 0, sizeof(_server));
    _server.sin_family = AF_INET;
    _server.sin_addr.s_addr = INADDR_ANY;
    port = atoi(argv[1]);
    _server.sin_port = htons(port);

    _connection = connect(_socket, (struct sockaddr*)&_server, sizeof(_server));
    if(_connection == -1){
        perror("error: the problem with connecting");
        return 1;
    }

    std::thread send_t(&Client::send_message, this, _socket);//thread for sending message
    std::thread recv_t(&Client::receive_message, this, _socket);//thread for receiving message
    send_t.join();
    recv_t.join();
}

int Client::send_message(int _soc)
{
    std::cout << "CLIENT" << std::endl;

    while(1){
        std::cin.getline(message, sizeof(message));
        std::cout << "enter message" << std::endl;
        send(_soc, message, sizeof(message), 0);
    }
}

int Client::receive_message(int _soc)
{
    while(1){
        recv(_soc, message, sizeof(message), 0);
        std::cout << "read message" << std::endl;

        puts(message);
        bzero(message, 5000);
    }
}

int main(int argc, char* argv[])
{
    Client ob;
    ob.make_connection(argc, argv);
}
