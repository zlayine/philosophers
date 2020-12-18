/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 14:24:50 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/18 13:51:13 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


void	ft_get_fork(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutex[philo->l_fork]);
	pthread_mutex_lock(&philo->mutex[philo->r_fork]);
	print_status(philo, FORK_ACTION);
	print_status(philo, FORK_ACTION);
}

void	ft_drop_fork(t_philo *philo)
{
	pthread_mutex_unlock(&philo->mutex[philo->r_fork]);
	pthread_mutex_unlock(&philo->mutex[philo->l_fork]);
}

void	ft_eat(t_philo *philo)
{
	philo->start = get_current_time(1, philo->start_time) + (philo->die_time * 1000);
	pthread_mutex_lock(philo->mtphilo);
	print_status(philo, EAT_ACTION);
	usleep(philo->eat_time * 1000);
	pthread_mutex_unlock(philo->mtphilo);
	if (philo->eat_num != -1)
		philo->eat_num--;
}

void	ft_sleep(t_philo *philo)
{
	print_status(philo, SLEEP_ACTION);
	usleep(philo->sleep_time * 1000);
	print_status(philo, THINK_ACTION);
}
