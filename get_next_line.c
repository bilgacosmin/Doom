/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbilga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 14:28:47 by cbilga            #+#    #+#             */
/*   Updated: 2018/12/03 11:07:23 by cbilga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static t_line	*select_fd(t_line *l, int fd)
{
	t_line	*link;

	link = l;
	while (l != NULL)
	{
		if (l->fd == fd)
			return (l);
		link = l;
		l = l->next;
	}
	if (!(l = (t_line*)malloc(sizeof(t_line))))
		return (NULL);
	if (!(l->buff = (char*)malloc(sizeof(char) * (BUFF_SIZE + 1))))
		return (NULL);
	l->buff[0] = '\0';
	l->fd = fd;
	l->next = NULL;
	if (link != NULL)
		link->next = l;
	return (l);
}

static char		*cut_buff(char **buff)
{
	int		i;
	int		j;
	char	*s;

	i = 0;
	while ((*buff)[i] && ((*buff)[i] != '\n'))
		i++;
	j = 0;
	if (!(s = (char*)malloc(sizeof(char) * (i + 1))))
		return (NULL);
	while (j < i)
	{
		s[j] = (*buff)[j];
		j++;
	}
	s[j] = '\0';
	j = -1;
	if ((*buff)[i] == '\0')
		i--;
	while ((*buff)[++j + i + 1])
		(*buff)[j] = (*buff)[j + i + 1];
	(*buff)[j] = '\0';
	return (s);
}

static t_line	*rem_fd(t_line *list, int fd)
{
	t_line	*prev;
	t_line	*curr;

	prev = NULL;
	curr = list;
	while (curr)
	{
		if (curr->fd == fd)
		{
			free(curr->buff);
			if (prev == NULL)
				list = curr->next;
			else
				prev->next = curr->next;
			free(curr);
		}
		prev = curr;
		curr = curr->next;
	}
	return (list);
}

static int		get_next_line_aux(t_line *l)
{
	int		bool;
	int		size;
	char	*tofree;
	char	*tofree2;

	bool = 1;
	size = 1;
	while (bool)
	{
		if (l->buff[0] == '\0')
		{
			size = read(l->fd, l->buff, BUFF_SIZE);
			if (size < 0)
				return (-1);
			l->buff[size] = '\0';
		}
		if (ft_strchr(l->buff, '\n') != NULL || size <= 0)
			bool = 0;
		tofree = l->line;
		tofree2 = cut_buff(&(l->buff));
		l->line = ft_strjoin(l->line, tofree2);
		free(tofree);
		free(tofree2);
	}
	return (size);
}

int				get_next_line(const int fd, char **line)
{
	static t_line	*list;
	t_line			*l;
	int				size;

	if (fd >= 0 && BUFF_SIZE > 0)
		l = select_fd(list, fd);
	else
		return (-1);
	if (list == NULL)
		list = l;
	if (!(l->line = malloc(sizeof(char))))
		return (-1);
	l->line[0] = '\0';
	size = get_next_line_aux(l);
	if (size == -1)
		return (-1);
	*line = l->line;
	if (l->line[0] == '\0' && size == 0)
	{
		list = rem_fd(list, fd);
		return (0);
	}
	return (1);
}
