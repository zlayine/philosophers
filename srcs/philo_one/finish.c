/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finish.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 16:51:11 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/18 13:50:59 by zlayine          ###   ########.fr       */
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
	while (++i < total)
		pthread_mutex_destroy(&mutex[i]);
	while (curr)
	{
		curr->die = 1;
		pthread_mutex_destroy(curr->mtphilo);
		tmp = curr->next;
		ft_del(curr);
		curr = tmp;
	}
}

void	finish_simulation(t_table *table, int death)
{
	t_philo	*curr;
	t_philo	*tmp;
	int		i;

	curr = table->philos;
	free_simulation(curr, table->persons);
	ft_del(table);
	exit(0);
}
