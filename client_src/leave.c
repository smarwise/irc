#include "../includes/client.h"

char	**remove_one(char **array, char *name)
{
	int n;
	int i;
	char **new_array;

	n = 0;
	i = 0;
	new_array = (char**)malloc(sizeof(char *) * 1000);
	while (array[i])
	{
		if (ft_strcmp(array[i], name) != 0)
		{
			new_array[n] = ft_strdup(array[i]);
			n++;
		}
		i++;
	}
	new_array[n] = NULL;
	free_2d_array((void**)array);
	return (new_array);
}

void	process_result(char buf[10], t_user *user, char *str)
{
	if (ft_strcmp(buf, "success") == 0)
	{
		ft_succ("You left ");
		ft_succ(str);
		ft_putchar('\n');
		user->channels =remove_one(user->channels, str);
		if (arraylen(user->channels) <= 0)
		{
			free(user->channels);
			user->channels = NULL;
		}
	}
	else if (ft_strcmp(buf, "fail") == 0)
		ft_err("Leaving failed\n");
	else
		alertnexit("Undefined problem");
}

void 	leave_feedback(t_conn *conn, t_user *user, char *str)
{
	char buf[50];
	
	if (recv_cmd(buf, conn->fd) > 0)
		process_result(buf, user, str);
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

void	leave(char *str, t_user *user, t_conn *conn,
		char **array)
{
	if (user->logged_in == 0)
		ft_err("You must log in to use this command");
	else if (user->channels == NULL)
		ft_err("You are not in a channel");
	else
	{
		if (arraylen(array) == 2)
		{
			if (array[1][0] != '#')
				ft_err("Invalid channel name");
			else if (check_in_channel(user->channels, array[1]) == 0)
				ft_err("You are not in this channel");
			else
			{
				send_cmd(str, conn);
				leave_feedback(conn, user, array[1]);
			}
		}
		else
			ft_err("Incorrect use");
	}
}