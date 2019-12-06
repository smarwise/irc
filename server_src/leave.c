#include "../includes/server.h"

void	destroy_one(t_channel *chan, t_conn *conn)
{
	t_channel *tmp;

	if (chan->next == NULL)
	{
		free(chan);
		chan = NULL;
		conn->chan_head = NULL;
		ft_putendl("channel destroyed");
		return ;
	}
	tmp = chan->next;
	conn->chan_head = tmp;
	free(chan);
}

void	destroy_chan(t_channel *chan, t_conn *conn)
{
	char *name;
	t_channel *tmp;

	name = chan->name;
    chan = conn->chan_head;
	if (ft_strcmp(name, chan->name) == 0)
		destroy_one(chan, conn);
	else
	{	
		while (chan)
		{
			if (ft_strcmp(name, chan->next->name) == 0)
			{
				tmp = chan->next->next;
				free(chan->next);
				chan->next = tmp;
				break;
			}
			chan = chan->next;
		}
	}
}

void	remove_one(t_client *client, t_channel *chan, t_conn *conn)
{
	t_client *tmp;

	if (client->next == NULL)
	{
		free(client);
		client = NULL;
		chan->nb_users = 0;
		chan->head = NULL;
		ft_putendl("no users left");
		destroy_chan(chan, conn);
		return ;
	}
	tmp = client->next;
	chan->head = tmp;
	free(client);
	chan->nb_users--;
}


void	remove_user(t_channel *chan, int fd, t_conn *conn)
{
	t_client *tmp;
	t_client *client;

    client = chan->head;
	if (client->fd == fd || ft_structlen(client) == 1)
		remove_one(client, chan, conn);
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
	chan->nb_users--;
}

void	leave(t_conn *conn, char *str, int fd)
{
	char **array;
	t_channel *chan;
	int n;

	array = ft_strsplit(str, ' ');
	chan = conn->chan_head;
	n = 0;
	while (chan)
	{
		if (ft_strcmp(chan->name, array[1]) == 0)
		{
			remove_user(chan, fd, conn);
			remove_channel_from_user(fd, array[1], conn);
			send_result(1, fd);
			n = 1;
		}
		chan = chan->next;
	}
	if (n == 0)
	{
		ft_err("Channel does not exist");
		send_result(0, fd);
	}
}