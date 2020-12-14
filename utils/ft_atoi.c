#include "utils.h"

static int	ft_check(long n, int sign)
{
	if (n < 0 && sign > 0)
		return (-1);
	if (n > 0 && sign < 0)
		return (0);
	return ((int)n);
}

int			ft_atoi(const char *str)
{
	int		i;
	long	n;
	int		sign;

	i = 0;
	n = 0;
	sign = 1;
	while (str[i] == '\t' || str[i] == ' ' || str[i] == '\n' ||
		str[i] == '\v' || str[i] == '\r' || str[i] == '\f')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] != '\0' && str[i] >= '0' && str[i] <= '9')
	{
		n = n * 10 + (str[i] - 48);
		i++;
	}
	n = n * sign;
	return (ft_check(n, sign));
}
