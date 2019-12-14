#include "../includes/server.h"

void 	send_cmd(int fd, char *cmd)
{
	int	len;
	char *size;
	char *real_cmd;

	len = ft_strlen(cmd);
	size = ft_itoa(len);
	real_cmd = ft_strjoin(size, cmd);
	if (send(fd, real_cmd, ft_strlen(real_cmd), 0) <= 0)
		handle_error(6);
}

t_client    *check_if_there(t_client *client, char *name)
{
	while (client)
	{
		if (ft_strcmp(client->name, name) == 0)
			return (client);
		client = client->next;
	}
	return(NULL);
}

char		*get_sender_name(t_client *client, int fd)
{
	while (client)
	{
		if (client->fd == fd)
			return (ft_strdup(client->name));
		client = client->next;
	}
	return(NULL);
}

char	*make_cmd(char *sender_name, char *msg)
{
	char *cmd;
	char *tmp;

	tmp = ft_strjoin(msg, " ");
	cmd = ft_strjoin(tmp, sender_name);
	free(tmp);
	return (cmd);
}

int		check_msg(t_select *select, char **array, t_client *client, int fd)
{
	(void)select;
	if (arraylen(array) < 3)
        send(fd, "Too few arguments to command call\n", 35, 0);
    else if (!client->name)
        send(fd, "You must be logged in to send messages\n", 40, 0);
    else if (ft_strlen(array[1]) > 9)
		send(fd, "Nickname cannot be more than 9 characters\n", 43, 0);
	else
	{
		FD_ZERO(&select->write_fds);
		FD_ZERO(&select->read_fds);	
		FD_SET(fd, &select->write_fds);
		return (1);
	}
	return (-1);
}

int		msg(t_client *client, char *cmd, int fd, t_conn *conn)
{
	int i;
	char **array;
	char	*name;
	t_select	*select;
	t_client *receiver;

	array = ft_strsplit(cmd, ' ');
	client = get_client(conn, fd);
	i = 0;
	select = (t_select*)malloc(sizeof(t_select));
	if (check_msg(select, array, client, fd) > 0)
	{
		if ((receiver = check_if_there(conn->head, array[1])) != NULL)
		{
			name = get_sender_name(conn->head, fd);
			if (name != NULL)
			{
				do_select(fd+1, &select->read_fds, &select->write_fds);
				if (FD_ISSET(fd, &select->write_fds))
				{
					if (receiver->fd == fd)
					{
						ft_err("You can't send a message to yourself\n");
						send_result(-1, fd);
						return (-1);
					}
					send_cmd(receiver->fd, make_cmd(name, cmd));
					send_result(1, fd);
					i = 1;
				}
			}
		}
		if (i == 0)
			send_result(-1, fd);
		free_2d_array((void**)array);
		return (1);
	}
	return (-1);
}