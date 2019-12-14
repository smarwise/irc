#include "../includes/client.h"

void 	check(t_conn *conn)
{
	char buf[50];
	
	if (recv_cmd(buf, conn->fd))
	{
		if (ft_strcmp(buf, "success") == 0)
			ft_succ("Message sent\n");
		else if (ft_strcmp(buf, "fail") == 0)
			ft_err("Error in sending message");
		else
			alertnexit("You have officially been booted ");
	}
}

void	msg(t_user *user, t_conn *conn, char *str)
{
    char **array;

    array = ft_strsplit(str, ' ');
    if (arraylen(array) < 3)
        ft_err("Too few arguments to command call");
    else if (user->logged_in == 0)
        ft_err("You must be logged in to send messages");
    else if (ft_strlen(array[1]) > 9)
	{
		ft_err("Nickname cannot be more than 9 characters");
		read_input(user, conn);
	}
    else
    {
        send_cmd(str, conn);
        check(conn);
    }
}