#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdint.h>

void PrintHeading(void) {
  fprintf(stdout, "**************************************\n");
  fprintf(stdout, "*                                    *\n");
  fprintf(stdout, "*        A simple HTTP Server        *\n");
  fprintf(stdout, "*                                    *\n");
  fprintf(stdout, "* Author: Adam Peryman               *\n");
  fprintf(stdout, "* Contact: adam.peryman@gmail.com    *\n");
  fprintf(stdout, "**************************************\n");
}

void* SendFile(void* fd) {
  // Get request.
  char data[512] = { '\0' };

  // -1 byte for null terminator.
  int request = recv((uintptr_t) fd, &data, 511, 0);
  if (request < 0) {
    fprintf(stderr, "Error: reading from socket.\n");
    exit(1);
  }

  // Get filename from request.
  char fileName[256] = { '\0' };
  sscanf(data, "GET /%s ", fileName);

  // debug
  fprintf(stdout, "data: %s\n", data);

  // Check filename.
  FILE* fp = fopen(fileName, "rb");
  if (!fp) {
    fprintf(stderr, "Error! Can't find %s.\n\n", fileName);
    fprintf(stdout, "> Waiting..\n\n");
    exit(1);
  }

  // Get and send file.
  fprintf(stdout, "Sending %s.\n\n", fileName);
  int tempChar = 0;
  while ((tempChar = fgetc(fp)) != EOF) {
    send((uintptr_t) fd, &tempChar, sizeof(tempChar), 0);
  }

  // We're done here.
  fprintf(stdout, "Finished sending %s!\n\n", fileName);
  fprintf(stdout, "> Waiting..\n\n");
  fclose(fp);

  return 0;
}
