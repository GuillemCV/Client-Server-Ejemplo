// Inclusión de archivos .h habituales
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <string.h>

// Inclusión de la fachada para los sockets
#include "../socket_facade/socket_facade.h"

// Librerías para los sockets
#include <unistd.h>
#include <arpa/inet.h>

/**
 * @brief Funcion principal del CLIENTE
 *
 * @param argc Número de parámetros
 * @param argv Parámetros
 * @param argv[1] Dirección IP del servidor
 * @param argv[2] Puerto del servidor
 */
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("El número de parámetros no es correcto!\n");
        exit(1);
    }

    int socket_cliente = get_socket();                                           // Creamos el socket del cliente
    struct sockaddr_in direccion_servidor = get_address(argv[1], atoi(argv[2])); // struct con la información de la dirección del servidor
    char paquete[TAMAÑO_PAQUETE];                                                // Para poner los datos a enviar/recibir

    // Pedimos el nombre del usuario y lo enviamos al servidor
    printf("Introduce tu nombre: ");
    scanf("%s", paquete);
    send_data(socket_cliente, paquete, direccion_servidor);

    // Recibimos el mensaje de bienvenida y lo mostramos
    receive_data(socket_cliente, paquete, NULL);
    printf("%s\n", paquete);

    // Pedimos la dificultad y la enviamos al servidor
    int dificultad;
    scanf("%d", &dificultad);
    sprintf(paquete, "%d", dificultad);
    send_data(socket_cliente, paquete, direccion_servidor);
    printf("Introduce un número:");

    int numero;
    bool acertado = false;
    while (!acertado)
    {
        // Pedimos el número al usuario y lo enviamos al servidor
        scanf("%d", &numero);
        sprintf(paquete, "%d", numero);
        send_data(socket_cliente, paquete, direccion_servidor);

        // Recibimos la respuesta del servidor
        receive_data(socket_cliente, paquete, NULL);
        printf("%s\n", paquete);

        // Si la respuesta del servidor contiene "Felicitaciones", hemos adivinado el número
        if (strstr(paquete, "Felicitats") != NULL)
        {
            acertado = true;
        }
    }

    // Cerramos el socket
    close(socket_cliente);
    return 0;
}
