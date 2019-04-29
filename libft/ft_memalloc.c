/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbilga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/09 13:51:04 by cbilga            #+#    #+#             */
/*   Updated: 2018/11/16 10:54:45 by cbilga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memalloc(size_t size)
{
	size_t	i;
	void	*res;

	i = 0;
	res = (void*)malloc(sizeof(char) * size);
	if (res == NULL)
		return (NULL);
	ft_memset(res, 0, size);
	return (res);
}
