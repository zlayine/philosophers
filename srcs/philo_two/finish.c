/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finish.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 16:51:11 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/18 09:30:35 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_simulation(t_philo *curr)
{
	t_philo	*tmp;
	sem_t	*print;

	print = curr->print;
	sem_close(curr->sem);
	sem_unlink("table_sem");
	sem_unlink("print_sem");
	while (curr)
	{
		curr->die = 1;
		pthread_detach(curr->thrd);
		pthread_detach(curr->checker);
		tmp = curr->next;
		ft_del(curr);
		curr = tmp;
	}
	sem_post(print);
	sem_close(print);
}

void	finish_simulation(t_table *table, int death)
{
	t_philo	*curr;
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
	pthread_detach(table->checker);
	while (++i < table->persons)
		sem_post(curr->sem);
	free_simulation(curr);
	ft_del(table);
	exit(0);
}
