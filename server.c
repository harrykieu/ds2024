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

char dirCont[MAX_INPUT][MAX_INPUT];

void writeFile(int fd, char *fileName)
{
    int file_fd;
    char buf[1024];
    file_fd = open(fileName, O_RDONLY, 0666);
    int byte_read;
    while (byte_read = read(file_fd, buf, 1024))
    {
        byte_read = write(fd, buf, byte_read);
        if (byte_read < 0)
        {
            printf("Error writing!");
            exit(EXIT_FAILURE);
        }
    }
}

void queryDir(int clientfd)
{
    DIR *dir = opendir("/home/hhk/Desktop/tcpfiletransfer");
    struct dirent *dir_entry;
    int i = 0;
    FILE *clientstream = fdopen(clientfd, "w");
    while ((dir_entry = readdir(dir)) != NULL)
    {
        if (dir_entry->d_type == DT_REG)
        {
            strcpy(dirCont[i], dir_entry->d_name);
            fprintf(clientstream, "%d: %s\n", i, dir_entry->d_name);
            fflush(clientstream);
            printf("%d: %s\n", i, dir_entry->d_name);
            i++;
        }
    }
    fclose(clientstream);
    closedir(dir);
}

int main(int argc, char argv[])
{
    int opt = 1;
    int servfd = socket(AF_INET, SOCK_STREAM, 0);
    if (setsockopt(servfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        printf("Error setting socket option \n");
        return EXIT_FAILURE;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = INADDR_ANY;

    socklen_t addrlen = sizeof(addr);
    if (bind(servfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        printf("Error binding socket\n");
        return EXIT_FAILURE;
    }

    if (listen(servfd, 3) < 0)
    {
        printf("Socket cannot listen\n");
        return EXIT_FAILURE;
    };

    int clientfd = accept(servfd, (struct sockaddr *)&addr, (socklen_t *)&addrlen);
    if (clientfd < 0)
    {
        printf("Error accepting socket\n");
        return EXIT_FAILURE;
    }
    queryDir(clientfd);
    // bug
    char buf[1024];
    read(clientfd, buf, 1024);
    printf("%s", buf);
    // must redirect to the socket fd
    // writeFile(clientfd, dirCont[choice]);
    close(clientfd);
    close(servfd);
    return EXIT_SUCCESS;
}