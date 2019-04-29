/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbilga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 14:49:33 by cbilga            #+#    #+#             */
/*   Updated: 2018/11/14 13:38:01 by cbilga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_aux(char *res, int n, int i)
{
	int nbdec;

	nbdec = 1;
	while ((n / nbdec) > 9)
		nbdec = nbdec * 10;
	while (nbdec != 0)
	{
		res[i] = (n / nbdec) + 48;
		i++;
		n = n % nbdec;
		nbdec = nbdec / 10;
	}
	res[i] = '\0';
}

char		*ft_itoa(int n)
{
	int		i;
	char	*res;

	res = (char*)malloc(sizeof(char) * 12);
	if (res == NULL)
		return (NULL);
	i = 0;
	if (n < 0)
	{
		res[0] = '-';
		if (n == -2147483648)
		{
			res[1] = '2';
			n = 147483648;
			i++;
		}
		else
			n = -n;
		i++;
	}
	ft_aux(res, n, i);
	return (res);
}
