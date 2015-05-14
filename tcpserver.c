/* tcpserver.c */
/*LIBRERIAS DE TRABAJO*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


int main()
{
        int sock, connected, bytes_recieved , true = 1;
        char send_data [1024] , recv_data[1024];

        struct sockaddr_in server_addr,client_addr; // libreria <arpa/inet.h>
        int sin_size;

        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("Socket");
            exit(1);
        }

        if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int)) == -1) {
            perror("Setsockopt");
            exit(1);
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(5000); /*Verificación en el caso de no poseer puertos se usa el 5000
                                              ya que es universar y puede servir tanto como para tcp como
                                              para udp*/
        server_addr.sin_addr.s_addr = INADDR_ANY; /*Verificación para atender cualquier socket*/
        bzero(&(server_addr.sin_zero),8);

        if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))
                                                                       == -1) {
            perror("Unable to bind");
            exit(1);
        }

        if (listen(sock, 5) == -1) {
            perror("Escuchando");
            exit(1);
        }

	printf("\nTCPServer Esperando Cliente para el Puerto 5000");
        fflush(stdout);


        while(1)
        {

            sin_size = sizeof(struct sockaddr_in);

            connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size); /*A la vuelta de la función, esta estructura
                                                                                contendrá la dirección y demás datos del
                                                                                ordenador cliente que se ha conectado a
                                                                                nosotros  */
            printf("\n Tienes un cliente desde (%s , %d)",
                   inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

            while (1)
            {
              printf("\n Enviar (q o Q para salir) : ");
              //gets(send_data);
              fgets(send_data, sizeof(send_data), stdin);

              if (strcmp(send_data , "q") == 0 || strcmp(send_data , "Q") == 0)
              {
                send(connected, send_data,strlen(send_data), 0);
                printf("\n Conexion Finalizada");
                close(connected);
                break;
              }

              else{
                 send(connected, send_data,strlen(send_data), 0);

              bytes_recieved = recv(connected,recv_data,1024,0);

              recv_data[bytes_recieved] = '\0';

              if (strcmp(recv_data , "q") == 0 || strcmp(recv_data , "Q") == 0)
              {
                close(connected);
                break;
                printf("\n Conexion Finalizada");
              }
              else
              {
                  printf("\n Recibido = %s " , recv_data);
                  fflush(stdout);
              }
              }
            }
            close(connected);
        }

      close(connected);
      return 0;
}
