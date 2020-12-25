/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 16:51:11 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/25 14:25:30 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo		*init_philo(int name, t_philo *prev, char **args, int argc)
{
	t_philo	*philo;
	char	*strname;
	char	*tmp;

	philo = malloc(sizeof(t_philo));
	philo->die_time = ft_atoi(args[1]);
	philo->eat_time = ft_atoi(args[2]);
	philo->sleep_time = ft_atoi(args[3]);
	philo->eat_num = argc == 6 ? ft_atoi(args[4]) : -1;
	philo->name = name;
	strname = ft_itoa(name);
	tmp = ft_strjoin("sem_ph_", strname);
	philo->mtphilo = init_semaphore(1, tmp);
	ft_del(strname);
	ft_del(tmp);
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

t_philo		*create_philos(int total, t_table *table, char **args, int argc)
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
		tmp = init_philo(i + 1, tmp, args, argc);
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

t_table		*init_table(char **args, int argc)
{
	t_table	*table;
	int		i;

	i = 0;
	table = malloc(sizeof(t_table));
	table->persons = atoi(args[0]);
	table->forks = atoi(args[0]);
	table->end = 0;
	table->start = get_time();
	table->mtdie = init_semaphore(1, "sem_game");
	table->philos = create_philos(table->persons, table, args, argc);
	return (table);
}

sem_t		*init_semaphore(int total, char *name)
{
	sem_t	*sem;

	sem_unlink(name);
	sem = sem_open(name, O_CREAT, 0666, total);
	return (sem);
}
