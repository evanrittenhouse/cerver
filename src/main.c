#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include "util.h"

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
   struct addrinfo hints, *serv_info;
   int socket_fd, socket_res, conn_fd;
   struct sockaddr_storage client_addr;
   socklen_t addr_size;
   char s[INET6_ADDRSTRLEN];

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

   if (listen(socket_fd, BACKLOG_CONNECTIONS) == -1) {
      printf("listen");
      exit(1);
   }

   printf("server waiting for connections...\n");

   addr_size = sizeof client_addr;

   while (1) {
      conn_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &addr_size);
      if (conn_fd == -1) {
         printf("accept() error");
         continue;
      }

      inet_ntop(client_addr.ss_family,
            get_in_addr((struct sockaddr*) &client_addr),
            s, sizeof s
      );

      printf("server got connection from: %s\n", s);

      if (!fork()) {
         close(socket_fd);

         if (send(conn_fd, "hello world", 11, 0) == -1) {
            printf("send() error");
         }

         close(conn_fd);
         exit(0);
      }

      // Main server loop doesn't need this
      close(conn_fd);
   }

   return 0;
}

