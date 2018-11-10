#include <iostream>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread>


class Server {
    private:
        struct sockaddr_in server;
        struct sockaddr_in client;
        int _socket;
        int addr_length;
        int _accept; 
        char message[5000];
        int port;
        void send_message(int);
        void receive_message(int);
    public:
        int make_socket(int argc, char* argv[]);

};

int Server::make_socket(int argc, char* argv[])
{
    if (argc != 2){
        std::cout << "You must enter port number!" << std::endl;
    }

    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == -1){
        perror( "error: problem with makeing socket"); 
        return 0;
    }

    memset(&server, 0, sizeof(server));
    server.sin_addr.s_addr = INADDR_ANY;
    port = atoi(argv[1]);
    server.sin_port = htons(port);
    server.sin_family = AF_INET;

    if (bind(_socket, (struct sockaddr*)&server, sizeof(server)) < 0){
        perror("error: problem with binding"); 
        close(_socket);
        return 0;
    }

    if (listen(_socket, 10) < 0){
        std::cout << "error: problem with listening"<< std::endl;
        return 0;
    }

    puts( "waiting for incoming connections...."); 
    
    addr_length = sizeof(struct sockaddr_in);

    _accept = accept(_socket, (struct sockaddr*)&client, (socklen_t*)&addr_length);

    if (addr_length < 0){
        perror("error: problem with accepting"); 
        close(_socket);
    }


    std::thread t1(&Server::send_message, this, _accept);
    std::thread t2(&Server::receive_message, this, _accept);
    t1.join();
    t2.join();
}

void Server::receive_message(int addr_length)
{

    while (1) {
        recv(addr_length, message, sizeof(message), 0);
        std::cout << "read message" << std::endl;

       /* if (strlen(message) == 0){ 
            exit(2);
        }*/
        puts(message);
        bzero(message, 5000);
    }
}

void Server::send_message(int addr_length)
{

    std::cout << "SERVER" << std::endl;

    while(1){
        std::cin.getline(message, 2000);
        std::cout << "write message" << std::endl;
        send(addr_length, message, sizeof(message), 0);
    }
}

int main(int argc, char* argv[])
{
    Server o;
    o.make_socket(argc, argv);

}
