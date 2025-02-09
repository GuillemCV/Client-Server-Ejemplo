/* Inclusio de fitxers .h habituals */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <string.h>

/* Inclusio de fitxers .h per als sockets */
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MIDA_PAQUET 500

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
        struct sockaddr_in adr_servidor; /* Adreça i port on hi ha el servidor */
        socklen_t adr_servidor_mida;     /* Longitud de les dades adreça i port */

        char paquet[MIDA_PAQUET]; /* Per posar les dades a enviar/rebre */

        /* Volem socket d'internet i no orientat a la connexio */
        s = socket(AF_INET, SOCK_DGRAM, 0);

        adr_servidor.sin_family = AF_INET;                 /* Socket a Internet */
        adr_servidor.sin_addr.s_addr = inet_addr(argv[1]); /* Adreça on està el servidor */
        adr_servidor.sin_port = htons(atoi(argv[2]));      /* Port on escolta el servidor */

        /* Demanem el nom del usuari i l'enviem al servidor */
        printf("Introdueix el teu nom: ");
        scanf("%s", paquet);
        sendto(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&adr_servidor, sizeof(adr_servidor));

        /* Rebem el missatge de benvinguda i el mostrem */
        recvfrom(s, paquet, MIDA_PAQUET, 0, NULL, NULL);
        printf("%s\n", paquet);

        /* Demanem la dificultat i l'enviem al servidor*/
        int dificultat;
        scanf("%d", &dificultat);
        sprintf(paquet, "%d", dificultat);
        sendto(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&adr_servidor, sizeof(adr_servidor));
        printf("Introdueix un numero:");

        int num;
        bool endevinat = false;

        while (!endevinat)
        {
            /* Demanem el número al usuari i l'enviem al servidor */
            scanf("%d", &num);
            sprintf(paquet, "%d", num);
            sendto(s, paquet, MIDA_PAQUET, 0, (struct sockaddr *)&adr_servidor, sizeof(adr_servidor));

            /* Rebem la resposta del servidor */
            recvfrom(s, paquet, MIDA_PAQUET, 0, NULL, NULL);
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
