#ifndef UTILS_H
# define UTILS_H

#include <unistd.h>
#include <stdlib.h> 

int			ft_atoi(const char *str);
void		ft_putstr(char *s);
char		*ft_itoa(long n);
void		ft_putchar(char c);
void		ft_del(void *data);

#endif