#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

void PrintHeading(void) {
  fprintf(stdout, "**************************************\n");
  fprintf(stdout, "*                                    *\n");
  fprintf(stdout, "*        A simple HTTP Server        *\n");
  fprintf(stdout, "*                                    *\n");
  fprintf(stdout, "* Author: Adam Peryman               *\n");
  fprintf(stdout, "* Contact: adam.peryman@gmail.com    *\n");
  fprintf(stdout, "**************************************\n");
}
