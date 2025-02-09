/* Inclusio de fitxers .h habituals */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <string.h>

// Inclusion de la fachada para los sockets
#include "../socket_facade/socket_facade.h"

// Librerias para los sockets
#include <unistd.h>
#include <arpa/inet.h>


/**
 * @brief Funció principal del CLIENT
 *
 * @param argc Número de paràmetres
 * @param argv Paràmetres
 * @param argv[1] Adreça IP del servidor
 * @param argv[2] Port del servidor
 */
int main(int argc, char **argv)
{

    if (argc == 3)
    {
        int s;                           /* El socket */
        struct sockaddr_in adr_servidor; /* Dirección i puerto del servidor */
        char paquet[TAMAÑO_PAQUETE]; /* Para poner los datos a enviar/recibir */

        /* Volem socket d'internet i no orientat a la connexio */
        s = get_socket();

        adr_servidor = get_address(argv[1], atoi(argv[2]));

        /* Demanem el nom del usuari i l'enviem al servidor */
        printf("Introdueix el teu nom: ");
        scanf("%s", paquet);
        send_data(s, paquet, adr_servidor);

        /* Rebem el missatge de benvinguda i el mostrem */
        receive_data(s, paquet, NULL);
        printf("%s\n", paquet);

        /* Demanem la dificultat i l'enviem al servidor*/
        int dificultat;
        scanf("%d", &dificultat);
        sprintf(paquet, "%d", dificultat);
        send_data(s, paquet, adr_servidor);
        printf("Introdueix un numero:");

        int num;
        bool endevinat = false;

        while (!endevinat)
        {
            /* Demanem el número al usuari i l'enviem al servidor */
            scanf("%d", &num);
            sprintf(paquet, "%d", num);
            send_data(s, paquet, adr_servidor);

            /* Rebem la resposta del servidor */
            receive_data(s, paquet, NULL);
            printf("%s\n", paquet);

            /* Si la resposta del servidor conté "Felicitats", hem endevinat el número */
            if (strstr(paquet, "Felicitats") != NULL)
            {
                endevinat = true;
            }
        }

        /* Tanquem el socket */
        close(s);
    }
    else
    {
        printf("El nombre de paràmetres no és el correcte!\n");
    }

    return 0;
}
