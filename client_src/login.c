#include "../includes/client.h"

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
	char buf[50];

	if (recv_cmd(buf, conn->fd) > 0)
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