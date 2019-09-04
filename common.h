#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/if_ether.h>
#include <errno.h>
#include <sys/time.h>

/*
* Uncomment USE_RAW_SOCKET for packet sniffer application
*/
#define USE_RAW_SOCKET   (1)

#define SERVER_PORT 3000

#define CLIENT_IP_ADDR  "192.168.122.1"  // Ethernet eth0
#define SERVER_IP_ADDR  "127.0.0.1"       // Loopback lo
#define CLIENT_INTERFACE_NAME "eth0"
#define SERVER_INTERFACE_NAME "lo"


#define PKT_SIZE  1024 //2048

/*
*
* Run to see tcpdump output for server: example here is on my VM
* tcpdump -i lo udp port 3000 -vv -X
* Do with RAW socket to see all headers
*
*/



