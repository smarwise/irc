#include "irc.h"
#define BUFFSIZE 50


typedef struct 		s_buffer
{
	int 			write;
	int 			end;
	int 			start;
	int 			read;
	char 			buffer[50];
	char			*cmd;
	size_t			cmdlen;
	int				fd;
	int				save;
	char			*incomplete;
	struct s_buffer	*next;
}					t_buffer;

typedef struct s_client
{
	char			*ip;
	char			*name;
	int 			fd;
	char			**channels;
	int 			logged_in;
	struct s_client *next;
}				t_client;

typedef struct s_select
{
	fd_set		master;
	int 		fdmax;
	fd_set		read_fds;
	fd_set		write_fds;
	t_buffer	*buffer;
}				t_select;

typedef struct 		s_channel
{
	t_client		*users;
	int 			nb_clients;	
	char			*name;
	t_client		*head;
	char			*creator;
	int 			nb_users;
	struct s_channel *next;
}					t_channel;

typedef	struct 		s_conn
{
	int 			port;
	char			*hostname;
	int 			socketfd;
    socklen_t		sin_size;
    int 			nb_clients;
    t_client		*head;
    t_channel		*chan_head;
    t_channel 		*channel;
}					t_conn;

char    *get_ip(struct sockaddr_in *sa);
int		handle_error(int err);
int   	init(char *str);
void  	sigchild_handler();
void	send_result(int n, int fd);
void 	ft_err(char *msg);
void    handle_old_clients(t_select *select, int *i,
		t_conn *conn);
void 		ft_succ(char *msg);
int		exec_cmd(t_buffer *buffer, t_conn *conn, t_client *client, int fd);
void	disconnect_client(int fd, t_client *client, t_conn *conn);
int		join(t_conn *conn, char *str, int fd, t_buffer *buffer);
t_client	*structdup(int fd, t_conn *conn, char *chan_name);
t_client		*get_client(t_conn *conn, int fd);
int			ft_structlen(t_client *client);
int	leave(t_conn *conn, char *str, int fd);
int		who(t_conn *conn, int fd, char *cmd);
int		login(t_client *client, char *cmd, int fd, t_conn *conn);
t_client    *structend(t_client *client, t_conn *conn);
void	update_client_info(int fd, t_conn *conn, char *name);
void	remove_user(t_channel *chan, int fd, t_conn *conn);
int		quit(t_conn *conn, int fd);
int		msg(t_client *client, char *cmd, int fd, t_conn *conn);
int		msg_channel(t_client *client, char *cmd, int fd, t_conn *conn);
void    send_to_all(t_client *sender, t_channel *chan, char *msg);
void 	send_cmd(int fd, char *cmd);
char	*make_cmd(char *sender_name, char *msg);
void 	send_cmd(int fd, char *cmd);
void    remove_channel_from_user(int fd, char *name, t_conn *conn);
void	add_channel_to_client(t_client *cl, char *name);