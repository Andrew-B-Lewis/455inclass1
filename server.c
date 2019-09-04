#include "common.h"


int num_of_packets=1;

int main( int argc, char * argv[]) 
{
	
	int socket_fd;
	struct sockaddr_in client_addr;
	struct sockaddr_in server_addr;
	struct ifreq interface;
	
	char pkt[PKT_SIZE];
	int recv_bytes=0, client_len, ret;

       
	printf("device_name = %s\n", SERVER_INTERFACE_NAME);
	if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
	{
		printf("Could not create socket...error %d, exiting\n", errno);
		perror("Error ");
		return EXIT_FAILURE;
	}

	snprintf(interface.ifr_name, sizeof(interface.ifr_name), "%s", SERVER_INTERFACE_NAME);
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
	if(bind(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
	{
		printf("Could not bind socket...error %d, exiting\n", errno);
		perror("Error ");
        return EXIT_FAILURE;
	}	
printf("Starting to receive...\n");
        

        client_len = sizeof(client_addr);
	while(1)
	{

		recv_bytes = recvfrom(socket_fd, pkt, PKT_SIZE, 0, (struct sockaddr  *) &client_addr, (socklen_t *) &client_len);

		if (recv_bytes < 0)
		{
			printf("Error no = %d, recv_bytes = %d \n", errno, recv_bytes);
			perror("Error ");
			continue;
		}

		printf("Packet #%d received as is:\n%s\n", num_of_packets, pkt);


		num_of_packets++;
		
	}
	
	close(socket_fd);
	
  	return EXIT_SUCCESS;
}
