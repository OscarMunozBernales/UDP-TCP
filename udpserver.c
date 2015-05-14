/* udpserver.c */
/*LIBRERIAS DE TRABAJO*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main()
{
        int sock;
        int addr_len, bytes_read;
        char recv_data[1024];
        struct sockaddr_in server_addr , client_addr;


        if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) { /*Para indicar si los clientes pueden estar
                                                               en otros ordenadores distintos del servidor
                                                               o van a correr en el mismo ordenador*/
            perror("Socket");
            exit(1);
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(5000); /*Verificación en el caso de no poseer puertos se usa el 5000
                                              ya que es universar y puede servir tanto como para tcp como
                                              para udp*/
        server_addr.sin_addr.s_addr = INADDR_ANY; /*Verificación para atender cualquier socket*/
        bzero(&(server_addr.sin_zero),8);


        if (bind(sock,(struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) 
                                              /* Avisar al sistema operativo de que hemos abierto un
                                               socket y queremos que asocie nuestro programa a dicho
                                               socket */
        {
            perror("Bind");
            exit(1);
        }

        addr_len = sizeof(struct sockaddr);

	printf("\nUDPServer Esperando Cliente en el puerto 5000");
        fflush(stdout);

	while (1)
	{

          bytes_read = recvfrom(sock,recv_data,1024,0,
	                    (struct sockaddr *)&client_addr, &addr_len);


	  recv_data[bytes_read] = '\0';

          printf("\n(%s , %d) said : ",inet_ntoa(client_addr.sin_addr), // rescata la direccion del cliente
                                       ntohs(client_addr.sin_port)); // con el numero de puerto
          printf("%s", recv_data); // y se imprime el mensaje
	  fflush(stdout);

        }
        return 0;
}
