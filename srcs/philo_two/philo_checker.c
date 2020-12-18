/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 16:51:11 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/18 14:03:17 by zlayine          ###   ########.fr       */
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
		if (philo->die == 0 && (philo->eat_num || philo->eat_num == -1) && get_current_time(1,
			philo->start_time) > philo->start)
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
	me->start = get_current_time(1, me->start_time) + (me->die_time * 1000);
	pthread_create(&checker, NULL, &ft_philo_checker, (void *)me);
	me->checker = checker;
	while (1)
	{
		ft_get_fork(me);
		ft_eat(me);
		ft_drop_fork(me);
		ft_sleep(me);
		if (me->eat_num == 0)
		{
			me->done = 1;
			break ;
		} else if (me->die)
			break ;
	}
	return (NULL);
}

void	create_lifes(t_table *table)
{
	int				i;
	t_philo			*tmp;
	pthread_t		checker;
	struct timeval	current_time;
	pthread_t		tids[table->persons];

	i = 0;
	tmp = table->philos;
	pthread_create(&checker, NULL, &game_checker, (void *)table);
	pthread_detach(checker);
	sem_wait(table->mtdie);
	gettimeofday(&current_time, NULL);
	while (tmp)
	{
		tmp->start_time = current_time;
		pthread_create(&tmp->thrd, NULL, &ft_philo_life, (void *)tmp);
		usleep(45);
		tmp = tmp->next;
		tids[i++] = tmp->thrd;
		if (tmp->head)
			break ;
	}
	sem_wait(table->mtdie);
	sem_post(table->mtdie);
}