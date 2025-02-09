/* Inclusio de fitxers .h habituals */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> /* Per generar números aleatoris */
#include <stdbool.h>

// Inclusio de la fachada para los sockets
#include "../socket_facade/socket_facade.h"

// Librerias para los sockets
#include <unistd.h>
#include <arpa/inet.h>


/**
 * @brief Genera un número aleatori entre min i max
 *
 * @return Número aleatori
 */
int num_aleatori(int min, int max)
{
    srand(time(NULL));
    return rand() % (max - min + 1) + min;
}

/**
 * @brief Funció principal del SERVIDOR
 *
 * @param argc Número de paràmetres
 * @param argv Paràmetres
 * @param argv[1] Port on escolta el servidor
 */
int main(int argc, char **argv)
{
    if (argc == 2)
    {
        int s;                                          /* El socket */
        struct sockaddr_in socket_servidor;             /* Socket on escolta el servidor */
        struct sockaddr_in adr_client;                  /* Adreça i port des d'on el client envia el paquet */
        char paquet[TAMAÑO_PAQUETE]; /* Per posar les dades a enviar/rebre */

        /* Volem socket d'internet i no orientat a la connexio */
        s = get_socket();

        /* Posem les dades del socket del servidor */
        socket_servidor = get_address(NULL, atoi(argv[1]));

        /* Enllacem el socket */
        int resultat_bind = bind_socket(s, socket_servidor);
        if (resultat_bind < 0)
        {
            printf("No s'ha pogut enllacar el socket\n");
        }
        else
        {
            /* Servidor operatiu! */
            printf("Servidor operatiu al port %d!\n", atoi(argv[1]));

            int num_random, num_usuari;
            int min = 1;
            int max;
            int nivells[3] = {10, 50, 100};
            char nom[20];
            bool endevinat;
            int dificultat;

            while (1)
            {
                printf("Esperant petició d'algun client...\n");

                /* Rebem el nom del client */
                receive_data(s, paquet, &adr_client);
                strcpy(nom, paquet);

                printf("Petició rebuda!\n");
                printf("Enviant missatge de benvinguda...\n");

                /* Enviem el missatge de benvinguda */
                sprintf(paquet, "[SERVIDOR]: Benvingut %s! Selecciona la dificultat:\n\t\t1) del 1 al %d  \n\t\t2) del 1 al %d   \n\t\t3) del 1 al %d \n", nom, nivells[0], nivells[1], nivells[2]);
                send_data(s, paquet, adr_client);

                /* Rebem la dificultat escollida pel client*/
                receive_data(s, paquet, &adr_client);
                sscanf(paquet, "%d", &dificultat);
                
                switch (dificultat)
                {
                case 1:
                    max = nivells[0];
                    break;
                case 2:
                    max = nivells[1];
                    break;
                case 3:
                    max = nivells[2];
                    break;
                default:
                    max = nivells[2];
                    break;
                }

                // sprintf(paquet, "[SERVIDOR]: Benvingut %s! Has d'endevinar un número entre %d i %d.\nIntrodueix un número: ", nom, min, max);
                /* Generem un número aleatori */
                printf("Generant número aleatori...\n");
                num_random = num_aleatori(min, max);

                endevinat = false;
                while (!endevinat)
                {
                    /* Rebem el número del client */
                    printf("Esperant número del client...\n");
                    receive_data(s, paquet, &adr_client);
                    sscanf(paquet, "%d", &num_usuari);

                    /* Comprovem si s'ha endevinat */
                    printf("Comprovant si el client ha endevinat el número...\n");
                    if (num_usuari == num_random)
                    {
                        printf("El client ha endevinat el número!\n");
                        printf("FI DE LA PARTIDA!\n\n\n");
                        sprintf(paquet, "[SERVIDOR]: Felicitats %s! Has endevinat el número!\n", nom);
                        send_data(s, paquet, adr_client);
                        endevinat = true;
                    }
                    else
                    {
                        printf("El client no ha endevinat el número!\n");
                        if (num_random > num_usuari)
                        {
                            sprintf(paquet, "[SERVIDOR]: Ho sento %s! El número ha endevinar és més gran!\nTorna a provar: ", nom);
                            send_data(s, paquet, adr_client);
                        }
                        else
                        {
                            sprintf(paquet, "[SERVIDOR]: Ho sento %s! El número ha endevinar és més petit!\nTorna a provar: ", nom);
                            send_data(s, paquet, adr_client);
                        }
                    }
                }
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
