/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbilga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 10:38:41 by cbilga            #+#    #+#             */
/*   Updated: 2018/11/19 09:22:19 by cbilga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	nb_chaines(char const *s, char c)
{
	int	i;

	i = 0;
	while (*s == c)
		s++;
	while (*s)
	{
		while ((*s != c) && (*s))
			s++;
		while (*s == c)
			s++;
		i++;
	}
	return (i + 1);
}

static int	l_chaine(char const *s, char c, int i)
{
	int l;

	l = 0;
	while ((s[i] != c) && (s[i]))
	{
		i++;
		l++;
	}
	return (l);
}

static int	ft_strcpyx(char *dest, const char *src, int i, char c)
{
	int	j;

	j = 0;
	while ((src[i]) && (src[i] != c))
	{
		dest[j] = src[i];
		j++;
		i++;
	}
	dest[j] = '\0';
	return (i);
}

static char	**ft_freetab(char **res, int m)
{
	int	i;

	i = 0;
	while (i < m)
	{
		if (res[i] != NULL)
			free(res[i]);
		i++;
	}
	res = NULL;
	return (NULL);
}

char		**ft_strsplit(char const *s, char c)
{
	char	**res;
	int		i;
	int		l;
	int		m;

	if (s == NULL || !c)
		return (NULL);
	if (!(res = (char**)malloc(sizeof(char*) * nb_chaines(s, c))))
		return (NULL);
	i = 0;
	m = 0;
	while (s[i] == c)
		i++;
	while (s[i])
	{
		l = l_chaine(s, c, i);
		if (!(res[m] = (char*)malloc(sizeof(char) * (l + 1))))
			return (ft_freetab(res, m));
		i = ft_strcpyx(res[m], s, i, c);
		while ((s[i] == c) && (s[i]))
			i++;
		m++;
	}
	res[m] = NULL;
	return (res);
}
