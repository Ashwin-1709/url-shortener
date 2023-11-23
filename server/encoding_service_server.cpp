#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <random> 
#include <ctime>
#include <chrono>
#include "../service/encoding_service.hh"
#include "../service/http_parser.hh"

#define LISTEN_PORT 3600
#define MAX_CLIENTS 5
#define MAX_BUFFER_SIZE 4096

void log_error(std::string error_message) {
    std::cerr << "Error: " << error_message << '\n';
    exit(0);
}

void close_fd(int sockfd, std::string socket_description) {
    if(close(sockfd) == -1) {
        log_error("unable to close " + socket_description);
    }
}

std::string process_http_request(std::string encoding_http_request, EncodingService* encoding_service) {
    auto url = get_http_body(encoding_http_request);
    auto encoded_url = encoding_service->encode(url);
    std::string shortened_url = "www.ashwin/" + encoded_url;
    return shortened_url;
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

    std::cerr << "binding complete" << '\n';

    // 3. Listen
    if(listen(server_sockfd, MAX_CLIENTS) < 0) {
        log_error("Error on listening to the port");
    }

    std::cerr << "listening" << '\n';

    // 4. Initialising the encoding service
    std::mt19937 mt(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int64_t>dist(1, 1e14);
    EncodingService *encoder = new EncodingService(dist(mt), "base62");

    while(true) {
        struct sockaddr_in client_address;
        socklen_t client_address_length = sizeof(client_address);
        int client_socket_fd = accept(server_sockfd, (struct sockaddr*)&client_address, &client_address_length);
        if(client_socket_fd < 0) {
            log_error("Unsuccessfull attempt at accepting new connection");
        }

        std::cerr << "connected to client" << '\n';
        
        // TODO: process the connection encoding request
        char buffer[MAX_BUFFER_SIZE];
        bzero((char *)buffer, sizeof(buffer));
        std::string encoding_http_request;
        ssize_t byted_read;
        byted_read = recv(client_socket_fd, buffer, sizeof(buffer), 0); 
        encoding_http_request.append(buffer, byted_read);
        

        std::cerr << "http request received" << '\n';
        if(byted_read == -1) {
            close_fd(client_socket_fd, "client socket");
            close_fd(server_sockfd, "server socket");
            log_error("Reading from client socket");
        }

        auto short_url = process_http_request(encoding_http_request, encoder);
        auto http_response = form_http_response(short_url);
        ssize_t send_bytes = send(client_socket_fd, http_response.c_str(), http_response.size(), 0);

        if(send_bytes == -1) {
            close_fd(client_socket_fd, "client socket");
            close_fd(server_sockfd, "server socket");
            log_error("Writing to client socket");
        }

        close(client_socket_fd);
    }

    close_fd(server_sockfd, "server socket");
}