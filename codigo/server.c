/* Inclusión de archivos .h habituales */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> /* Para generar números aleatorios */
#include <stdbool.h>

// Inclusión de la fachada para los sockets
#include "../socket_facade/socket_facade.h"

// Librerías para los sockets
#include <unistd.h>
#include <arpa/inet.h>

/**
 * @brief Genera un número aleatorio entre min y max
 *
 * @return Número aleatorio
 */
int numero_aleatorio(int minimo, int maximo)
{
    srand(time(NULL));
    return rand() % (maximo - minimo + 1) + minimo;
}

/**
 * @brief Funcion principal del SERVIDOR
 *
 * @param argc Número de parámetros
 * @param argv Parámetros
 * @param argv[1] Puerto donde escucha el servidor
 */
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("El número de parámetros no es correcto!\n");
        exit(1);
    }

    int socket_servidor = get_socket();                                       // Socket del servidor
    struct sockaddr_in direccion_servidor = get_address(NULL, atoi(argv[1])); // struct con la información de la dirección del servidor
    struct sockaddr_in direccion_cliente;                                     // struct con la información de la dirección del cliente
    char paquete[TAMAÑO_PAQUETE];                                             // Para poner los datos a enviar/recibir

    // Enlazamos el socket con la dirección del servidor
    int resultado_bind = bind_socket(socket_servidor, direccion_servidor);
    if (resultado_bind < 0)
    {
        printf("No se ha podido enlazar el socket!\n");
        exit(1);
    }

    printf("Servidor operativo en el puerto %d\n", atoi(argv[1]));

    int numero_aleatorio_servidor, numero_usuario;
    int minimo = 1;
    int maximo;
    int niveles[3] = {10, 50, 100};
    char nombre[20];
    bool acertado;
    int dificultad;

    while (1)
    {
        printf("Esperando petición de algún cliente...\n");

        // Recibimos el nombre del cliente
        receive_data(socket_servidor, paquete, &direccion_cliente);
        strcpy(nombre, paquete);

        printf("Petición recibida!\n");
        printf("Enviando mensaje de bienvenida...\n");

        // Enviamos un mensaje de bienvenida al cliente
        sprintf(paquete, "[SERVIDOR]: ¡Bienvenido %s! Selecciona la dificultad:\n\t\t1) del 1 al %d  \n\t\t2) del 1 al %d   \n\t\t3) del 1 al %d \n", nombre, niveles[0], niveles[1], niveles[2]);
        send_data(socket_servidor, paquete, direccion_cliente);

        // Recibimos la dificultad
        receive_data(socket_servidor, paquete, &direccion_cliente);
        sscanf(paquete, "%d", &dificultad);

        switch (dificultad)
        {
        case 1:
            maximo = niveles[0];
            break;
        case 2:
            maximo = niveles[1];
            break;
        case 3:
            maximo = niveles[2];
            break;
        default:
            maximo = niveles[2];
            break;
        }

        // Generamos un número aleatorio
        printf("Generando número aleatorio...\n");
        numero_aleatorio_servidor = numero_aleatorio(minimo, maximo);

        acertado = false;
        while (!acertado)
        {
            // Recibimos el número del cliente
            printf("Esperando número del cliente...\n");
            receive_data(socket_servidor, paquete, &direccion_cliente);
            sscanf(paquete, "%d", &numero_usuario);

            // Comprobamos si el cliente ha acertado el número
            printf("Comprobando si el cliente ha acertado el número...\n");
            if (numero_usuario == numero_aleatorio_servidor)
            {
                printf("El cliente ha acertado el número!\n");
                printf("FIN DE LA PARTIDA!\n\n\n");
                sprintf(paquete, "[SERVIDOR]: ¡Felicidades %s! ¡Has acertado el número!\n", nombre);
                acertado = true;
            }
            else
            {
                printf("El cliente no ha acertado el número...\n");
                if (numero_aleatorio_servidor > numero_usuario)
                    sprintf(paquete, "[SERVIDOR]: Lo siento %s, el número a adivinar es más grande.\nInténtalo de nuevo: ", nombre);
                else
                    sprintf(paquete, "[SERVIDOR]: Lo siento %s, el número a adivinar es más pequeño.\nInténtalo de nuevo: ", nombre);
            }

            // Enviamos el mensaje al cliente
            send_data(socket_servidor, paquete, direccion_cliente);
        }
    }

    // Cerramos el socket
    close(socket_servidor);
    return 0;
}