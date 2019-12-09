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

void	msg(t_client *client, char *cmd, int fd, t_conn *conn)
{
	int i;
	char **array;
	char	*name;
	t_select	*select;
	t_client *receiver;

	array = ft_strsplit(cmd, ' ');
	client = conn->head;
	i = 0;
	(void)client;
	select = (t_select*)malloc(sizeof(t_select));
	FD_ZERO(&select->write_fds);
	FD_ZERO(&select->read_fds);	
	FD_SET(fd, &select->write_fds);
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
					return ;
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
}