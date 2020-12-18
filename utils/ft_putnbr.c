/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 13:35:03 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/18 13:35:13 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	ft_putnbr(long long d)
{
	if (d < 0)
	{
		ft_putchar('-');
		d = d * -1;
	}
	if (d / 10)
		ft_putnbr(d / 10);
	ft_putchar(d % 10 + 48);
}