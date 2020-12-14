#include "utils.h"

void	ft_putstr(char *s)
{
	if (s)
		while (*s)
			ft_putchar(*s++);
}
