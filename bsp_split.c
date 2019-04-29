 /* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbilga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/15 09:30:43 by cbilga            #+#    #+#             */
/*   Updated: 2019/03/21 16:05:27 by cbilga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsp.h"

static void init_split(t_split *split)
{
	split->first = NULL;
	split->normal = NULL;
	split->intersect = NULL;
	split->a = NULL;
	split->b = NULL;
	split->f_count = 0;
	split->b_count = 0;
	split->loop = 1;
	split->curr_ver = 0;
}

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
	s->split_plane.point = s->splitter->ver_list[0];
	s->current = poly_list;
	s->score = 0;
	s->backfaces = 0;
	s->frontfaces = 0;
	s->splits = 0;
}

static void	get_score(t_select_split *s)
{
	//printf("GET SCORE\n");
	while(s->current != NULL)
	{
	//	printf("BUCLE\n");
		s->result = class_poly(&s->split_plane,s->current);
	//	printf("BUCLE\n");
		if (s->result == 1)
			s->frontfaces++;
		if (s->result == -1)
			s->backfaces++;
		if (s->result == 2)
			s->splits++;
		s->current = s->current->next;
	//	printf("BUCLE\n");
	}
	//printf("GET SCORE\n");
	//s->current = s->current->next;
	s->score = ft_abs((s->frontfaces - s->backfaces) + (s->splits * 3));
	if (s->score < s->best_score)
	{
		s->best_score = s->score;
		s->selected = s->splitter;
	}
	//printf("FIN GET SCORE\n");
}

int			select_splitter(t_bsp *bsp) //function choosing best splitter for a more balanced tree
{
	t_select_split	s;

	printf("SELECT SPLIT\n");
	init_select_split(&s, bsp->poly_list);
	while(s.splitter != NULL)
	{
		//printf("QUOI??\n");
		if (s.splitter->was_splitter == 0)
		{
			init_select_score(&s, bsp->poly_list);
			get_score(&s);
		}
		s.splitter = s.splitter->next;
	}
	if (s.selected == NULL) //si ily a plus de poly qui peuvent etre splitters
		return (-1);
	//printf("SELECT SPLIT\n");
	s.selected->was_splitter = 1;
	//printf("SELECT SPLIT22\n");
	bsp->plane[bsp->nb_planes].point = s.selected->ver_list[0];
	bsp->plane[bsp->nb_planes].normal = s.selected->normal;
	inc_planes(bsp);
	printf("END SELECT SPLIT\n");
	return (bsp->nb_planes - 1);
}

void		split_poly(t_poly *poly, t_plane *plane, t_poly *front, t_poly *back)
{
	t_split split;

	//printf("SPLIT POLY\n");
	init_split(&split);
	//printf("HOP\n");
	split.point = &plane->point;
	split.first = &poly->ver_list[0];
	split.class = class_point(split.first, plane);
	//printf("HOP\n");
	if (split.class == 1)
		split.front[split.f_count++] = *split.first;
	if (split.class == -1)
		split.back[split.b_count++] = *split.first;
	if (split.class == 0)
	{
		split.front[split.f_count++] = *split.first;
		split.back[split.b_count++] = *split.first;
	}
	while (split.loop < poly->nb_ver + 1) //+1?
	{
		//printf("HOP\n");
		if (split.loop == poly->nb_ver)
			split.curr_ver = 0;
		else
			split.curr_ver = split.loop;
		split.a = &poly->ver_list[split.loop - 1];
		split.b = &poly->ver_list[split.curr_ver];
		split.normal = &plane->normal;
		split.class = class_point(split.b, plane);
		//printf("HOP\n");
		if (split.class == 0)
		{
			split.back[split.b_count++] = poly->ver_list[split.curr_ver];
			split.front[split.f_count++] = poly->ver_list[split.curr_ver];
			//printf("HOP\n");
		}
		else
		{
			//printf("HOP\n");
			init_inter(&(split.inter), &split);
			if (get_inter(&split.inter) == 1)
			{
				split.deltax = poly->ver_list[split.curr_ver].tx - poly->ver_list[split.loop - 1].tx;
				split.deltay = poly->ver_list[split.curr_ver].ty - poly->ver_list[split.loop - 1].ty;
				split.texx = poly->ver_list[split.loop - 1].tx + split.deltax * split.inter.perc;
				split.texy = poly->ver_list[split.loop - 1].ty + split.deltay * split.inter.perc;
				split.copy.x = split.inter.inter.x;
				split.copy.y = split.inter.inter.y;
				split.copy.z = split.inter.inter.z;
				split.copy.tx = split.texx;
				split.copy.ty = split.texy;
				if (split.class == 1)
				{
					split.back[split.b_count++] = split.copy;
					split.front[split.f_count++] = split.copy;
					if (split.curr_ver != 0)
						split.front[split.f_count++] = poly->ver_list[split.curr_ver];
				}
				if (split.class == -1)
				{
					split.back[split.b_count++]= split.copy;
					split.front[split.f_count++] = split.copy;
					if (split.curr_ver != 0)
						split.back[split.b_count++] = poly->ver_list[split.curr_ver];
				}
			}
			else
			{
				if (split.class == 1)
				{
					if (split.curr_ver != 0)
						split.front[split.f_count++] = poly->ver_list[split.curr_ver];
				}
				if (split.class == -1)
					if (split.curr_ver != 0)
						split.back[split.b_count++] = poly->ver_list[split.curr_ver];
			}
		}
		front->nb_ver = 0;
		back->nb_ver = 0;
		//printf("HOP\n");
		if (!(front->ver_list = (t_vec*)malloc(sizeof(t_vec) * split.f_count)))
			exit(0);
		if (!(back->ver_list = (t_vec*)malloc(sizeof(t_vec) * split.b_count)))
			exit(0);
		printf("FCOUNT %d BCOUNT %d\n", split.f_count, split.b_count);
		split.i = 0;
		while (split.i < split.f_count)
		{
			front->nb_ver++;
			front->ver_list[split.i] = split.front[split.i];
			split.i++;
		}
		split.i = 0;
		while (split.i < split.b_count)
		{
			back->nb_ver++;
			back->ver_list[split.i] = split.back[split.i];
			split.i++;
		}
		front->nb_indices = (front->nb_ver - 2) * 3;
		back->nb_indices = (back->nb_ver - 2) * 3;
		printf("HOP %d %d \n", front->nb_indices, back->nb_indices);
		if (!(front->indices = (int*)malloc(sizeof(int) * front->nb_indices)))
			exit(0);
		printf("HOP\n");
		if (!(back->indices = (int*)malloc(sizeof(int) * back->nb_indices)))
			exit(0);
		printf("HOP\n");
		split.i = 0;
		while (split.i < front->nb_indices / 3)
		{
			if (split.i == 0)
			{
				split.v0 = 0;
				split.v1 = 1;
				split.v2 = 2;
			}
			else
			{
				split.v1 = split.v2;
				split.v2++;
			}
			front->indices[split.i * 3] = split.v0;
			front->indices[(split.i * 3) + 1] = split.v1;
			front->indices[(split.i * 3) + 2] = split.v2;
			split.i++;
		}
		split.i = 0;
		while (split.i < back->nb_indices / 3)
		{
			if (split.i == 0)
			{
				split.v0 = 0;
				split.v1 = 1;
				split.v2 = 2;
			}
			else
			{
				split.v1 = split.v2;
				split.v2++;
			}
			back->indices[split.i * 3] = split.v0;
			back->indices[(split.i * 3) + 1] = split.v1;
			back->indices[(split.i * 3) + 2] = split.v2;
			split.i++;
		}
		front->normal = poly->normal;
		back->normal = poly->normal;
		split.loop++;
	}
	//printf("FIN SPLIT POLY\n");
}