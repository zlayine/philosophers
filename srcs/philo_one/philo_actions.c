/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 14:24:50 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/25 14:20:38 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_get_fork(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutex[philo->l_fork]);
	print_status(philo, FORK_ACTION);
	pthread_mutex_lock(&philo->mutex[philo->r_fork]);
	print_status(philo, FORK_ACTION);
}

void	ft_eat(t_philo *philo)
{
	pthread_mutex_lock(philo->mtphilo);
	philo->die = -2;
	philo->start = get_time();
	philo->death_time = philo->start + philo->die_time;
	print_status(philo, EAT_ACTION);
	usleep(philo->eat_time * 1000);
	if (philo->eat_num != -1)
		philo->eat_num--;
	philo->die = 0;
	pthread_mutex_unlock(philo->mtphilo);
}

void	ft_finish_eat(t_philo *philo)
{
	pthread_mutex_unlock(&philo->mutex[philo->l_fork]);
	pthread_mutex_unlock(&philo->mutex[philo->r_fork]);
	print_status(philo, SLEEP_ACTION);
	usleep(philo->sleep_time * 1000);
	print_status(philo, THINK_ACTION);
}
