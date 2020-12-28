/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 14:25:37 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/28 15:03:15 by zlayine          ###   ########.fr       */
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
	while (1)
	{
		if (philo && philo->eat_num == 0 && philo->done && philo->die != -1)
		{
			done++;
			philo->die = -1;
		}
		if (done == table->persons || !philo)
			break ;
		philo = philo->next;
	}
	if (!philo)
		return (NULL);
	print_status(philo, SIM_OVER);
	pthread_mutex_unlock(table->mtdie);
	return (NULL);
}

void	*ft_philo_checker(void *arg)
{
	t_philo			*philo;
	pthread_mutex_t	*mtdie;

	philo = (t_philo*)arg;
	mtdie = philo->table->mtdie;
	while (philo->die != -1)
	{
		if (pthread_mutex_lock(philo->mtphilo) < 0)
			break ;
		if (philo->die != -2 && philo->eat_num != 0 &&
			get_time() > philo->death_time)
		{
			philo->die = 1;
			print_status(philo, DIE_ACTION);
			pthread_mutex_unlock(philo->mtphilo);
			pthread_mutex_unlock(mtdie);
			break ;
		}
		pthread_mutex_unlock(philo->mtphilo);
		usleep(100);
	}
	return (NULL);
}

void	*ft_philo_life(void *arg)
{
	t_philo			*me;
	pthread_t		checker;

	me = (t_philo*)arg;
	me->start = get_time();
	me->death_time = me->start + me->die_time;
	pthread_create(&checker, NULL, &ft_philo_checker, (void *)me);
	pthread_detach(checker);
	while (1)
	{
		ft_get_fork(me);
		ft_eat(me);
		ft_finish_eat(me);
		if (me->eat_num == 0)
		{
			me->done = 1;
			break ;
		}
	}
	return (NULL);
}

void	create_lifes(t_table *table)
{
	t_philo			*tmp;
	pthread_t		thrd;

	tmp = table->philos;
	if (tmp->eat_time != -1)
	{
		pthread_create(&thrd, NULL, &game_checker, (void *)table);
		pthread_detach(thrd);
	}
	pthread_mutex_lock(table->mtdie);
	while (tmp)
	{
		pthread_create(&thrd, NULL, &ft_philo_life, (void *)tmp);
		pthread_detach(thrd);
		usleep(100);
		tmp = tmp->next;
		if (tmp->head)
			break ;
	}
	pthread_mutex_lock(table->mtdie);
	pthread_mutex_unlock(table->mtdie);
}
