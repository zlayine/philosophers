/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finish.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 14:52:29 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/15 20:53:47 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_simulation(t_philo *curr, int death)
{
	t_philo	*tmp;

	sem_close(curr->print);
	sem_close(curr->sem);
	if (death == 2)
		sem_post(curr->done);
	sem_close(curr->done);
	sem_unlink("table_sem");
	sem_unlink("print_sem");
	sem_unlink("done_sem");
	while (curr && death == 2)
	{
		pthread_detach(curr->checker);
		tmp = curr->next;
		ft_del(curr);
		curr = tmp;
	}
}

void	finish_simulation(t_table *table, int death)
{
	t_philo	*curr;
	t_philo	*tmp;
	int		i;

	i = -1;
	curr = table->philos;
	curr->prev->next = NULL;
	if (death != 2 && death != 0)
		ft_putstr("End of simulation: one of the philosophers died\n");
	else if (death != 2)
		ft_putstr("End of simulation: philosophers reached the eat limit\n");
	while (table->forks < table->persons)
	{
		sem_post(curr->sem);
		table->forks++;
	}
	sem_close(table->game);
	sem_unlink("game_sem");
	if (death != 2)
	{
		ft_del(table);
		exit(0);
	}
}

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
	if (ft_atoi(args[0]) <= 0 || ft_atoi(args[1]) <= 0 ||
		ft_atoi(args[2]) <= 0 || ft_atoi(args[3]) <= 0 || (total == 6 &&
			ft_atoi(args[4]) <= 0))
	{
		ft_putstr("Error: value of an argument is out of range\n");
		return (0);
	}
	return (1);
}
