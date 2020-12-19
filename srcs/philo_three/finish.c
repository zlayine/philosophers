/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finish.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 14:52:29 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/19 20:03:09 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_simulation(t_philo *curr)
{
	t_philo	*tmp;
	char	*name_tmp;
	char	*strname;

	sem_close(curr->sem);
	sem_post(curr->done);
	sem_close(curr->done);
	sem_unlink("table_sem");
	sem_unlink("done_sem");
	while (curr)
	{
		strname = ft_itoa(curr->name);
		name_tmp = ft_strjoin("sem_ph_", strname);
		sem_post(curr->mtphilo);
		sem_close(curr->mtphilo);
		curr->die = 1;
		sem_unlink(name_tmp);
		ft_del(strname);
		ft_del(name_tmp);
		tmp = curr->next;
		ft_del(curr);
		curr = NULL;
		curr = tmp;
	}
}

void	finish_simulation(t_table *table)
{
	t_philo	*curr;
	t_philo	*tmp;
	sem_t	*print;

	curr = table->philos;
	print = curr->print;
	curr->prev->next = NULL;
	while (table->forks++ < table->persons)
		sem_post(curr->sem);
	free_simulation(curr);
	sem_close(print);
	sem_unlink("print_sem");
	sem_close(table->mtdie);
	sem_unlink("sem_game");
	ft_del(table);
	exit(0);
}
