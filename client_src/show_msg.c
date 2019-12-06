#include "../includes/client.h"

char    *get_sender_name(char *cmd, int *n)
{
    int len;

    len = *n + 1;
    while (cmd[*n] != ' ')
        (*n)--;
    return (ft_strsub(cmd, *n, len - *n));
}

void    handle_private_msg(char *cmd)
{
    char *sender_name;
    int n;
    int i;
    char *msg;

    ft_putendl("");
    i = 0;
    while (cmd[i] != ' ')
        i++;
    while (cmd[i] == ' ')
        i++;
    while (cmd[i] != ' ')
        i++;
    while (cmd[i] == ' ')
        i++;
    n = ft_strlen(cmd) - 1;
    sender_name = get_sender_name(cmd, &n);
    ft_putstr("\e[1;33m<msg> from");
    ft_putstr(sender_name);
    ft_putendl(":\e[0m");
    msg = ft_strsub(cmd, i, n - i);
    ft_putendl(msg);
}

void    channel_msg(char *cmd)
{
    char *sender_name;
    char *channel_name;
    int n;
    int i;
    char *msg;
    int len;
    int t;

    i = 0;
    while (cmd[i] != ' ')
        i++;
    while (cmd[i] == ' ')
        i++;
    while (cmd[i] != ' ')
        i++;
    while (cmd[i] == ' ')
        i++;
    n = ft_strlen(cmd) - 1;
    len = n;
    while (cmd[len] != ' ')
        len--;
    channel_name = ft_strsub(cmd, len, (n+1) - len);
    while (cmd[len] == ' ')
        len--;
    t = len + 1;
    while (cmd[len] != ' ')
        len--;
    sender_name = ft_strsub(cmd, len + 1, t - len);
    ft_putstr("\e[1;33m<msg> from channel");
    ft_putstr(channel_name);
    ft_putendl(":\e[0m");
    ft_putstr(sender_name);
    ft_putstr(": ");
    msg = ft_strsub(cmd, i, len - i);
    ft_putendl(msg);
}

void	show_msg(t_buffer *buffer)
{
    if (ft_strstr(buffer->cmd, "/msg_channel"))
        channel_msg(buffer->cmd);
    else if (ft_strstr(buffer->cmd, "/msg"))
        handle_private_msg(buffer->cmd);
}