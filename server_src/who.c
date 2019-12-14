#include "../includes/server.h"

char	*get_list(t_channel *channel)
{
	t_client *cl;
	char *tmp;
	char *tmp2;
	int n;

	cl = channel->head;
	n = 0;
	while (cl)
	{
		if (n == 0)
			tmp = ft_strdup(cl->name);
		else
		{
			tmp2 = tmp;
			tmp = ft_strjoin(tmp, " ");
			free(tmp2);
			tmp2 = tmp;
			tmp = ft_strjoin(tmp, cl->name);
			free(tmp2);
		}
		cl = cl->next;
		n++;
	}
	return(tmp);
}

int		check_who(t_client *client, char **array, int fd)
{
	if (client->channels == NULL)
		send(fd, "You have to be in a channel to list participants\n", 50, 0);
	else if (check_in_channel(client->channels, array[1]) == 0)
		send(fd, "You are not in this channel\n", 29, 0);
	else if (arraylen(array) != 2)
		send(fd, "Incorrect use: /who <channel name>\n", 36, 0);
	else
		return (1);
	return (-1);
}

int		who(t_conn *conn, int fd, char *cmd)
{
	t_channel *channel;
	t_client 	*client;
	char 		*cur_channel;
	char		*list;
	char		**array;

	client = get_client(conn, fd);
	list = NULL;
	array = ft_strsplit(cmd, ' ');
	if (check_who(client, array, fd) > 0)
	{
		channel = conn->chan_head;
		cur_channel = array[1];
		while (channel)
		{
			if (ft_strcmp(channel->name, cur_channel) == 0)
			{
				list = get_list(channel);
				break;
			}
			channel = channel->next;
		}
		send(fd, list, ft_strlen(list), 0);
		free(list);
		return (1);
	}
	return (-1);
 }