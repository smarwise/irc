#include "../includes/server.h"

void 	send_info(char *cmd, int fd)
{
	t_fd *fds;

	fds = (t_fd*)malloc(sizeof(t_fd));
	FD_ZERO(&fds->read_fds);
	FD_ZERO(&fds->write_fds);
	FD_SET(fd, &fds->write_fds);
	do_select(fd+1, &fds->read_fds, &fds->write_fds);
	if (FD_ISSET(fd, &fds->write_fds))
    {
        if (send(fd, cmd, ft_strlen(cmd), 0) < 0)
            handle_error(6);
    }
}