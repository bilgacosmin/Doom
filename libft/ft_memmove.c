/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbilga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/08 11:07:50 by cbilga            #+#    #+#             */
/*   Updated: 2018/11/19 09:25:06 by cbilga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t	i;
	void	*aux;

	i = 0;
	if ((src < dst) && ((src + len) > dst))
	{
		if (!(aux = malloc(sizeof(char) * len)))
			return (NULL);
		ft_memcpy(aux, src, len);
		ft_memcpy(dst, aux, len);
	}
	else
	{
		while (i < len)
		{
			((char*)dst)[i] = ((char*)src)[i];
			i++;
		}
	}
	return (dst);
}
