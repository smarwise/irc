#include "../includes/server.h"

void	exec_cmd(t_buffer *buffer, t_conn *conn, t_client *client, int fd)
{
	if (ft_strstr(buffer->cmd, "/nick"))
		login(client, buffer->cmd, fd, conn);
	else if (ft_strstr(buffer->cmd, "/join"))
		join(conn, buffer->cmd, fd, buffer);
	else if (ft_strstr(buffer->cmd, "/leave"))
		leave(conn, buffer->cmd, fd);
	else if (ft_strstr(buffer->cmd, "/who"))
		who(conn, fd, buffer->cmd);
	else if (ft_strstr(buffer->cmd, "/msg_channel"))
		msg_channel(client, buffer->cmd, fd, conn);
    else if (ft_strstr(buffer->cmd, "/msg"))
		msg(client, buffer->cmd, fd, conn);
	else
		buffer->save = 1;
}