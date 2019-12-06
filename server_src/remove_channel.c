#include "../includes/server.h"

char	**remove_channel(char **array, char *name)
{
	int n;
	int i;
	char **new_array;

	n = 0;
	i = 0;
	new_array = (char**)malloc(sizeof(char *) * 1000);
    if (array != NULL)
    {
        while (array[i])
        {
            if (ft_strcmp(array[i], name) != 0)
            {
                new_array[n] = ft_strdup(array[i]);
                n++;
            }
            i++;
        }
    }
    new_array[n] = NULL;
    if (array != NULL)
	    free_2d_array((void**)array);
	return (new_array);
}

void    remove_channel_from_user(int fd, char *name, t_conn *conn)
{
    t_client *cl;
    int i;

    i = 0;
    cl = get_client( conn, fd);
    cl->channels = remove_channel(cl->channels, name);
    if (arraylen(cl->channels) <= 0)
    {
        free(cl->channels);
        cl->channels = NULL;
    }
}