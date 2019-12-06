#include "../includes/client.h"

void    add_to_buffer(t_buffer *buff, char buffer[50])
{
    int i;

    i = 0;
    while (buffer[i])
    {
        if (buff->write == 49)
            buff->write = 0;
        buff->buffer[buff->write] = buffer[i];
        buff->write++;
        i++;
    }
}

void    get_full_cmd(int len, t_buffer *buffer, int fd)
{
    int i;
    char buf[50];

    i = 0;
    buffer->cmd = ft_strnew(buffer->cmdlen);
    while ( i < len)
    {
        if ((i == buffer->cmdlen - 1)  || (i % 48 == 0 && i != 0))
        {
            ft_memset(buf, '\0', 50);
            buffer->cmd = realloc(buffer, len + 1);
            buffer->cmdlen = len + 1;
            if (recv(fd, buf, 49, 0) <= 0)
                ft_err("Receive error\n");
            else
                add_to_buffer(buffer, buf);
        }
        buffer->cmd[i] = buffer->buffer[buffer->read];
        i++;
        if (buffer->read == 48 && i <= len)
            buffer->read = 0;
        else if (i <= len && buffer->read != 48)
            buffer->read++;
    }
}

void    read_cmds(t_buffer *buffer, int fd)
{
    char    number[10];
    int i;

    i = 0;
    ft_memset(number, '\0', 10);
    while (buffer->buffer[buffer->read])
    {
        if (ft_isalpha(buffer->buffer[buffer->read]) == 0)
        {
            number[i] = buffer->buffer[buffer->read];
            i++;
        }
        else
        {
            buffer->read == 0 ? buffer->read = 48 : buffer->read--;
            break;
        }
        buffer->read == 48 ? buffer->read = 0 : buffer->read++;
    }
    get_full_cmd(ft_atoi(number), buffer, fd);
}

void    check_err(t_conn *conn, int nbytes)
{
    if (nbytes == 0)
    {
        ft_err("Server disconnected unexpectedly");
        close(conn->fd);
        quit();
    }
    else
        ft_err("Receive error\n");
}

void    receive_msg(t_conn *conn, t_buffer *buffer)
{
    char buf[50];
    int nbytes;

    ft_memset(buf, '\0', 50);
    while (1)
    {
        if ((nbytes = (recv(conn->fd, buf, 49, 0))) <= 0)
        {
            check_err(conn, nbytes);
        }
        else
        {
            add_to_buffer(buffer, buf);
            read_cmds(buffer, conn->fd);
            show_msg(buffer);
        }
        break;
    }
}