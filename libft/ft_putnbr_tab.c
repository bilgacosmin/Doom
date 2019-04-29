/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_tab.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbilga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 14:02:30 by cbilga            #+#    #+#             */
/*   Updated: 2018/11/16 14:12:44 by cbilga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_tab(int *tab, size_t len)
{
	size_t	i;

	i = 0;
	if (tab == NULL)
		return ;
	while (i < len)
	{
		ft_putnbr(tab[i]);
		ft_putchar('\n');
		i++;
	}
}
