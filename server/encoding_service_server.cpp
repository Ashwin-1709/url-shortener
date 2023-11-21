#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include "../service/base_62_encoding.hh"
#define LISTEN_PORT 3600
#define MAX_CLIENTS 5

void log_error(std::string error_message) {
    std::cerr << "Error: " << error_message << '\n';
    exit(0);
}

int main(int argc, char *argv[]) {
    int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sockfd < 0) {
        log_error("Initialising server socket file descriptor");
    }

    struct sockaddr_in server_address;
    bzero((char *)&server_address, sizeof(server_address));
    
    //  1. Initialise server address
    {
        server_address.sin_port = htons(LISTEN_PORT);
        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = INADDR_ANY;
    }

    //  2. Binding the server address to socket
    if(bind(server_sockfd, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
        log_error("Server address binding unsuccessfull");
    }

    // 3. Listen
    if(listen(server_sockfd, MAX_CLIENTS) < 0) {
        log_error("Error on listening to the port");
    }


    while(true) {
        struct sockaddr_in client_address;
        socklen_t client_address_length = sizeof(client_address);
        int client_socket_fd = accept(server_sockfd, (struct sockaddr*)&client_address, &client_address_length);
        if(client_socket_fd < 0) {
            log_error("Unsuccessfull attempt at accepting new connection");
        }
        
        // TODO: process the connection encoding request

        close(client_socket_fd);
    }
}