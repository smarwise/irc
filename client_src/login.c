#include "../includes/client.h"

void 	send_cmd(char *cmd, t_conn *conn)
{
	int	len;
	char *size;
	char *real_cmd;

	len = ft_strlen(cmd);
	size = ft_itoa(len);
	real_cmd = ft_strjoin(size, cmd);
	send(conn->fd, real_cmd, ft_strlen(real_cmd), 0);
}

void	set_vars(t_user *user, char *str)
{
	char **array;

	user->logged_in = 1;
	array = ft_strsplit(str, ' ');
	user->name = ft_strdup(array[1]);
	ft_succ("You are successfully logged in\n");
	free_2d_array((void**)array);
}

void 	check_feedback(t_conn *conn, t_user *user, char *str)
{
	char buf[10];
	int nbytes;

	ft_memset(buf, '\0', 10);
	if ((nbytes = (recv(conn->fd, buf, 9, 0))) <= 0)
	{
		if (nbytes == 0)
		{
			ft_err("Connection got cut unexpectedly");
			exit(0);
		}
		else
			ft_err("Result receival failed");
	}
	else
	{
		if (ft_strcmp(buf, "success") == 0)
			set_vars(user, str);
		else if (ft_strcmp(buf, "fail") == 0)
			ft_err("Login failed: Username already taken");
		else
			alertnexit("Undefined problem");
	}
}

void	login(char *str, t_user *user, t_conn *conn)
{
	char **array;
	int ar_len;

	array = ft_strsplit(str, ' ');
	ar_len = get_array_len((void**)array);
	if (user->connected == 0)
		ft_err("Connect to server first");
	else if (ar_len == 2)
	{
		if (ft_strlen(array[1]) > 9)
		{
			ft_err("Nickname cannot be more than 9 characters");
			read_input(user, conn);
		}
		else
		{
			send_cmd(str, conn);
			check_feedback(conn, user, str);
		}
	}
	else
		ft_err("Command used incorrectly");
}