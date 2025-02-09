#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define TAMAÑO_PAQUETE 500

/**
 * Fachada para simplificar el uso de sockets
 */

/**
 * @brief Función para obtener un socket
 */
int get_socket();

/**
 * @brief Función para enlazar un socket
 */
int bind_socket(int socket, struct sockaddr_in address);

/**
 * @brief Función para crear un struct sockaddr_in con la dirección y puerto especificados
 */
struct sockaddr_in get_address(const char *ip, int port);

/**
 * @brief Función para enviar datos a la dirección especificada a través del socket
 */
ssize_t send_data(int socket, const char *data, struct sockaddr_in address);

/**
 * @brief Función para recibir datos a través del socket
 */
ssize_t receive_data(int socket, char *data, struct sockaddr_in *address);
