#include "../includes/server.h"

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

int    check_err(int fd, t_conn *conn, int nbytes, t_select *select)
{
    t_client *cl;
    char *name;

    cl = get_client(conn, fd);
    name = cl->name;
    if (nbytes == 0)
    {
        quit(conn, fd);
        if (name == NULL)
            ft_err("Client disconnected\n");
        else
        {
            ft_err(cl->name);
            ft_err(" logged out\n");
            cl->name = NULL;
        }
    }
    else
        ft_err("Receive error\n");
    close(fd);
    FD_CLR(fd, &select->master);
    return (-1);
}

void        read_cmd(t_buffer *buffer, size_t *i)
{
    while (buffer->read != buffer->write)
    {
        buffer->cmd[*i] = buffer->buffer[buffer->read];
        *i += 1;
        if (buffer->read == 49 && *i <= buffer->cmdlen)
            buffer->read = 0;
        else if (*i <= buffer->cmdlen && buffer->read != 49)
            buffer->read++;
    }
}

int   get_cmd(t_buffer *buffer, int fd, t_conn *conn, t_select *select)
{
    size_t i;
    char buf[50];
    int numbytes;

    i = 0;
    ft_putendl(buffer->buffer);
    buffer->cmd = ft_strnew(buffer->cmdlen);
    while (1)
    {
        ft_memset(buf, '\0', 50);
        if ((i == (buffer->cmdlen - 1))  || (i % 49 == 0 && i != 0))
        {
            buffer->cmd = realloc(buffer->cmd, buffer->cmdlen + 50);
            buffer->cmdlen += 50;
        }
        if ((numbytes = recv(fd, buf, 49, 0)) <= 0)
            return (check_err(fd, conn, numbytes, select));
        else
            add_to_buffer(buffer, ft_strtrim(buf));
        read_cmd(buffer, &i);
        if (numbytes < 49)
            break ;
    }
    buffer->cmd = ft_strtrim(buffer->cmd);
    ft_putendl(buffer->buffer);
    return (0);
}

t_buffer    *get_buffer(int fd, t_buffer *buf)
{
    printf("%p\n", buf);
    while (buf)
    {
        if (buf->fd == fd)
            return (buf);
        buf = buf->next;
    }
    return (NULL);
}

// void    check_cmd_exec(t_buffer *buff, int fd, t_client *client, t_conn *conn)
// {
//     char *temp;

//     if (buff->save == 1)
//     {
//         if (buff->incomplete == NULL)
//         {
//             buff->incomplete = ft_strdup(buff->cmd);
//         }
//         else
//         {
//             temp = buff->cmd;
//             buff->incomplete = ft_strjoin(buff->incomplete, buff->cmd);
//             exec_cmd(buff, conn, client, fd);
//         }
//     }
//     buff->save = 0;
// }

void    handle_old_clients(t_select *select, int *i, t_conn *conn)
{
    char buf[50];
    t_client *client;
    t_buffer *buff;

    ft_memset(buf, '\0', 50);
    client = conn->head;
    buff = get_buffer(*i, select->buffer);
    if (get_cmd(buff, *i, conn, select) == -1)
        return;
    exec_cmd(buff, conn, client, *i);
    // check_cmd_exec(buff, *i, client, conn);
}