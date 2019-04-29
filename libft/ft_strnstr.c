/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbilga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/08 16:27:12 by cbilga            #+#    #+#             */
/*   Updated: 2018/11/16 10:49:07 by cbilga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	int		i;
	int		j;
	int		k;

	i = 0;
	if (*needle == '\0')
		return ((char*)haystack);
	while (haystack[i] && (len > 0))
	{
		j = 0;
		if (haystack[i] == needle[j])
		{
			k = i;
			while ((haystack[k++] == needle[j]) && (len-- > 0) &&
					(needle[j] != '\0'))
				j++;
			len = len + j;
			if (needle[j] == '\0')
				return ((char*)haystack + i);
		}
		i++;
		len--;
	}
	return (NULL);
}
