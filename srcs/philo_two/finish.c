/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finish.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 16:51:11 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/18 14:05:07 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


void	free_simulation(t_philo *curr)
{
	t_philo	*tmp;
	sem_t	*print;
	char	*name_tmp;
	int		i;

	print = curr->print;
	sem_close(curr->sem);
	sem_unlink("table_sem");
	while (curr)
	{
		name_tmp = ft_strjoin("sem_ph_", ft_itoa(curr->name));
		sem_post(curr->mtphilo);
		sem_wait(curr->mtphilo);
		curr->die = 2;
		sem_post(curr->mtphilo);
		sem_close(curr->mtphilo);
		sem_unlink(name_tmp);
		pthread_detach(curr->checker);
		ft_del(name_tmp);
		tmp = curr->next;
		ft_del(curr);
		curr = NULL;
		curr = tmp;
	}
	i = -1;
	while (++i < 3)
		sem_post(print);
	sem_close(print);
	sem_unlink("print_sem");
}

void	finish_simulation(t_table *table, int death)
{
	t_philo	*curr;
	int		i;

	i = -1;
	curr = table->philos;
	curr->prev->next = NULL;
	while (++i < table->persons)
		sem_post(curr->sem);
	free_simulation(curr);
	sem_close(table->mtdie);
	sem_unlink("sem_game");
	ft_del(table);
	exit(0);
}