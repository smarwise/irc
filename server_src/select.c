#include "../includes/server.h"

int FD_IS_ANY_SET(fd_set const *fdset)
{
    static fd_set empty;
    return (ft_memcmp(fdset, &empty, sizeof(fd_set)) != 0);
}

void	do_select(int fdmax, fd_set *read_fds, fd_set *write_fds)
{
    typedef struct temp {
        struct timeval timeout;
    }       t_temp;

    t_temp *temp;

    temp = (t_temp *)malloc(sizeof(t_temp));
    if ((!FD_IS_ANY_SET(read_fds)) && (!(FD_IS_ANY_SET(write_fds))))
    {
        temp->timeout.tv_sec = 0;
        temp->timeout.tv_usec = 100000;
    }
    else
    {
        temp = NULL;
    }
	if (select(fdmax + 1, read_fds, write_fds, NULL, &temp->timeout) == -1)
    {
        ft_err("select");
        exit(0);  
    }
}