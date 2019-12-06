#include "../includes/client.h"

void	print_result(char buf[1000])
{
	char **arr;
	int n;

	n = 0;
	if (buf[0] != '\0')
	{
		arr = ft_strsplit(buf, ' ');
		ft_putstr("\033[0;33m");
		if (arraylen(arr) == 1)
			ft_putendl("Only you is in this channel");
		else
		{
			while (arr[n])
			{
				ft_putendl(arr[n]);
				n++;
			}
		}
		ft_putstr("\033[0m");
		free_2d_array((void**)arr);
	}
}

void	list_users(char *chan_name, t_conn *conn)
{
	char *str;
	char *tmp;
	char buf[1000];


	ft_memset(buf, '\0', 1000);
	ft_putstr("Users in channel ");
	ft_putstr(chan_name);
	ft_putendl(":");
	tmp = ft_strjoin("/who", " ");
	str = ft_strjoin(tmp, chan_name);
	send_cmd(str, conn);
	recv(conn->fd, buf, 1000, 0);
	print_result(buf);
}

void	who(t_user *user, t_conn *conn, char **array)
{
	int i;

	i = 0;
	if (user->channels == NULL)
		ft_err("You have to be in a channel to list participants");
	else if (check_in_channel(user->channels, array[1]))
		ft_err("You are not in this channel");
	else if (arraylen(array) != 01)
		ft_err("Incorrect use: /who");
	else
	{
		while (user->channels[i] != NULL)
		{
			list_users(user->channels[i], conn);
			i++;
		}
	}
}