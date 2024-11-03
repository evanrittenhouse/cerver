#include <netinet/in.h>

const char *PORT = "5757";
const int BACKLOG_CONNECTIONS = 10;

const int CRV_OK = 1;
const int CRV_ERR = -1;

/// Get the sin_addr or sin6_addr of an incoming connection. 
void *get_in_addr(struct sockaddr *sa);

/// Get a file descriptor for a listening socket.
int crv_get_socket(struct addrinfo hints, struct addrinfo *res,
                   int *socket_fd);
