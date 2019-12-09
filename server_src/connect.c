#include "../includes/server.h"

int    init_struct(struct sockaddr_in *sa, t_conn *conn, t_select *select)
{
    struct sigaction sig;

    sa->sin_family = AF_INET;
    sa->sin_port = htons(conn->port);
    sa->sin_addr.s_addr = INADDR_ANY;
    ft_memset(sa->sin_zero, '\0', sizeof(sa->sin_zero));
    if (bind(conn->socketfd, (struct  sockaddr *)sa,
        sizeof(*sa)) == -1)
        return (handle_error(2));
    if (listen(conn->socketfd, BACKLOG) == -1)
        return (handle_error(3));
    FD_SET(conn->socketfd, &select->master);
    ft_putendl("\033[1;12mListening..........\033[1;0m");
    sig.sa_handler = sigchild_handler;
    sigemptyset(&sig.sa_mask);
    sig.sa_flags = SA_RESTART;
    if (signal(1,sigchild_handler) == SIG_ERR)
        return (handle_error(4));
    return 0;
}

void  create_client_buffer(int fd, t_select *select, t_conn *conn)
{
    t_buffer *buf;

    if (conn->nb_clients == 0)
    {
        buf = (t_buffer*)malloc(sizeof(t_buffer));
        select->buffer = buf;
    }
    else
    {
        buf = select->buffer;
        while (buf->next)
            buf = buf->next;
        buf->next = (t_buffer*)malloc(sizeof(t_buffer));
        buf = buf->next;
    }
    buf->write = 0;
    buf->read = 0;
    buf->cmdlen = 50;
    buf->fd = fd;
    buf->save = 0;
    buf->incomplete = NULL;
    ft_memset(buf->buffer, '\0', 50);
    buf->next = NULL;
}

void   connect_new_client(t_conn *conn, t_select *select,
    struct sockaddr_in *their_addr)
{
    t_client *client;

    conn->sin_size = sizeof(*their_addr);
    client = conn->head;
    client = structend(client, conn);
    if ((client->fd  = accept(conn->socketfd,
        (struct sockaddr *)their_addr, &conn->sin_size)) != -1 )
    {
        FD_SET(client->fd, &select->master);
        if (client->fd > select->fdmax)
            select->fdmax = client->fd;
        client->ip = get_ip(their_addr);
        client->next = NULL;  
        client->name = NULL;
        client->channels = NULL;
        ft_succ("New client connected\n");
        create_client_buffer(client->fd, select, conn);
        conn->nb_clients++;
    }
    else
    {
        ft_err("Accepting failed");
        exit (0);
    }
}

void init_vars(t_select *select, t_conn *conn)
{
    select->fdmax = conn->socketfd;
    conn->nb_clients = 0; 
    conn->chan_head = NULL;
}

int  be_connected(t_conn *conn, t_select *select)
{
    int i;
    struct sockaddr_in their_addr;

    init_vars(select, conn);
    while (42)
    {
        select->read_fds = select->master;
        select->write_fds = select->master;
        do_select(select->fdmax, &select->read_fds, &select->write_fds);
        for (i = 0; i <= select->fdmax; i++)
        {
            if (FD_ISSET(i, &select->read_fds))
            {
                if (i == conn->socketfd) 
                    connect_new_client(conn, select, &their_addr);
                else
                    handle_old_clients(select, &i, conn);
            }
        }
    }
    return 0;
}

int		check_is_num(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void    get_port(t_conn *conn, char *str)
{
    if (!check_is_num(str))
	{
		ft_putendl("Port should be a number");
		exit (0);
	}
    conn->port = ft_atoi(str);
}

int   init(char *str)
{
    struct sockaddr_in my_addr;
    int yes;
    struct protoent *pe;
    t_select *select;
    t_conn *conn;
  
    pe = (struct protoent*)getprotobyname("tcp");
    select = (t_select*)malloc(sizeof(t_select));
    conn = (t_conn*)malloc(sizeof(t_conn));
    yes = 1;
    get_port(conn, str);
    conn->nb_clients = 0;
    FD_ZERO(&select->master);  
    FD_ZERO(&select->read_fds);
    {
        if ((conn->socketfd = socket(PF_INET, SOCK_STREAM, pe->p_proto)) == -1)
           return (handle_error(1));
        if (setsockopt(conn->socketfd, SOL_SOCKET, SO_REUSEADDR,
            &yes, sizeof(int)) == -1)
            return (handle_error(1));
        init_struct(&my_addr, conn, select);
        return (be_connected(conn, select));
    }
    return 0;
}