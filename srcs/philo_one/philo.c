/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 14:24:59 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/28 15:03:04 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

char	*get_action(int action)
{
	if (action == FORK_ACTION)
		return ("has taken a fork\n");
	else if (action == EAT_ACTION)
		return ("is eating\n");
	else if (action == SLEEP_ACTION)
		return ("is sleeping\n");
	else if (action == THINK_ACTION)
		return ("is thinking\n");
	else if (action == DIE_ACTION)
		return ("died\n");
	else
		return ("End of simulation: reacthed eat limit\n");
}

void	print_status(t_philo *philo, int action)
{
	if (pthread_mutex_lock(philo->print) < 0)
		exit(1);
	ft_putnbr(get_time() - philo->table->start);
	ft_putchar('\t');
	if (action != 6)
	{
		ft_putnbr(philo->name);
		ft_putchar(' ');
	}
	ft_putstr(get_action(action));
	action != 5 ? pthread_mutex_unlock(philo->print) : 0;
}

int		main(int argc, char **argv)
{
	t_table	*table;

	if (!valid_args(argc, ++argv))
		return (1);
	table = init_table(argc, argv);
	create_lifes(table);
	finish_simulation(table);
	return (0);
}
