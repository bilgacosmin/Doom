/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbilga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 16:27:28 by cbilga            #+#    #+#             */
/*   Updated: 2018/11/16 12:58:08 by cbilga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr(int n)
{
	int	nbdec;

	if (n < 0)
	{
		ft_putchar('-');
		if (n == -2147483648)
		{
			ft_putchar('2');
			n = 147483648;
		}
		else
			n = -n;
	}
	nbdec = 1;
	while ((n / nbdec) > 9)
		nbdec = nbdec * 10;
	while (nbdec != 0)
	{
		ft_putchar((n / nbdec) + 48);
		n = n % nbdec;
		nbdec = nbdec / 10;
	}
}
