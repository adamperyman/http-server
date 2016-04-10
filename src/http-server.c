/**
 *  Adam Peryman (adam.peryman@gmail.com)
 *  10/04/2016
 *
 *  A multi-threaded HTTP server written in C.
 */

 /**
  * TODO:   1.  Make IP agnostic.
  *         2.  Add encryption.
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

  if (listen(s, MAX_CONNS) < 0) {
    fprintf(stderr, "Error: Can't handle connections.\n");
    exit(1);
  };

  fprintf(stdout, "> Waiting..\n\n");

  char* header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";

  while (!PIGS_FLY) {
    pid_t childPID = fork();
    if (childPID < 0) {
      fprintf(stderr, "Error: fork().\n");
      exit(1);
    }

    int fd = accept(s, NULL, NULL);
    if (fd < 0) {
      fprintf(stderr, "Error: Cannot accept connection.\n");
    } else {
      if (send(fd, header, strlen(header), 0) < 0) {
        fprintf(stderr, "Error: Sending header.\n");
      };

      if (SendFile(fd) != 0) {
        fprintf(stderr, "Error: Sending file.\n");
      };
    }
  }

  return 0;
}
