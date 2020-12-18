/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 16:51:11 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/18 14:29:08 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_get_fork(t_philo *philo)
{
	sem_wait(philo->sem);
	sem_wait(philo->sem);
	print_status(philo, FORK_ACTION);
	print_status(philo, FORK_ACTION);
}

void	ft_drop_fork(t_philo *philo)
{
	sem_post(philo->sem);
	sem_post(philo->sem);
}

void	ft_eat(t_philo *philo)
{
	philo->start = get_current_time(1, philo->start_time) +
		(philo->die_time * 1000);
	sem_wait(philo->mtphilo);
	print_status(philo, EAT_ACTION);
	usleep(philo->eat_time * 1000);
	sem_post(philo->mtphilo);
	if (philo->eat_num != -1)
		philo->eat_num--;
}

void	ft_sleep(t_philo *philo)
{
	print_status(philo, SLEEP_ACTION);
	usleep(philo->sleep_time * 1000);
	print_status(philo, THINK_ACTION);
}
