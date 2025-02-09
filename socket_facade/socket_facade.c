#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "socket_facade.h"

int get_socket() {
    return socket(AF_INET, SOCK_DGRAM, 0);
}

int bind_socket(int socket, struct sockaddr_in address) {
    return bind(socket, (struct sockaddr *)&address, sizeof(address));
}

struct sockaddr_in get_address(const char *ip, int port) {
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    if (ip == NULL) {
        address.sin_addr.s_addr = INADDR_ANY;
    } else {    
        address.sin_addr.s_addr = inet_addr(ip);
    }
    address.sin_port = htons(port);
    return address;
}

ssize_t send_data(int socket, const char *data, struct sockaddr_in address) {
    return sendto(socket, data, TAMAÑO_PAQUETE, 0, (struct sockaddr *)&address, sizeof(address));
}

ssize_t receive_data(int socket, char *data, struct sockaddr_in *address) {
    if (address == NULL) {
        return recvfrom(socket, data, TAMAÑO_PAQUETE, 0, NULL, NULL);
    } else {
        socklen_t address_size = sizeof(*address);
        return recvfrom(socket, data, TAMAÑO_PAQUETE, 0, (struct sockaddr *)address, &address_size);
    }
}
