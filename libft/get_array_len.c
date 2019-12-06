#include "libft.h"

int		get_array_len(void **array)
{
	int		i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}