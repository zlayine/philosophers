/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 17:14:31 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/18 13:35:21 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>

int			ft_atoi(const char *str);
void		ft_putstr(char *s);
char		*ft_itoa(long n);
void		ft_putchar(char c);
void		ft_del(void *data);
int			ft_is_strdig(char *str);
char		*ft_strjoin(char const *s1, char const *s2);
size_t		ft_strlen(const char *str);
int			valid_args(int total, char **args);
long		get_current_time(int micro, struct timeval start_time);
void		ft_putnbr(long long d);

#endif
