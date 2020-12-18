/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 16:51:05 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/18 10:57:25 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_philo *philo, int action)
{
	char	*tmp;

	if (philo->die || (philo->table->end && action != 5) ||
		(action == 5 && philo->table->end != philo->name))
		return ;
	action != 5 ? sem_wait(philo->print) : 0;
	tmp = ft_itoa(get_current_time(0, philo->start_time));
	ft_putstr(tmp);
	ft_del(tmp);
	ft_putchar(' ');
	tmp = ft_itoa(philo->name);
	ft_putstr(tmp);
	ft_del(tmp);
	ft_putchar(' ');
	if (action == FORK_ACTION)
		ft_putstr("has taken a fork\n");
	else if (action == EAT_ACTION)
		ft_putstr("is eating\n");
	else if (action == SLEEP_ACTION)
		ft_putstr("is sleeping\n");
	else if (action == THINK_ACTION)
		ft_putstr("is thinking\n");
	else if (action == DIE_ACTION)
		ft_putstr("died\n");
	action != 5 ? sem_post(philo->print) : 0;
}

// long	get_current_time(int micro, struct timeval start_time)
// {
// 	struct timeval	current_time;
// 	int				time;

// 	gettimeofday(&current_time, NULL);
// 	time = ((current_time.tv_sec - start_time.tv_sec)
// 		* 1000000L + current_time.tv_usec) - start_time.tv_usec;
// 	if (!micro)
// 		return (time / 1000);
// 	else
// 		return (time);
// }

// int		valid_args(int total, char **args)
// {
// 	if (total < 5 || total > 6)
// 	{
// 		ft_putstr("Error: please specify the required arguments\n");
// 		return (0);
// 	}
// 	if (!ft_is_strdig(args[0]) || !ft_is_strdig(args[1]) ||
// 		!ft_is_strdig(args[2]) || !ft_is_strdig(args[3]) ||
// 		(total == 6 && !ft_is_strdig(args[4])))
// 	{
// 		ft_putstr("Error: value of an arguments must be numbers only\n");
// 		return (0);
// 	}
// 	if (ft_atoi(args[0]) <= 0 || ft_atoi(args[1]) <= 0 ||
// 		ft_atoi(args[2]) <= 0 || ft_atoi(args[3]) <= 0 || (total == 6 &&
// 			ft_atoi(args[4]) <= 0))
// 	{
// 		ft_putstr("Error: value of an argument is out of range\n");
// 		return (0);
// 	}
// 	return (1);
// }

int		main(int argc, char **argv)
{
	t_table	*table;

	if (!valid_args(argc, ++argv))
		return (1);
	table = init_table(argv);
	create_lifes(table);
	return (0);
}
