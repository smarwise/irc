#include "../includes/server.h"

t_client		*get_client(t_conn *conn, int fd)
{
	t_client *cl;

	cl = conn->head;
	while (cl->fd != fd)
		cl = cl->next;
	return (cl);
}

t_client	*structdup(int fd, t_conn *conn, char *chan_name)
{
	t_client *client;
	t_client *cpy;

	cpy = (t_client *)malloc(sizeof(t_client));
	client = conn->head;
	while (client->fd != fd)
		client = client->next;
	cpy->ip = ft_strdup(client->ip);
	cpy->name = ft_strdup(client->name);
	cpy->fd = fd;
	cpy->channels = NULL;
	add_channel_to_client(cpy, chan_name);
	add_channel_to_client(client, chan_name);
	return (cpy);
}

int			ft_structlen(t_client *client)
{
	t_client *tmp;
	int len;

	len = 0;
	tmp = client;
	while (tmp)
	{
		tmp = tmp->next;
		len++;
	}
	return (len);
}

t_client    *structend(t_client *client, t_conn *conn)
{
    if (conn->nb_clients > 0)
    {
        client = conn->head;
        while (client->next)
            client = client->next;
        client->next = (t_client*)malloc(sizeof(t_client));
        client = client->next;
    }
    else
    {
        client = (t_client*)malloc(sizeof(t_client));
        conn->head = client;
    }
    return (client);
}