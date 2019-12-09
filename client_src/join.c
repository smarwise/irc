#include "../includes/client.h"

void	check_result(char buf[10], t_user *user, char *str)
{
	int i;

	i = 0;
	if (ft_strcmp(buf, "success") == 0)
	{
		if (user->channels == NULL)
			user->channels = (char **)malloc(sizeof(char *) * 1000);
		while (user->channels[i])
			i++;
		user->channels[i] = (char *)malloc(sizeof(char) * (ft_strlen(str) + 1));
		ft_succ("You joined ");
		ft_succ(str);
		ft_putchar('\n');
		user->channels[i] = ft_strdup(str);
		user->channels[i + 1] = NULL;
	}
	else if (ft_strcmp(buf, "fail") == 0)
		ft_err("Login failed\n");
	else
		alertnexit("Undefined problem");
		i = 0;
	while (user->channels[i])
	{
		ft_putendl(user->channels[i]);
		i++;
	}
}

void 	check_feedbac(t_conn *conn, t_user *user, char *str)
{
	char buf[10];
	int nbytes;

	ft_memset(buf, '\0', 10);
	if ((nbytes = (recv(conn->fd, buf, 9, 0))) <= 0)
	{
		if (nbytes == 0)
		{
			ft_err("Connection got cut unexpectedly\n");
			exit(0);
		}
		else
			ft_err("Result receival failed\n");
	}
	else
		check_result(buf, user, str);
}

void	join(char *str, t_user *user, t_conn *conn,
		char **array)
{
	if (user->logged_in == 0)
		ft_err("User must be logged in to join channels");
	else
	{
		if (arraylen(array) == 2)
		{
			if (array[1][0] != '#' || (ft_strlen(array[1]) == 1))
				ft_err("Invalid channel name");
			else if (ft_strlen(array[1]) > 16)
				ft_err("Channel name cannot be greater than 16");
			else if (check_in_channel(user->channels, array[1]))
				ft_err("You are already in this channel");
			else
			{
				send_cmd(str, conn);
				check_feedbac(conn, user, array[1]);
			}
		}
		else
			ft_err("Incorrect use");
	}
}