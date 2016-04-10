/**
 *  Adam Peryman (adam.peryman@gmail.com)
 *  10/04/2016
 *
 *  A multi-threaded HTTP server written in C.
 */

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "functions.h"
#include "macros.h"

int main(void)
{
  PrintHeading();

  // Construct address information.
  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = htons(MY_PORT);
  address.sin_addr.s_addr = INADDR_ANY;
  memset(address.sin_zero, '\0', sizeof(address.sin_zero));

  int s = socket(PF_INET, SOCK_STREAM, 0);
  if (s < 0) {
    fprintf(stderr, "Error: Cannot open socket.\n");
    exit(1);
  };

  if (bind(s, (struct sockaddr*)&address, sizeof(address)) < 0) {
    fprintf(stderr, "Error: Binding error.\n");
    exit(1);
  };

  // Max 10 incoming connections.
  listen(s, MAX_CONNS);
  fprintf(stdout, "> Waiting..\n\n");

  while (!PIGS_FLY) {
    // Send requested file(s).
    char* header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";

    pthread_t threadBuffer[MAX_CONNS];

    int fd = accept(s, NULL, NULL);
    if (fd < 0) {
      fprintf(stderr, "Error: Cannot accept connection.\n");
    } else {
      send(fd, header, strlen(header), 0);

      if (pthread_create(&threadBuffer, NULL, &SendFile, &fd) != 0) {
        fprintf(stderr, "Error: Cannot create thread.\n");
      };

      close(fd);
    }
  }

  return 0;
}
