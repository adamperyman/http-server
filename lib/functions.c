#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "macros.h"

void _printHeading (void) {
  fprintf(stdout, "*************************************\n");
  fprintf(stdout, "*                                   *\n");
  fprintf(stdout, "*        A Simple Web Server        *\n");
  fprintf(stdout, "*                                   *\n");
  fprintf(stdout, "* Author: Adam Peryman              *\n");
  fprintf(stdout, "* Contact: adam.peryman@gmail.com   *\n");
  fprintf(stdout, "*************************************\n");
}

void _sendFile (int clientSocket) {
  char buffer[BUFF_SIZE] = { '\0' };
  bzero(buffer, BUFF_SIZE);

  int readStatus = read(clientSocket, buffer, BUFF_SIZE);
  if (readStatus < 0) {
    fprintf(stderr, "Error: Reading from socket.\n");
    return;
  }

  // Temporary GET handling.
  char fileName[BUFF_SIZE] = { '\0' };
  sscanf(buffer, "GET /%s ", fileName);

  // Handle file request.
  FILE* fp = fopen(fileName, "rb");
  if (!fp) {
    fprintf(stderr, "Error: File opening.\n");
    return;
  }

  fprintf(stdout, "Sending: %s\n", fileName);

  char fileChar = 0;
  int i = -1;
  bzero(buffer, BUFF_SIZE);
  while ((fileChar = fgetc(fp)) != EOF) {
    buffer[++i] = fileChar;
  }
  fclose(fp);

  // Temporary header handling.
  char header[BUFF_SIZE] = "HTTP/1.1 200 OK\r\n";
  char contentType[BUFF_SIZE] = "Content-Type: text/html; charset=UTF-8\r\n";
  char contentLength[BUFF_SIZE] = { '\0' };
  char connection[BUFF_SIZE] = "Connection: Keep-Alive\r\n";
  char server[BUFF_SIZE] = "Server: AP Server\r\n";
  char status[BUFF_SIZE] = "Status: 200\r\n";
  sprintf(contentLength, "Content-Length: %d\r\n", strlen(buffer) + 1);

  char response[BUFF_SIZE] = { '\0' };
  strcat(response, header);
  strcat(response, contentType);
  strcat(response, contentLength);
  strcat(response, connection);
  strcat(response, server);
  strcat(response, status);
  // strcat(response, buffer);
  strcat(response, "\r\n\r\n");

  printf("\n%s\n", response);
  printf("\nlen: %d\n", strlen(buffer));

  if (write(clientSocket, response, strlen(response)) < 0) {
    fprintf(stderr, "Error: Sending header.");
    return;
  };

  if (write(clientSocket, buffer, strlen(buffer) + 1) < 0) {
    fprintf(stderr, "Error: Sending pt. 2.");
    return;
  }

  fprintf(stdout, "Sent: %s\n\n", fileName);
  fprintf(stdout, "Waiting..\n\n");
}
