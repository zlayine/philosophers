/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 17:14:27 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/19 16:38:12 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int		valid_args(int total, char **args)
{
	if (total < 5 || total > 6)
	{
		ft_putstr("Error: please specify the required arguments\n");
		return (0);
	}
	if (!ft_is_strdig(args[0]) || !ft_is_strdig(args[1]) ||
		!ft_is_strdig(args[2]) || !ft_is_strdig(args[3]) ||
		(total == 6 && !ft_is_strdig(args[4])))
	{
		ft_putstr("Error: value of an arguments must be numbers only\n");
		return (0);
	}
	if (ft_atoi(args[0]) <= 1 || ft_atoi(args[1]) <= 60 ||
		ft_atoi(args[2]) <= 60 || ft_atoi(args[3]) <= 60 || (total == 6 &&
			ft_atoi(args[4]) <= 0))
	{
		ft_putstr("Error: value of an argument is out of range\n");
		return (0);
	}
	return (1);
}

long	get_current_time(int micro, struct timeval start_time)
{
	struct timeval	current_time;
	int				time;

	gettimeofday(&current_time, NULL);
	time = ((current_time.tv_sec - start_time.tv_sec)
		* 1000000L + current_time.tv_usec) - start_time.tv_usec;
	if (!micro)
		return (time / 1000);
	else
		return (time);
}

long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}
