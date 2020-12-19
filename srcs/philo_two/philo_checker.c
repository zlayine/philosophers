/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 16:51:11 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/18 14:41:26 by zlayine          ###   ########.fr       */
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
	table->end = 1;
	sem_post(table->mtdie);
	return (NULL);
}

void	*ft_philo_checker(void *arg)
{
	t_philo *philo;
	sem_t	*die;

	philo = (t_philo*)arg;
	die = philo->table->mtdie;
	while (1)
	{
		sem_wait(philo->mtphilo);
		if (philo->die == 0 && philo->eat_num != 0
			&& get_time() > philo->death_time)
		{
			philo->die = 1;
			print_status(philo, DIE_ACTION);
			sem_post(philo->mtphilo);
			sem_post(die);
			break ;
		}
		sem_post(philo->mtphilo);
		usleep(5);
	}
	return (NULL);
}

void	*ft_philo_life(void *arg)
{
	t_philo		*me;
	pthread_t	checker;

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
		// if (me->eat_num == 0)
		// {
		// 	me->done = 1;
		// 	break ;
		// }
		// else if (me->die)
		// 	break ;
	}
	return (NULL);
}

void	create_lifes(t_table *table)
{
	t_philo			*tmp;
	pthread_t		thrd;
	struct timeval	current_time;

	tmp = table->philos;
	if (tmp->eat_time != -1)
	{
		pthread_create(&thrd, NULL, &game_checker, (void *)table);
		pthread_detach(thrd);
	}
	sem_wait(table->mtdie);
	gettimeofday(&current_time, NULL);
	while (tmp)
	{
		tmp->start_time = current_time;
		pthread_create(&thrd, NULL, &ft_philo_life, (void *)tmp);
		pthread_detach(thrd);
		// usleep(50);
		tmp = tmp->next;
		if (tmp->head)
			break ;
	}
	sem_wait(table->mtdie);
	sem_post(table->mtdie);
}
