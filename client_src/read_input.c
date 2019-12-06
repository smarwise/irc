#include "../includes/client.h"

char    *get_ip(struct sockaddr_in *sa)
{
    char *ip;

    ip = malloc(sizeof(char) * INET_ADDRSTRLEN);
    ip = inet_ntoa(sa->sin_addr);
    return ip;
}

void		process_args(char *str, t_user *user, t_conn *conn)
{
	char	**array;

	array = ft_strsplit(str, ' ');
	if (ft_strcmp(array[0], "/connect") == 0)
		handle_connect(array, user, conn);
	else if (ft_strcmp(array[0], "/nick") == 0)
		login(str, user, conn);
	else if (ft_strcmp(array[0], "/join") == 0)
		join(str, user, conn, array);
	else if (ft_strcmp(array[0], "/leave") == 0)
		leave(str, user, conn, array);
	else if (ft_strcmp(array[0], "/who") == 0)
		who(user, conn, array);
	else if (ft_strcmp(array[0], "/quit") == 0)
		quit();
	else if (ft_strcmp(array[0], "/msg") == 0)
		msg(user, conn, str);
	else if (ft_strcmp(array[0], "/msg_channel") == 0)
		msg_channel(str, conn, user);
	else
		ft_putendl("\033[0;31mCommand not recognized\033[0m");
	free_2d_array((void **)array);
	array = NULL;
}

char		*read_cmd(char *cmd, int *buff, int n, int i)
{
	char c;

	while (n < *buff)
	{
		if (read(STDIN_FILENO, &c, 1) > 0) 
		{
			if (c != '\n' && c != EOF)
				cmd[i] = c;
			else
				break;
		}
		else
			break;
		if (n + 1 == *buff && c != '\n' && c != EOF)
		{
			*buff += BUFFSIZE;
			cmd = realloc(cmd, *buff);
		}
		i++;
		n++;
	}
	cmd[i] = '\0';
	return(cmd);
}

char		*read_support(int buff, int *read, int i, t_conn *conn)
{
	char *cmd;
	t_fd *fds;
	int fdmax;

	fds = (t_fd*)malloc(sizeof(t_fd));
	FD_ZERO(&fds->read_fds);
	FD_ZERO(&fds->write_fds);
	fdmax = conn->fd > 0 ? conn->fd : STDIN_FILENO; 
    FD_SET(STDIN_FILENO, &fds->read_fds);
	if (conn->fd > 0)
		FD_SET(conn->fd, &fds->read_fds);
	do_select(fdmax+1, &fds->read_fds, &fds->write_fds);
	cmd = NULL;
	if (FD_ISSET(STDIN_FILENO, &fds->read_fds))
	{
		cmd = (char *)malloc(sizeof(char) * BUFFSIZE);
		cmd = read_cmd(cmd, &buff, 0, i);		
	}
	if (conn->fd > 0)
	{
		if (FD_ISSET(conn->fd, &fds->read_fds))
			*read = 1;
	}
	return (cmd);
}

int		read_input(t_user *user, t_conn *conn)
{
	char *cmd;
	int read;
 
	read = 0;
	if (user->logged_in == 1)
	{
			ft_putstr("\033[1;34mirc[");
			ft_putstr(user->name);
			ft_putstr("]> \033[0m");
	}
	else
		ft_putstr("\033[1;34mirc> \033[0m");
	cmd = read_support(BUFFSIZE, &read, 0, conn);
	if (cmd != NULL)
		process_args(cmd, user, conn);
	if (read == 1)
		receive_msg(conn, conn->buffer);
	free(cmd);
	return (0);
}