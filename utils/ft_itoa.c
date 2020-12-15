/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 17:14:22 by zlayine           #+#    #+#             */
/*   Updated: 2020/12/15 17:14:23 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

char	*ft_itoa(long n)
{
	char	*ptr;
	long	nbr;
	int		len;

	len = 1;
	n < 0 ? ++len : 0;
	nbr = n < 0 ? -(long)n : n;
	while (nbr > 9)
	{
		nbr /= 10;
		len++;
	}
	if (!(ptr = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	ptr[len] = '\0';
	n < 0 ? *ptr = '-' : 0;
	nbr = n < 0 ? -(long)n : n;
	while (nbr > 9)
	{
		ptr[--len] = (nbr % 10) + '0';
		nbr /= 10;
	}
	ptr[--len] = nbr + '0';
	return (ptr);
}
