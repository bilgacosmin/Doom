/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbilga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 10:13:15 by cbilga            #+#    #+#             */
/*   Updated: 2018/11/15 10:24:59 by cbilga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_last_char_index(char const *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	i--;
	while ((s[i] == ' ' || s[i] == '\t' || s[i] == '\n') && (i >= 0))
		i--;
	return (i);
}

char		*ft_strtrim(char const *s)
{
	int		i;
	int		li;
	char	*res;
	int		j;

	if (s == NULL)
		return (NULL);
	i = 0;
	while (s[i] == ' ' || s[i] == '\t' || s[i] == '\n')
		i++;
	if (s[i] == '\0')
		i = 0;
	li = get_last_char_index(s);
	res = (char*)malloc(sizeof(char) * (li - i + 2));
	if (res == NULL)
		return (NULL);
	j = 0;
	while (i <= li)
	{
		res[j] = s[i];
		j++;
		i++;
	}
	res[j] = '\0';
	return (res);
}
