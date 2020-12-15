/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 16:51:11 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/15 16:52:15 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_get_fork(t_philo *philo)
{
	sem_wait(philo->sem);
	print_status(philo, FORK_ACTION);
	sem_wait(philo->sem);
	print_status(philo, FORK_ACTION);
	philo->table->forks -= 2;
}

void	ft_drop_fork(t_philo *philo)
{
	sem_post(philo->sem);
	sem_post(philo->sem);
	philo->table->forks += 2;
}

void	ft_eat(t_philo *philo)
{
	philo->start = get_current_time(1, philo->start_time);
	print_status(philo, EAT_ACTION);
	usleep(philo->eat_time * 1000);
	ft_drop_fork(philo);
	if (philo->eat_num != -1)
		philo->eat_num--;
	ft_sleep(philo);
}

void	ft_sleep(t_philo *philo)
{
	print_status(philo, SLEEP_ACTION);
	usleep(philo->sleep_time * 1000);
	print_status(philo, THINK_ACTION);
}
