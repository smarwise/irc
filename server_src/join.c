#include "../includes/server.h"

void		create_channel(t_conn *conn, char *name,
			char *chan_name, int n)
{
	if (n == 0)
	{
		conn->channel = (t_channel *)malloc(sizeof(t_channel));
		conn->chan_head = conn->channel;
	}
	else
	{
		conn->channel = conn->chan_head;
		while (conn->channel->next)
			conn->channel = conn->channel->next;
		conn->channel->next = (t_channel *)malloc(sizeof(t_channel));
		conn->channel = conn->channel->next;
	}
	conn->channel->name = ft_strdup(chan_name);
	conn->channel->creator = ft_strdup(name);
	conn->channel->head = NULL;
	conn->channel->nb_users = 0;
	conn->channel->next = NULL;
}

void		add_to_channel(t_conn *conn, int fd, char *chan_name)
{
	t_client *client;

	conn->channel = conn->chan_head;
	while (ft_strcmp(conn->channel->name, chan_name) != 0)
		conn->channel = conn->channel->next;
	if (conn->channel->nb_users > 0)
    {
        client = conn->channel->head;
        while (client->next)
            client = client->next;
        client->next = structdup(fd, conn, chan_name);
        client->next->next = NULL;
    }
    else
    {
        client = structdup(fd, conn, chan_name);
        conn->channel->head = client;
        client->next = NULL; 
    }
    conn->channel->nb_users++;
    send_result(1, fd);
}

void	add_channel_to_client(t_client *cl, char *name)
{
	int i;

	i = 0;
	if (cl->channels == NULL)
	{
		cl->channels = (char**)malloc(sizeof(char*) * 1000);
		cl->channels[0] = NULL;
	}
	if  (cl->channels[i])
	{
		while (cl->channels[i])
			i++;
	}
	cl->channels[i] = (char *)malloc(sizeof(char) * ft_strlen(name));
	cl->channels[i] = ft_strdup(name);
	cl->channels[i + 1] = NULL;
}

int		check_in_channel(char **array, char *name)
{
	int i;

	i = 0;
	if (array)
	{
		while (array[i])
		{
			if (ft_strcmp(array[i], name) == 0)
				return (1);
			i++;
		}
	}
	return (0);
}

int		check_inp(t_client *cl, char **array, t_buffer *buffer, int fd)
{
	if (arraylen(array) == 1)
	{
		buffer->save = 1;
	}
	else if (!cl->name)
		send_info("You must be logged in to join a channel\n", fd);
	else if ((array[1][0]) != '#')
		send_info("Invalid channel name\n", fd);
	else if (ft_strlen(array[1]) < 2)
		send_info("Invalid channel name\n", fd);
	else if (ft_strlen(array[1]) > 16)
		send_info("Channel name cannot be greater than 16\n", fd);
	else if (check_in_channel(cl->channels, array[1]))
		send_info("You are already in this channel\n", fd);
	else
		return (1);
	return (-1);
}

int		join(t_conn *conn, char *str, int fd, t_buffer *buffer)
{
	char **array;
	t_client *cl;
	int set;

	array = ft_strsplit(str, ' ');
	cl = get_client(conn, fd);
	if (check_inp(cl, array, buffer, fd) > 0)
	{
		set = 0;
		if (conn->chan_head == NULL)
			create_channel(conn, cl->name, array[1], 0);
		else
		{
			conn->channel = conn->chan_head;
			while (conn->channel)
			{
				if (ft_strcmp(array[1], conn->channel->name) == 0)
					set = 1;
				conn->channel = conn->channel->next;
			}
			if (set == 0)
				create_channel(conn, cl->name, array[1], 1);
		}
		add_to_channel(conn, fd, array[1]);
		ft_succ(ft_strjoin(cl->name, " joined channel"));
		ft_succ(ft_strjoin(" ", array[1]));
		ft_putchar('\n');
		return (1);
	}
	return (-1);
}
