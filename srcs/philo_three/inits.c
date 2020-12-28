/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 14:38:33 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/28 15:00:20 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo		*init_philo(int name, t_philo *prev, char **args, int argc)
{
	t_philo	*philo;
	char	*tmp;
	char	*strname;

	philo = malloc(sizeof(t_philo));
	philo->die_time = ft_atoi(args[1]);
	philo->eat_time = ft_atoi(args[2]);
	philo->sleep_time = ft_atoi(args[3]);
	philo->eat_num = argc == 6 ? ft_atoi(args[4]) : -1;
	philo->name = name;
	strname = ft_itoa(name);
	tmp = ft_strjoin("sem_ph_", strname);
	philo->mtphilo = init_semaphore(1, tmp);
	ft_del(tmp);
	ft_del(strname);
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

t_philo		*create_philos(int i, t_table *table, char **args, int argc)
{
	t_philo			*head;
	t_philo			*tmp;
	sem_t			*sem;
	sem_t			*print;
	sem_t			*done;

	head = NULL;
	tmp = NULL;
	sem = init_semaphore(table->forks, "table_sem");
	print = init_semaphore(1, "print_sem");
	done = init_semaphore(0, "done_sem");
	while (++i < table->forks)
	{
		tmp = init_philo(i + 1, tmp, args, argc);
		tmp->table = table;
		tmp->done = done;
		tmp->sem = sem;
		tmp->print = print;
		head = head ? head : tmp;
	}
	head->prev = tmp;
	tmp->next = head;
	head->head = 1;
	return (head);
}

t_table		*init_table(char **args, int argc)
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	table->persons = atoi(args[0]);
	table->forks = atoi(args[0]);
	table->end = 0;
	table->philos = create_philos(-1, table, args, argc);
	table->mtdie = init_semaphore(1, "game_sem");
	return (table);
}

sem_t		*init_semaphore(int total, char *name)
{
	sem_t	*sem;

	sem_unlink(name);
	sem = sem_open(name, O_CREAT, 0666, total);
	return (sem);
}
