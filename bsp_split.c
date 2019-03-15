/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbilga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/15 09:30:43 by cbilga            #+#    #+#             */
/*   Updated: 2019/03/15 18:38:35 by cbilga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsp.h"

static void	init_select_split(t_select_split *s, t_poly *poly_list)
{
	s->splitter = poly_list;
	s->current = NULL;
	s->selected = NULL;
    s->best_score = 2147483647;
}

static void	init_select_score(t_select_split *s, t_poly *poly_list)
{
	s->split_plane.normal = s->splitter->normal;
	s->split_plane.point = s->splitter->ver_list;
	s->current = poly_list;
	s->score = 0;
	s->backfaces = 0;
	s->frontfaces = 0;
	s->splits = 0;
}

static void	get_score(t_select_split *s)
{
	while(s->current !=NULL)
	{
		s->result = class_poly(&s->split_plane,s->current);
		if (result == 1)
			s->frontfaces++;
		if (result == -1)
			s->backfaces++;
		if (result == 2)
			s->splits++;
	}
	s->current = s->current->next;
	s->score = ft_abs((s->frontfaces - s->backfaces) + (s->splits * 3));
	if (s->score < s->best_score)
	{
		s->best_score = s->score;
		s->selected = s->splitter;
	}
}

int			select_splitter(t_bsp *bsp) //function choosing best splitter for a more balanced tree
{
	t_select_split	*s;

	init_select_split(s, bsp->poly);
	while(s->splitter != NULL)
	{
		if (s->splitter->was_splitter == 0)
		{
			init_select_score(s, bsp->poly);
			get_score(s);
		}
		s->splitter = s->splitter->next;
	}
	s->selected->was_splitter = 1;
	bsp->plane[bsp->nb_planes].point = s->selected->ver_list;
	bsp->plane[bsp->nb_planes].normal = s->selected->normal;
	inc_planes(bsp);
	return (bsp->nb_planes - 1);
}
