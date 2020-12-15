/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 16:51:11 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/15 18:28:00 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo		*init_philo(int name, t_philo *prev, char **args)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	philo->die_time = ft_atoi(args[1]);
	philo->eat_time = ft_atoi(args[2]);
	philo->sleep_time = ft_atoi(args[3]);
	philo->eat_num = args[4] ? ft_atoi(args[4]) : -1;
	philo->name = name;
	philo->head = 0;
	philo->start = 0;
	philo->die = 0;
	philo->done = 0;
	philo->next = NULL;
	philo->prev = prev;
	if (prev)
		prev->next = philo;
	return (philo);
}

t_philo		*create_philos(int total, t_table *table, char **args)
{
	t_philo			*head;
	t_philo			*tmp;
	sem_t			*sem;
	sem_t			*print;
	int				i;

	i = 0;
	head = NULL;
	tmp = NULL;
	sem = init_semaphore(table->forks, "table_sem");
	print = init_semaphore(1, "print_sem");
	while (i < total)
	{
		tmp = init_philo(i + 1, tmp, args);
		tmp->table = table;
		tmp->sem = sem;
		tmp->print = print;
		if (!head)
			head = tmp;
		i++;
	}
	head->prev = tmp;
	tmp->next = head;
	head->head = 1;
	return (head);
}

t_table		*init_table(char **args)
{
	t_table	*table;
	int		i;

	i = 0;
	table = malloc(sizeof(t_table));
	table->persons = atoi(args[0]);
	table->forks = atoi(args[0]);
	table->end = 0;
	table->philos = create_philos(table->persons, table, args);
	return (table);
}

sem_t		*init_semaphore(int total, char *name)
{
	sem_t	*sem;

	sem_unlink(name);
	sem = sem_open(name, O_CREAT, 0666, total);
	return (sem);
}
