
#define PORT 9002
#define IP_ADDRESS "0.0.0.0"

#include <stdio.h>

// Sockets libraries
#include <sys/types.h>
#include <sys/socket.h>

// Structs for sockets
#include <netinet/in.h>

#include <arpa/inet.h>
#include <unistd.h>

int main(void)
{
    // Use to hold the message to respond
    char server_message[255] = "You have reached the server!";

    // Create the socket
    int socket_server;
    socket_server = socket(AF_INET, SOCK_STREAM, 0);
    
    // Define the server address
    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    // server_address.sin_addr.s_addr = INADDR_ANY;
    // Bind the socket to our specified IP and port

    if(bind(socket_server, (struct sockaddr*) &server_address, sizeof(server_address)) != 0) {
        perror("bind");
        return 1;
    }
    
    // Listen to our socket
    printf("Listening on socket %s:%d...\n", IP_ADDRESS, PORT);
    if(listen(socket_server, 5) != 0) {
        perror("listen");
        return 1;
    }

    int client_socket = -1;
    struct sockaddr_in client_info;
    socklen_t client_size = sizeof(client_info);
    
    // Accept client
    client_socket = accept(socket_server, (struct sockaddr*) &client_info, &client_size);
    
    if (client_socket == -1)
        perror("accept");

    if(client_info.sin_family == AF_INET)
        printf("Client %s:%d connected.\n", inet_ntoa(client_info.sin_addr), ntohs(client_info.sin_port));

    // send the message
    send(client_socket, server_message, sizeof(server_message), 0);
    
    // close the socket
    close(socket_server);
    return 0;
}
