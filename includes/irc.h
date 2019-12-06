#include "../libft/libft.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <netdb.h>
#include <sys/select.h>
#include <netinet/in.h>
#define BACKLOG 10
#define MAXDATASIZE 1000

void	do_select(int fdmax, fd_set *read_fds, fd_set *write_fds);
void		free_2d_array(void **array);
int		ft_atoi(const char *str);
void				*ft_memset(void *b, int c, size_t len);
void	ft_putendl(char const *s);
void    ft_putchar(char c);
char				**ft_strsplit(char const *s, char c);