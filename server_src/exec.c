#include "../includes/server.h"

int		exec_cmd(t_buffer *buffer, t_conn *conn, t_client *client, int fd)
{
	if (ft_strstr(buffer->cmd, "/nick"))
		return (login(client, buffer->cmd, fd, conn));
	else if (ft_strstr(buffer->cmd, "/join"))
		return (join(conn, buffer->cmd, fd, buffer));
	else if (ft_strstr(buffer->cmd, "/leave"))
		return (leave(conn, buffer->cmd, fd));
	else if (ft_strstr(buffer->cmd, "/who"))
		return (who(conn, fd, buffer->cmd));
	else if (ft_strstr(buffer->cmd, "/msg_channel"))
		return (msg_channel(client, buffer->cmd, fd, conn));
    else if (ft_strstr(buffer->cmd, "/msg"))
		return (msg(client, buffer->cmd, fd, conn));
	else
	{
		buffer->save = 1;
		return (0);
	}
}