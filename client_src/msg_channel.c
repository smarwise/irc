#include "../includes/client.h"

void    msg_channel(char *str, t_conn *conn, t_user *user)
{
    char **array;

    array = ft_strsplit(str, ' ');
    if (arraylen(array) < 3)
        ft_err("Input message to send");
    else if (user->logged_in == 0)
        ft_err("You have to be logged in to send message");
    else if (user->channels == NULL)
        ft_err("You have to be in a channel to send channel messages");
    else if (check_in_channel(user->channels, array[1]) != 1)
        ft_err("You are not in this channel");
    else
    {
        send_cmd(str, conn);
        check(conn);
    }
}