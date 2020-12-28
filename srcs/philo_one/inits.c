/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 14:26:37 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/28 15:06:52 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo		*init_philo(int name, t_philo *prev, char **args, int argc)
{
	t_philo			*philo;

	philo = malloc(sizeof(t_philo));
	philo->die_time = ft_atoi(args[1]);
	philo->eat_time = ft_atoi(args[2]);
	philo->sleep_time = ft_atoi(args[3]);
	philo->eat_num = argc == 6 ? ft_atoi(args[4]) : -1;
	philo->name = name;
	philo->mtphilo = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(philo->mtphilo, NULL);
	philo->head = 0;
	philo->start = 0;
	philo->die = 0;
	philo->done = 0;
	philo->next = NULL;
	philo->prev = prev;
	philo->r_fork = name - 1;
	philo->l_fork = 0;
	if (prev)
	{
		philo->l_fork = prev->r_fork;
		prev->next = philo;
	}
	return (philo);
}

void		init_mutex(pthread_mutex_t *mutex, int total)
{
	int		i;

	i = -1;
	while (++i < total)
		pthread_mutex_init(&mutex[i], NULL);
}

t_table		*init_table(int argc, char **args)
{
	t_table			*table;

	table = malloc(sizeof(t_table));
	table->persons = atoi(args[0]);
	table->forks = atoi(args[0]);
	table->philos = create_philos(table->persons, table, args, argc);
	table->philos->head = 1;
	table->start = get_time();
	table->mtdie = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(table->mtdie, NULL);
	return (table);
}

t_philo		*create_philos(int total, t_table *table, char **args, int argc)
{
	t_philo			*head;
	t_philo			*tmp;
	pthread_mutex_t	*mutex;
	pthread_mutex_t	*print;
	int				i;

	i = -1;
	head = NULL;
	tmp = NULL;
	mutex = malloc(sizeof(pthread_mutex_t) * total);
	print = malloc(sizeof(pthread_mutex_t));
	init_mutex(mutex, total);
	pthread_mutex_init(print, NULL);
	while (++i < total)
	{
		tmp = init_philo(i + 1, tmp, args, argc);
		tmp->table = table;
		tmp->mutex = mutex;
		tmp->print = print;
		head = head ? head : tmp;
	}
	head->l_fork = tmp->r_fork;
	head->prev = tmp;
	tmp->next = head;
	return (head);
}
