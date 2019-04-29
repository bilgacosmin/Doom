/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbilga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/15 12:25:02 by cbilga            #+#    #+#             */
/*   Updated: 2018/12/03 08:16:49 by cbilga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# define BUFF_SIZE 12
# include <string.h>
# include <stdlib.h>
# include <fcntl.h>
# include "libft/includes/libft.h"
# include <stdio.h>

typedef struct s_line	t_line;
struct					s_line
{
	int				fd;
	char			*line;
	char			*buff;
	struct s_line	*next;
};

int						get_next_line(const int fd, char **line);
#endif
