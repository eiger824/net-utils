#define _DEFAULT_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <libgen.h>
#include <getopt.h>
#include <stdarg.h>

void err(const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    vprintf(msg, args);
    va_end(args);
}

int hostname_to_ip(char * hostname , char* ip)
{
    struct hostent *he;
    struct in_addr **addr_list;
    int i;

    if ( (he = gethostbyname( hostname ) ) == NULL) 
    {
        // get the host info
        herror("gethostbyname");
        return 1;
    }

    addr_list = (struct in_addr **) he->h_addr_list;

    for (i = 0; addr_list[i] != NULL; i++) 
    {
        //Return the first one;
        strcpy(ip , inet_ntoa(*addr_list[i]) );
        return 0;
    }

    return 1;
}

void usage(char* program)
{
    printf(
            "Usage: %s [OPTIONS] <hostname> [<hostname> ...]\n"
            "[OPTIONS]\n"
            " -h        Print this help and exit\n",
            basename(program)
          );
}

int main(int argc, char* argv[])
{
    char hostname[1024];
    char ip[16];
    int c, ret, index;
    char* line = NULL;
    size_t len = 0;
    ssize_t nread;

    while ((c = getopt(argc, argv, "h")) != EOF)
    {
        switch (c)
        {
            case 'h':
                usage(argv[0]);
                exit(0);
            default:
                usage(argv[0]);
                exit(1);
        }
    }

    printf("%-20s    %s\n", "Hostname", "IPv4 address");
    printf("%-20s    %s\n", "********", "************");
    if (argc - optind < 1)
    {
        while (( nread = getline(&line, &len, stdin)) != EOF)
        {
            line[nread - 1] = '\0';
            printf("%-20s => ", line);
            fflush(stdout);
            ret = hostname_to_ip(line, ip);
            if (ret != 0)
                continue;

            printf("%s\n", ip);
        }
        free(line);
    }
    else
    {
        for (index = optind; index < argc; ++index)
        {
            memset(hostname, 0, sizeof(hostname));
            strcpy(hostname, argv[index]);

            printf("%-20s => ", hostname);
            fflush(stdout);

            ret = hostname_to_ip(hostname, ip);
            if (ret != 0)
                continue;

            printf("%s\n", ip);
        }
    }
    return 0;
}

