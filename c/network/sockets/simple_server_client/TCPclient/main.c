#include <stdio.h>

// Sockets libraries
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

// Structs needs to store address informations
#include <netinet/in.h>

// Used to convert ip addresses
#include <arpa/inet.h>

int main(void)
{
    // Will contain the file description of the socket I think?
    int network_socket;
    
    // SOCK_STREAM is for TCP type
    network_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Must specify the remote socket to connect with so we use 
    // netinet/in.h to contain information of the remote socket
    
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = inet_addr("0.0.0.0");
    // server_address.sin_addr.s_addr = INADDR_ANY;
    
    int connection_status = connect(network_socket, (struct sockaddr*) &server_address, sizeof(server_address));

    if (connection_status != 0) {
        perror("connect");
        return 1;
    }

    puts("Socket connected.");
    
    // We also recieve data from the server
    char server_response[1024];
    recv(network_socket, server_response, sizeof(server_response), 0);

    // Print out the server's response
    printf("Server Response: %s\n", server_response);

    // Then we close the socket
    close(network_socket);

    return 0;
}
