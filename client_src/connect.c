#include "../includes/client.h"

int 	make_connection(t_user *user, t_conn *conn)
{
	struct hostent *he;
	struct sockaddr_in s;

	if ((he = gethostbyname(conn->hostname)) == NULL)
		handle_error(1);
	s.sin_addr = *((struct in_addr *)he->h_addr);
	if ((conn->fd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		handle_error (2);
	s.sin_family = AF_INET;
	s.sin_port = htons(conn->port);
	ft_memset(s.sin_zero, '\0', sizeof(s.sin_zero));
	if (connect(conn->fd, (struct sockaddr *)&s, sizeof(s)) == -1)
		handle_error(2);
	ft_putendl("\033[0;32mConnected to server.....\033[0m");
	user->connected = 1;
	return (0);
}

void	wipe_user(t_user *user, t_conn *conn)
{
	if (user->logged_in == 1)
	{
		user->logged_in = 0;
		user->name = NULL;
	}
	user->channels = NULL;
	close(conn->fd);
}

void	handle_connect(char **array, t_user *user, t_conn *conn)
{
	if (arraylen(array) != 3)
	{
		ft_err("Incorrect args");
		return ;
	}
	else
	{
		if (user->connected == 1)
			wipe_user(user, conn);
		if (ft_atoi(array[2]) > 1999)
		{
			conn->port = ft_atoi(array[2]);
			conn->hostname = array[1];
			connect_(user, conn);
		}
		else
			ft_err("Invalid port number");
	}
}

void	connect_(t_user *user, t_conn *conn)
{
	conn->buffer->write = 0;
	conn->buffer->read = 0;
	conn->buffer->cmdlen = 50;
	make_connection(user, conn);
	user->connected = 1;
	while (1)
		read_input(user, conn);
}