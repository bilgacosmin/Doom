/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbilga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 13:33:55 by cbilga            #+#    #+#             */
/*   Updated: 2018/11/15 11:34:50 by cbilga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	int	nbdec;

	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		if (n == -2147483648)
		{
			ft_putchar_fd('2', fd);
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
		ft_putchar_fd((n / nbdec) + 48, fd);
		n = n % nbdec;
		nbdec = nbdec / 10;
	}
}
