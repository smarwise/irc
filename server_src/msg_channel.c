#include "../includes/server.h"

t_channel    *get_chan(t_channel *chan, char *chan_name)
{
	while (chan)
	{
		if (ft_strcmp(chan_name, chan->name) == 0)
			return (chan);
		chan = chan->next;
	}
	return(NULL);
}

t_client		*get_sender(t_client *client, int fd)
{
	while (client)
	{
		if (client->fd == fd)
			return (client);
		client = client->next;
	}
	return(NULL);
}

char	*add_channel_name(char *cmd, char *chan_name)
{
	char *tmp;
	char *tmp2;

	tmp = ft_strjoin(cmd, " ");
	tmp2 = ft_strjoin(tmp, chan_name);
	free(tmp);
	return (tmp2);
}

void	msg_channel(t_client *client, char *str, int fd, t_conn *conn)
{
	int i;
	t_client *sender;
    t_channel *channel;
    char    *cmd;
    char *real_cmd;
	char **array;

	client = conn->head;
	i = 0;
	array = ft_strsplit(str, ' ');
    if ((sender = get_sender(conn->head, fd)) != NULL)
	{
		channel = get_chan(conn->chan_head, array[1]);
            cmd = make_cmd(sender->name, str);
            real_cmd = add_channel_name(cmd, channel->name);
            send_to_all(sender, channel, real_cmd);
            i = 1;
            send_result(1, fd);
    }
    else
    {
        ft_err("User disconnected");
    }
	if (i == 0)
		send_result(-1, fd);
}