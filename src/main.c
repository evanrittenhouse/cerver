#include <netdb.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

const char *PORT = "5757";

const int CRV_OK = 1;
const int CRV_ERR = -1;

int crv_getaddrinfo(struct addrinfo *hints, struct addrinfo **res) {
   int get_addrinfo_status;
   
   // Fills `res` out with a linked-list of `addrinfo`s, which contains `sockaddr`s that we can use later
   get_addrinfo_status = getaddrinfo(NULL, PORT, hints, res);
   if (get_addrinfo_status == -1) {
      fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(get_addrinfo_status));
      return CRV_ERR;
   }

   return CRV_OK;
}

int main() {
   struct addrinfo hints;
   struct addrinfo *serv_info;

   // Ensure hints is 0'd
   memset(&hints, 0, sizeof hints); 
   // IPv4 or IPv6 works
   hints.ai_family = AF_UNSPEC;
   // TCP stream
   hints.ai_socktype = SOCK_STREAM;
   // Sets IP address of returned sockaddr to INADDR_ANY
   hints.ai_flags = AI_PASSIVE;

   printf("Hello, World!");

   if (crv_getaddrinfo(&hints, &serv_info) == CRV_ERR) {
      exit(1);
   }


   // getaddrinfo to find a socket
   // socket to build the socket
   // bind to bind
   // listen

   return 0;
}
