/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 14:38:30 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/15 17:31:45 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*game_checker(void *arg)
{
	t_table	*table;
	t_philo *philo;
	int		done;

	table = (t_table*)arg;
	philo = table->philos;
	done = 0;
	while (done < table->persons || table->end)
	{
		if (!philo)
			break ;
		sem_wait(table->philos->done);
		done++;
		philo = philo->next;
	}
	table->end = 2;
	sem_post(table->game);
	finish_simulation(table, !philo);
	return (NULL);
}

void	*ft_philo_checker(void *arg)
{
	t_philo *philo;

	philo = (t_philo*)arg;
	while (philo)
	{
		if ((philo->eat_num || philo->eat_num == -1) && get_current_time(1,
			philo->start_time) > philo->start + (philo->die_time * 1000))
		{
			philo->die = 1;
			sem_wait(philo->print);
			print_status(philo, DIE_ACTION);
			break ;
		}
		else if (philo->eat_num == 0 && philo->die == -1)
			break ;
	}
	if (philo->die == 1)
		sem_post(philo->table->game);
	else if (philo->eat_num == 0)
		sem_post(philo->done);
	return (NULL);
}

void	ft_philo_life(t_philo *me)
{
	pthread_t	checker;

	me->start = get_current_time(1, me->start_time);
	pthread_create(&checker, NULL, &ft_philo_checker, (void *)me);
	me->checker = checker;
	while (me)
	{
		ft_get_fork(me);
		ft_eat(me);
		if (me->eat_num == 0)
		{
			me->die = -1;
			break ;
		}
		if (me->die || me->table->end)
			break ;
	}
	pthread_join(checker, NULL);
	exit(0);
}
