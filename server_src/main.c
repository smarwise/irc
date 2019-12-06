#include "../includes/server.h"

char    *get_ip(struct sockaddr_in *sa)
{
        char *ip;

        ip = malloc(sizeof(char) * INET_ADDRSTRLEN);
        ip = inet_ntoa(sa->sin_addr);
        return ip;
}

int     main(int argc, char **argv)
{

        if (argc == 2)
        {
            init(argv[1]);
            return (0);
        }
        else
            return (handle_error(0));
}