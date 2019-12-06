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

void	who(t_conn *conn, int fd, char *cmd)
{
	t_channel *channel;
	t_client 	*client;
	char 		*cur_channel;
	char		*list;
	char		**array;

	client = conn->head;
	list = NULL;
	array = ft_strsplit(cmd, ' ');
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
	ft_putendl(list);
	send(fd, list, ft_strlen(list), 0);
	free(list);
 }