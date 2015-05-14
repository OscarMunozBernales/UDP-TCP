/* tcpclient.c */

#include <sys/socket.h> //Libreria de Linux para win se usa winsock2.h
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>


int main()
{
        int sock, bytes_recieved;
        char send_data[1024],recv_data[1024];
        struct hostent *host;
        struct sockaddr_in server_addr; // libreria <netinet/in.h>

        host = gethostbyname("127.0.0.1"); // libreria <netinet/in.h>, sirve para dar la ip al pc con el
                                           // cual nos queremos comunicar

        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) { /*Para indicar si los clientes pueden estar
                                                               en otros ordenadores distintos del servidor
                                                               o van a correr en el mismo ordenador*/
            perror("Socket");                                 
            exit(1);                                          
        }
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(5000);
        server_addr.sin_addr = *((struct in_addr *)host->h_addr);
        bzero(&(server_addr.sin_zero),8);
        if (connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1){
            perror("Connect");
            exit(1);
        }

        while(1)
        {
          bytes_recieved=recv(sock,recv_data,1024,0);
          recv_data[bytes_recieved] = '\0';
          if (strcmp(recv_data , "q") == 0 || strcmp(recv_data , "Q") == 0){
           close(sock);
           break;
          }
          else
            printf("\nMensaje Recibido = %s " , recv_data);
          printf("\nMensaje Enviado (q o Q para salir) : ");
          fgets(send_data, sizeof(send_data), stdin);

          if (strcmp(send_data , "q") != 0 && strcmp(send_data , "Q") != 0)
           send(sock,send_data,strlen(send_data), 0);
          else{
           send(sock,send_data,strlen(send_data), 0);
           close(sock);
           break;
          }
        }
        close(sock);
return 0;
}
