#include "../includes/server.h"

void    load_fds(t_client *clients, int fd, t_select *select, int *fdmax)
{
    FD_ZERO(&select->read_fds);
    FD_ZERO(&select->write_fds);
    while (clients)
    {
        if (fd != clients->fd)
        {
            ft_putnbr(clients->fd);
            FD_SET(clients->fd, &select->write_fds);
            if (clients->fd > *fdmax)
                *fdmax = clients->fd;
        }
        clients = clients->next;
    }
}

void    send_to_all(t_client *sender, t_channel *chan, char *msg)
{
    t_select *select;
    int     fdmax;
    t_client *clients;

    fdmax = 0;
    select = (t_select*)malloc(sizeof(t_select));
    clients = chan->head;
    load_fds(chan->head, sender->fd, select, &fdmax);
    do_select(fdmax, &select->read_fds, &select->write_fds);
    while (clients)
    {
        if (FD_ISSET(clients->fd, &select->write_fds))
        {
            send_cmd(clients->fd, msg);
        }
        else
        {
            ft_err("Failed to send to client ");
            ft_putstr(clients->name);
        }
        clients = clients->next;
    }
}