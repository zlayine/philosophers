/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finish.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 16:51:11 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/15 19:03:09 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_simulation(t_philo *curr, int total)
{
	t_philo			*tmp;
	pthread_mutex_t	*mutex;
	pthread_mutex_t	*print;
	int				i;

	i = -1;
	mutex = curr->mutex;
	print = curr->print;
	curr->prev->next = NULL;
	while (curr)
	{
		pthread_detach(curr->thrd);
		pthread_detach(curr->checker);
		tmp = curr->next;
		ft_del(curr);
		curr = tmp;
	}
	while (++i < total)
		pthread_mutex_destroy(&mutex[i]);
	pthread_mutex_destroy(print);
}

void	finish_simulation(t_table *table, int death)
{
	t_philo	*curr;
	t_philo	*tmp;
	int		i;

	curr = table->philos;
	free_simulation(curr, table->persons);
	ft_del(table);
	ft_putstr("End of simulation: ");
	if (death)
		ft_putstr("one of the philosophers died\n");
	else
		ft_putstr("philosophers reached the eat limit\n");
	exit(0);
}

