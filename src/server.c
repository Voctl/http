#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

int main(void) {
  int sockfd = socket(AF_INET, SOCK_STREAM , 0);
  if (sockfd < 0){
    perror("Socket error\n");
    exit(-1);
  }

  int opt = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  struct addrinfo hint, *res;
  memset(&hint, 0, sizeof(hint));
  hint.ai_family = AF_INET;
  hint.ai_socktype = SOCK_STREAM;
  hint.ai_flags = AI_PASSIVE;

  getaddrinfo(NULL, "8080", &hint, &res);
  int bindresult = bind(sockfd, res->ai_addr, res->ai_addrlen);
  freeaddrinfo(res);

  if (bindresult < 0) {
    perror("Bind error\n");
    exit(-1);
  }
  printf("Bind result is %d\n", bindresult);


  // listen probbly for incmin connections
  int resll = listen(sockfd, 20);
  if (resll < 0){
    perror("Socket error\n");
    exit(-1);
  }

  while (1) {
    struct sockaddr_storage their_addr;
    socklen_t addr_size = sizeof(their_addr);
    int new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
    if (new_fd < 0) continue;

    int max_len = 1000;
    char received_request[max_len];
    memset(received_request, 0 , max_len);

    int num_bytes = recv(new_fd, received_request, max_len - 1, 0);
    // puts(received_request);

    if (num_bytes > 0 && strncmp(received_request, "GET", 3) == 0){
      printf("Received  HTTP get request\n");

      // We respond to the HTTP GET request

      char *status_line = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 13\r\nConnection: close\r\n\rFck Omarchy!\n";
      send(new_fd, status_line, strlen(status_line), 0);
    }
    else {
      printf("Received non-GET request!\n");
    }

    close(new_fd);
  }

  close(sockfd);
  return 0;
}
