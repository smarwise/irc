#include "../includes/server.h"

int			structlen(t_conn *conn)
{
	int 	len;
	t_client	*ptr;

	len = 0;
	ptr = conn->head;
	while (ptr)
	{
		len++;
		ptr = ptr->next;
	}
	return (len);

}

void		disconnect_first(t_client *client, t_conn *conn)
{
	t_client *tmp;
	
	if (client->next == NULL)
	{
		free(client);
		client = NULL;
		conn->nb_clients = 0;
		conn->head = NULL;
		return ;
	}
	tmp = client->next;
	conn->head = tmp;
	free(client);
}

void		disconnect_client(int fd, t_client *client, t_conn *conn)
{
	t_client *tmp;

    client = conn->head;
	if (client->fd == fd || structlen(conn) == 1)
		disconnect_first(client, conn);
	else
	{	
		while (client)
		{
			if (client->next->fd == fd)
			{
				tmp = client->next->next;
				free(client->next);
				client->next = tmp;
				break;
			}
			client = client->next;
		}
	}
	conn->nb_clients--;
}

int		quit(t_conn *conn, int fd)
{
	t_client *client;
	t_channel *chan;
	int i;

	i = 0;
	client = conn->head;
	while (client)
	{
		if (client->fd == fd)
			break;
		client = client->next;
	}
	if (client->channels != NULL)
	{
		while (client->channels[i])
		{
			chan = conn->chan_head;
			while (chan)
			{
				if (ft_strcmp(chan->name, client->channels[i]) == 0)
				{
					remove_user(chan, fd, conn);
					break;
				}
				chan = chan->next;
			}
			i++;
		}
	}
	disconnect_client(fd, client, conn);
	return (1);
}