/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbilga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/08 12:31:03 by cbilga            #+#    #+#             */
/*   Updated: 2018/11/16 10:45:59 by cbilga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t i;

	i = 0;
	while (i < n)
	{
		if (((char*)s1)[i] != ((char*)s2)[i])
			return ((int)(((unsigned char*)s1)[i] - ((unsigned char*)s2)[i]));
		i++;
	}
	return (0);
}
