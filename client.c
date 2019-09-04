#include "common.h"

int main( int argc, char * argv[]) 
{
	int index;
	int num_of_packets;
	int socket_fd;
	struct sockaddr_in client_addr;
	struct sockaddr_in server_addr;
	struct ifreq interface;
    char pkt[PKT_SIZE];
	int ret, send_bytes;


	if((argc != 2) || (!strncmp(argv[1], "help", strlen("help"))))
	{
		printf("usage: ./client number_of_packets\n");
		printf("Exiting..\n");
		return EXIT_FAILURE;
	}

	num_of_packets = atoi(argv[1]);


	printf("device_name: %s\n num_of_packets: %d\n", CLIENT_INTERFACE_NAME, num_of_packets);
			
				
	if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
	{
		printf("Could not create socket...error %d, exiting\n", errno);
		perror("Error ");
        return EXIT_FAILURE;
	}


        snprintf(interface.ifr_name, sizeof(interface.ifr_name), "%s", CLIENT_INTERFACE_NAME);
	if((ret = setsockopt(socket_fd, SOL_SOCKET, (SO_REUSEPORT | SO_REUSEADDR), (char *)&interface, sizeof(interface))) < 0) 
	{
		printf("Could not set option %s...exiting\n", "SO_BINDTODEVICE");

		return EXIT_FAILURE;
	}

	/*
   	* build the server's Internet address
   	*/
  	bzero((char *) &server_addr, sizeof(server_addr));
  	server_addr.sin_family = AF_INET;
  	server_addr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDR); //htonl(INADDR_ANY);
  	server_addr.sin_port = htons((unsigned short) SERVER_PORT);


	/*
   	* build the client's Internet address
   	*/
  	bzero((char *) &client_addr, sizeof(client_addr));
  	client_addr.sin_family = AF_INET;
  	client_addr.sin_addr.s_addr = inet_addr(CLIENT_IP_ADDR); //htonl(INADDR_ANY);
  	//client_addr.sin_port = htons((unsigned short) SERVER_PORT);


	//printf("src ip: %s\n", inet_ntoa(client_addr.sin_addr));
	if(bind(socket_fd, (struct sockaddr *) &client_addr, sizeof(client_addr)) < 0)
	{
		printf("Could not bind socket...error %d, exiting\n", errno);
		perror("Error ");
        return EXIT_FAILURE;

	}


	printf("Starting to transmit...\n");
	for(index = 0; index < num_of_packets; index++)
	{
		memset(pkt, 0, PKT_SIZE);
        snprintf(pkt, PKT_SIZE, "Hello world!!\n");
        send_bytes = sendto(socket_fd, pkt, PKT_SIZE, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
		if ( send_bytes  < 0) 
                {
                    printf("Client: send fails with error %d\n", errno);
                    perror("Error ");
                }
                else 
                {
                    printf("Packet of size %d sent, send_bytes %d: %s\n", (int) strlen(pkt), send_bytes, pkt);
                }
	}
	
	close(socket_fd);
	
  	return EXIT_SUCCESS;
}
