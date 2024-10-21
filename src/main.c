 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <errno.h>
 #include <string.h>
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include <netdb.h>
 #include <arpa/inet.h>
 #include <sys/wait.h>
 #include <signal.h>

const char *PORT = "5757";

const int CRV_OK = 1;
const int CRV_ERR = -1;

int crv_get_socket(struct addrinfo hints, struct addrinfo *res, int *socket_fd) {
   struct addrinfo *p;
   int get_addrinfo_status, socket_res;
   
   // Fills `res` with a linked-list of `addrinfo`s, which contains `sockaddr`s that we can use later
   get_addrinfo_status = getaddrinfo(NULL, PORT, &hints, &res);
   if (get_addrinfo_status == -1) {
      fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(get_addrinfo_status));
      return CRV_ERR;
   }

   // Loop through all returned `addrinfo`s and bind to the first one we can 
   for (p = res; p != NULL; p = p->ai_next) {
      socket_res = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

      if (socket_res == -1) {
         socket_res = errno;
         fprintf(stderr, "socket error: %d\n", socket_res);
         continue;
      }

      // Associate a socket with an IP address and port nmber
      if (bind(socket_res, p->ai_addr, p->ai_addrlen) == -1) {
         close(socket_res);
         fprintf(stderr, "bind error");
         continue;
      }

      break;
   }

   *socket_fd = socket_res;

   freeaddrinfo(res);

   return CRV_OK;
}

int main() {
   struct addrinfo hints;
   struct addrinfo *serv_info;
   int socket_fd, socket_res;

   // Ensure hints is 0'd
   memset(&hints, 0, sizeof hints); 
   // IPv4 or IPv6 works
   hints.ai_family = AF_UNSPEC;
   // TCP stream
   hints.ai_socktype = SOCK_STREAM;
   // Sets IP address of returned sockaddr to INADDR_ANY
   hints.ai_flags = AI_PASSIVE;

   socket_res = crv_get_socket(hints, serv_info, &socket_fd);
   if (socket_res == -1) {
      exit(1);
   }

   if (listen(socket_fd, 10) == -1) {
      printf("listen");
      exit(1);
   }

   printf("socket: %d", socket_fd);


   // getaddrinfo to find a socket
   // socket to build the socket
   // bind to bind
   // listen

   return 0;
}
