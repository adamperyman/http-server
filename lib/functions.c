#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

void PrintHeading(void)
{
    fprintf(stdout, "**************************************\n");
    fprintf(stdout, "*                                    *\n");
    fprintf(stdout, "*        A simple HTTP-Server        *\n");
    fprintf(stdout, "*                                    *\n");
    fprintf(stdout, "* Author: Adam Peryman               *\n");
    fprintf(stdout, "* Contact: adam.peryman@gmail.com    *\n");
    fprintf(stdout, "**************************************\n");
}

unsigned long SendFile(int fd)
{
    // Get request.
    char data[512] = { '\0' };
    int request = recv(fd, data, 512, 0);
    data[request] = '\0';

    // Get filename from request.
    char fileName[256] = { '\0' };
    sscanf(data, "GET /%s ", fileName);

    fprintf(stdout, "%s\n", data);

    // Check filename.
    FILE* fp = fopen(fileName, "rb");
    if (!fp)
    {
        fprintf(stderr, "Error! Can't find %s.\n\n", fileName);
        fprintf(stdout, "> Waiting..\n\n");
        return 0;
    }

    // Get and send file.
    fprintf(stdout, "Sending %s.\n\n", fileName);
    int tempChar = 0;
    while ((tempChar = fgetc(fp)) != EOF)
    {
        send(fd, &tempChar, sizeof(tempChar), 0);
    }

    // We're done here.
    fprintf(stdout, "Finished sending %s!\n\n", fileName);
    fprintf(stdout, "> Waiting..\n\n");
    fclose(fp);
    close(fd);

    return 0;
}
