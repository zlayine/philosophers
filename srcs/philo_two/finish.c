/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finish.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 16:51:11 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/19 19:50:28 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_simulation(t_philo *curr)
{
	t_philo	*tmp;
	char	*name_tmp;
	char	*strname;

	while (curr)
	{
		strname = ft_itoa(curr->name);
		name_tmp = ft_strjoin("sem_ph_", strname);
		sem_post(curr->mtphilo);
		sem_wait(curr->mtphilo);
		curr->die = 2;
		sem_post(curr->mtphilo);
		sem_close(curr->mtphilo);
		sem_unlink(name_tmp);
		ft_del(strname);
		ft_del(name_tmp);
		tmp = curr->next;
		ft_del(curr);
		curr = NULL;
		curr = tmp;
	}
}

void	finish_simulation(t_table *table, int death)
{
	t_philo	*curr;
	sem_t	*print;
	int		i;

	i = -1;
	curr = table->philos;
	print = curr->print;
	curr->prev->next = NULL;
	while (++i < table->persons)
		sem_post(curr->sem);
	sem_close(curr->sem);
	sem_unlink("table_sem");
	free_simulation(curr);
	sem_close(print);
	sem_unlink("print_sem");
	sem_close(table->mtdie);
	sem_unlink("sem_game");
	ft_del(table);
	exit(0);
}
