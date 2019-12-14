#include "../includes/client.h"

void 	send_cmd(char *cmd, t_conn *conn)
{
	t_fd *fds;

	fds = (t_fd*)malloc(sizeof(t_fd));
	FD_ZERO(&fds->read_fds);
	FD_ZERO(&fds->write_fds);
	FD_SET(conn->fd, &fds->write_fds);
	do_select(conn->fd+1, &fds->read_fds, &fds->write_fds);
	if (FD_ISSET(conn->fd, &fds->write_fds))
		send(conn->fd, cmd, ft_strlen(cmd), 0);
}

int     recv_cmd(char buf[50], int fd)
{
	t_fd *fds;
    int nbytes;

    ft_memset(buf, '\0', 50);
	fds = (t_fd*)malloc(sizeof(t_fd));
	FD_ZERO(&fds->read_fds);
	FD_ZERO(&fds->write_fds);
	FD_SET(fd, &fds->read_fds);
	do_select(fd+1, &fds->read_fds, &fds->write_fds);
    nbytes = 0;
	if (FD_ISSET(fd, &fds->read_fds))
    {
        if ((nbytes = (recv(fd, buf, 49, 0))) <= 0)
        {
            if (nbytes == 0)
            {
                ft_err("Connection got cut unexpectedly");
                exit(0);
            }
            else
                ft_err("Result receival failed");
        }
    }
    return (nbytes);
}