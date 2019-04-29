/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp_intersect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbilga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 11:35:38 by cbilga            #+#    #+#             */
/*   Updated: 2019/03/21 14:31:52 by cbilga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsp.h"

void	init_inter(t_inter *inter, t_split	*split)
{
	inter->start = split->a;
	inter->end = split->b;
	inter->point = split->point;
	inter->normal = split->normal;
}

int	get_inter(t_inter *inter)
{
	inter->direction->x = inter->end->x - inter->start->x;
	inter->direction->y = inter->end->y - inter->start->y;
	inter->direction->z = inter->end->z - inter->start->z;
	inter->line_len = inter->direction->x * inter->normal->x + inter->direction->y * inter->normal->y + inter->direction->z * inter->normal->z;
	if (fabs(inter->line_len) < 0.0001)
		return (0);
	inter->l->x = inter->point->x - inter->start->x;
	inter->l->y = inter->point->y - inter->start->y;
	inter->l->z = inter->point->z - inter->start->z;
	inter->dist = inter->l->x * inter->normal->x + inter->l->y * inter->normal->y + inter->l->z * inter->normal->z;
	inter->perc = inter->dist / inter->line_len;
	if (inter->perc < 0)
		return (0);
	else
	{
		if (inter->perc > 1)
			return (0);
	}
	inter->inter.x = inter->start->x + inter->direction->x * inter->perc;
	inter->inter.y = inter->start->y + inter->direction->y * inter->perc;
	inter->inter.z = inter->start->z + inter->direction->z * inter->perc;
	return (1);
}
