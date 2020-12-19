/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 14:38:30 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/16 09:37:42 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*game_checker(void *arg)
{
	t_table	*table;
	t_philo *philo;
	sem_t	*donemt;
	int		done;

	table = (t_table*)arg;
	philo = table->philos;
	done = 0;
	donemt = philo->done;
	while (1)
	{
		sem_wait(donemt);
		done++;
		philo = philo->next;
		if (!philo || done == table->persons)
			break ;
	}
	if (!philo)
		return (NULL);
	print_status(philo, SIM_OVER);
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
		if (sem_wait(philo->mtphilo) < 0)
			break ;
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
		usleep(1000);
	}
	return (NULL);
}

int		ft_philo_life(t_philo *me)
{
	pthread_t	checker;
	int			pid;

	pid = fork();
	if (pid == 0)
	{
		me->start = get_time();
		me->death_time = me->start + me->die_time;
		pthread_create(&checker, NULL, &ft_philo_checker, (void *)me);
		pthread_detach(checker);
		while (1)
		{
			ft_get_fork(me);
			ft_eat(me);
			ft_finish_eat(me);
			if (me->eat_num == 0 || me->die)
				break ;
		}
		exit(0);
	}
	return (pid);
}

void	life_creation_end(t_table *table, pid_t *pids)
{
	int			i;
	pthread_t	checker;

	i = -1;
	sem_wait(table->mtdie);
	sem_post(table->mtdie);
	while (++i < table->persons)
		kill(pids[i], SIGKILL);
}

void	create_lifes(t_table *table)
{
	int				i;
	int				pid;
	t_philo			*tmp;
	pid_t			pids[table->persons];

	i = 0;
	tmp = table->philos;
	table->start = get_time();
	if (tmp->eat_num != -1)
	{
		pthread_create(&table->checker, NULL, &game_checker, (void *)table);
		pthread_detach(table->checker);
	}
	sem_wait(table->mtdie);
	while (tmp)
	{
		pid = ft_philo_life(tmp);
		if (pid < 0)
			exit(1);
		tmp = tmp->next;
		pids[i++] = pid;
		if (tmp->head)
			break ;
	}
	life_creation_end(table, pids);
}
