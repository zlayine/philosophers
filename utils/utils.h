/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 17:14:31 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/15 17:14:41 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <unistd.h>
# include <stdlib.h>

int			ft_atoi(const char *str);
void		ft_putstr(char *s);
char		*ft_itoa(long n);
void		ft_putchar(char c);
void		ft_del(void *data);

#endif
