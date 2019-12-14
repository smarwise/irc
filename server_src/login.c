#include "../includes/server.h"

int		check_if_exists(t_client *client, char *name)
{
	while (client)
	{
		if (ft_strcmp(client->name, name) == 0)
			return (1);
		client = client->next;
	}
	return(0);
}

void	set(t_client *client, int fd, char *str)
{
	client->name = ft_strdup(str);
	client->logged_in = 1;
	send_result(1, fd);
	ft_succ(client->name);
	ft_succ(" logged in\n");
}

int			login(t_client *client, char *cmd, int fd, t_conn *conn)
{
	int		i;
	char **array;

	array = ft_strsplit(cmd, ' ');
	client = conn->head;
	i = 0;
	if (arraylen(array) != 2)
	{
		send_info("Incorrect use\n", fd);
		return (-1);
	}
	if (ft_strlen(array[1]) > 9)
	{
		send_info("Username cannot be greater than 9 characters\n", fd);
		return (-1);
	}
	while (client)
	{
		if (client->fd == fd)
		{
			if (check_if_exists(conn->head, array[1]) == 1)
				break;
			set(client, fd, array[1]);
			i = 1;
			break;
		}
		client = client->next;
	}
	if (i == 0)
		send_result(-1, fd);
	free_2d_array((void**)array);
	return (1);
}