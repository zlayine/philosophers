/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finish.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 16:51:11 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/15 20:54:39 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_simulation(t_philo *curr)
{
	t_philo	*tmp;

	sem_close(curr->print);
	sem_close(curr->sem);
	sem_unlink("table_sem");
	sem_unlink("print_sem");
	while (curr)
	{
		curr->die = 1;
		pthread_detach(curr->thrd);
		pthread_detach(curr->checker);
		curr->table = NULL;
		tmp = curr->next;
		ft_del(curr);
		curr = tmp;
	}
}

void	finish_simulation(t_table *table, int death)
{
	t_philo	*curr;
	t_philo	*tmp;
	sem_t	*print;
	int		i;

	i = -1;
	curr = table->philos;
	curr->prev->next = NULL;
	sem_wait(curr->print);
	ft_putstr("End of simulation: ");
	if (death)
		ft_putstr("one of the philosophers died\n");
	else
		ft_putstr("philosophers reached the eat limit\n");
	sem_post(curr->print);
	print = curr->print;
	while (table->forks < table->persons)
	{
		sem_post(curr->sem);
		table->forks++;
	}
	free_simulation(curr);
	pthread_detach(table->checker);
	ft_del(table);
	exit(0);
}
