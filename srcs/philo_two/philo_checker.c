/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 16:51:11 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/15 20:47:47 by zlayine          ###   ########.fr       */
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
	while (!table->end)
	{
		if (philo && philo->eat_num == 0 && philo->done && philo->die != -1)
		{
			done++;
			philo->die = -1;
		}
		else if (table->end)
			break ;
		philo = philo->next;
		if (done == table->persons)
			break ;
	}
	finish_simulation(philo->table, table->end);
	return (NULL);
}

void	*ft_philo_checker(void *arg)
{
	t_philo *philo;

	philo = (t_philo*)arg;
	while (philo && philo->table)
	{
		if (!philo || philo->die)
			break ;
		if ((philo->eat_num || philo->eat_num == -1) && get_current_time(1,
			philo->start_time) > philo->start + (philo->die_time * 1000))
		{
			philo->die = 1;
			sem_wait(philo->print);
			philo->table->end = !philo->table->end ?
				philo->name : philo->table->end;
			print_status(philo, DIE_ACTION);
			sem_post(philo->print);
			break ;
		}
	}
	return (NULL);
}

void	*ft_philo_life(void *arg)
{
	t_philo		*me;
	pthread_t	checker;

	me = (t_philo*)arg;
	me->start = get_current_time(1, me->start_time);
	pthread_create(&checker, NULL, &ft_philo_checker, (void *)me);
	me->checker = checker;
	while (1)
	{
		ft_get_fork(me);
		ft_eat(me);
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
	int				i;
	t_philo			*tmp;
	pthread_t		checker;
	struct timeval	current_time;
	pthread_t		tids[table->persons];

	i = 0;
	tmp = table->philos;
	gettimeofday(&current_time, NULL);
	pthread_create(&checker, NULL, &game_checker, (void *)table);
	while (tmp)
	{
		tmp->start_time = current_time;
		pthread_create(&tmp->thrd, NULL, &ft_philo_life, (void *)tmp);
		tmp = tmp->next;
		tids[i++] = tmp->thrd;
		if (tmp->head)
			break ;
	}
	i = 0;
	table->checker = checker;
	pthread_join(checker, NULL);
}
