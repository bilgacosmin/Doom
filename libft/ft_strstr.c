/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbilga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/08 15:51:53 by cbilga            #+#    #+#             */
/*   Updated: 2018/11/16 10:48:46 by cbilga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *haystack, const char *needle)
{
	int i;
	int j;
	int k;

	i = 0;
	if (needle[0] == '\0')
		return ((char*)haystack);
	while (haystack[i])
	{
		j = 0;
		if (haystack[i] == needle[j])
		{
			k = i;
			while ((haystack[k] == needle[j]) && (haystack[k] != 0))
			{
				k++;
				j++;
			}
			if (needle[j] == '\0')
				return ((char*)haystack + i);
		}
		i++;
	}
	return (NULL);
}
