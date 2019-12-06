#include "../includes/client.h"

int		check_is_num(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void    get_port(t_conn *conn, char *str)
{
    if (!check_is_num(str))
	{
		ft_putendl("Port should be a number");
		exit (0);
	}
    conn->port = ft_atoi(str);
}

int	main(int argc, char **argv)
{
	t_conn	*conn;
	t_user	*user;

	conn = (t_conn*)malloc(sizeof(t_conn));
	user = (t_user*)malloc(sizeof(t_user));
	conn->buffer = (t_buffer *)malloc(sizeof(t_buffer));
	user->logged_in = 0;
	user->connected = 0;
	user->channels = NULL;
	conn->fd = -1;
	if (argc == 3)
	{
		get_port(conn, argv[2]);
		conn->hostname = argv[1];
		connect_(user, conn);
	}
	else if (argc == 1)
	{
		while (1)
			read_input(user, conn);
	}
	else
		ft_putendl("Not enough args\n");
	return (0);
}