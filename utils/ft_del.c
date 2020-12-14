#include "utils.h"

void	ft_del(void *data)
{
	if (data)
		free(data);
}
