/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finish.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 16:51:11 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/28 15:02:33 by zlayine          ###   ########.fr       */
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
	pthread_mutex_destroy(print);
	ft_del(print);
	while (++i < total)
		pthread_mutex_destroy(&mutex[i]);
	ft_del(mutex);
	while (curr)
	{
		curr->die = 1;
		pthread_mutex_destroy(curr->mtphilo);
		ft_del(curr->mtphilo);
		tmp = curr->next;
		ft_del(curr);
		curr = tmp;
	}
}

void	finish_simulation(t_table *table)
{
	t_philo	*curr;

	curr = table->philos;
	free_simulation(curr, table->persons);
	ft_del(table);
	exit(0);
}
