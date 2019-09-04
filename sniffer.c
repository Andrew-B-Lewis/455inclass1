#include "common.h"


unsigned int len_ethhdr=0, len_iphdr=0, len_udphdr=0;
int num_of_packets=1;

FILE *log_file;
#define LOG_FILE_NAME "pkt_log.txt"

int main( int argc, char * argv[]) 
{
	
	int socket_fd, i;
	struct sockaddr_in client_addr;
	struct sockaddr_in server_addr;
	struct ifreq interface;
	
	char pkt[PKT_SIZE];
	int recv_bytes=0, client_len, ret;

	struct ethhdr *eth_hdr;
	struct iphdr *ip_hdr;
	struct udphdr *udp_hdr;

	struct sockaddr_in source,dest;	


       

	printf("device_name = %s\n", SERVER_INTERFACE_NAME);
	if ((socket_fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) 
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



     log_file = fopen(LOG_FILE_NAME, "w");
     if (log_file == NULL)
     {
         printf("Could not create file %s, error =%d\n", LOG_FILE_NAME, errno);
		 perror("Error ");
     }
printf("Starting to receive...\n");
        
        len_ethhdr = sizeof(struct ethhdr);
        len_iphdr = sizeof(struct iphdr);
        len_udphdr = sizeof(struct udphdr);
    
        printf("size of ether_header = %d, iphdr = %d, udphdr = %d\n", len_ethhdr, len_iphdr, len_udphdr);

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

		eth_hdr = (struct ethhdr *)pkt;
		ip_hdr = (struct iphdr *) (pkt + len_ethhdr); // Size of ethernet header
		udp_hdr = (struct udphdr *) (pkt + len_ethhdr + len_iphdr); // Size of IP header 
		
		fprintf(log_file, "\nPkt #%d, size in bytes = %d:\n", num_of_packets, recv_bytes);
		
		fprintf(log_file, "Source MAC=  %02x:%02x:%02x:%02x:%02x:%02x \n", 
							eth_hdr->h_source[0], eth_hdr->h_source[1],
							eth_hdr->h_source[2], eth_hdr->h_source[3],
							eth_hdr->h_source[4], eth_hdr->h_source[5]);	
		
					
		fprintf(log_file, "Dest MAC= %02x:%02x:%02x:%02x:%02x:%02x \n",
							eth_hdr->h_dest[0], eth_hdr->h_dest[1],
							eth_hdr->h_dest[2], eth_hdr->h_dest[3],
							eth_hdr->h_dest[4], eth_hdr->h_dest[5]);

		memset(&source, 0, sizeof(source));
		source.sin_addr.s_addr = ip_hdr->saddr;
		memset(&dest, 0, sizeof(dest));
		dest.sin_addr.s_addr = ip_hdr->daddr;

		fprintf(log_file, "Source IP= %s\n" , inet_ntoa(source.sin_addr) );
        fprintf(log_file, "Destination IP= %s\n" , inet_ntoa(dest.sin_addr) );
		fprintf(log_file, "IP protocol = %d\n", (unsigned int) ip_hdr->protocol);

		fprintf(log_file, "UDP source port = %d\n", udp_hdr->source);
		fprintf(log_file, "UDP destination port = %d\n", udp_hdr->dest);

		fprintf(log_file, "Packet data: ");
		
	        int offset = len_ethhdr+len_iphdr+len_udphdr;
		for (i=0; i< recv_bytes; i++)
                {
                	fprintf(log_file, "%c", pkt[offset+i]);
		}

		num_of_packets++;
		
	}
	
	close(socket_fd);
	
  	return EXIT_SUCCESS;
}
