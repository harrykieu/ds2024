#define _DEFAULT_SOURCE

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>

void readFromStream(int fd)
{
    FILE *stream = fdopen(fd, "r");
    char buf[1024];
    size_t bytesRead;
    while ((bytesRead = fread(buf, 1, 1024, stream)) > 0)
    {
        printf("%s", buf);
    }
    if (bytesRead < 0)
    {
        perror("Error reading from stream");
    }
    if (fclose(stream) != 0)
    {
        perror("Error closing stream");
    }
}

void writeToStream(int fd, char *str)
{
    FILE *stream = fdopen(fd, "w");
    size_t bytesWritten = fwrite(str, 1, sizeof(str), stream);
    if (bytesWritten < 0)
    {
        perror("Error writing");
        exit(EXIT_FAILURE);
    }
    if (fclose(stream) != 0)
    {
        perror("Error closing stream");
    }
}

void handle_file(int fd)
{
    int file_fd;
    char buf[1024];
    file_fd = open("test2.txt", O_WRONLY, 0666);
    int byte_read;
    while (byte_read = read(fd, buf, 1024))
    {
        byte_read = write(file_fd, buf, byte_read);
        if (byte_read < 0)
        {
            perror("Error writing");
            exit(EXIT_FAILURE);
        }
    }
    close(file_fd);
    close(fd);
}

char buf[1024];
int main(int argc, char argv[])
{
    int opt = 1;
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in sv_addr;
    sv_addr.sin_family = AF_INET;
    sv_addr.sin_port = htons(8888);
    inet_pton(AF_INET, "127.0.0.1", &sv_addr.sin_addr);

    if (connect(clientfd, (struct sockaddr *)&sv_addr, sizeof(sv_addr)) < 0)
    {
        perror("Error connect to socket");
        return EXIT_FAILURE;
    }
    readFromStream(clientfd);
    char *choice;
    scanf("%s", choice);
    writeToStream(clientfd, choice);
    close(clientfd);
    return EXIT_SUCCESS;
}