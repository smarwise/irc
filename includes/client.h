#include "../libft/libft.h"
#include "irc.h"
#define BUFFSIZE 1024

typedef struct 		s_buffer
{
	int 			write;
	int 			end;
	int 			start;
	int 			read;
	char 			buffer[50];
	char			*cmd;
	int 			cmdlen;
}					t_buffer;

typedef struct 		s_user
{
	int 			logged_in;
	char			**channels;
	char			*name;
	int 			connected;
}					t_user;

typedef	struct 		s_conn
{
	int 			port;
	char			*hostname;
	int 			fd;
	t_buffer		*buffer;
}					t_conn;

typedef struct		s_fd
{
	fd_set		master;
	int 		fdmax;
	fd_set		read_fds;
	fd_set		write_fds;
}					t_fd;

int		handle_error(int errno);
void	alertnexit(char *msg);
int 	make_connection(t_user *user, t_conn *conn);
void	login(char *str, t_user *user, t_conn *conn);
int		read_input(t_user *user, t_conn *conn);
void 	ft_err(char *msg);
void	connect_(t_user *user, t_conn *conn);
void	handle_connect(char **array, t_user *user, t_conn *conn);
void 		ft_succ(char *msg);
void	join(char *str, t_user *user, t_conn *conn,
		char **array);
void 	send_cmd(char *cmd, t_conn *conn);
void	leave(char *str, t_user *user, t_conn *conn,
		char **array);
void	who(t_user *user, t_conn *conn, char **array);
void	quit(void);
void	msg(t_user *user, t_conn *conn, char *str);
void    receive_msg(t_conn *conn, t_buffer *buffer);
void	show_msg(t_buffer *buffer);
void    msg_channel(char *str, t_conn *conn, t_user *user);
void 	check(t_conn *conn);
int		check_in_channel(char **array, char *name);