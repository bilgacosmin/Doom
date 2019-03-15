/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp_aux.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbilga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/15 11:38:45 by cbilga            #+#    #+#             */
/*   Updated: 2019/03/15 16:04:43 by cbilga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsp.h"

t_vec	*create_vec(float x, float y, float z)
{
	t_vec	*new;

	if (!(new = (t_vec*)malloc(sizeof(t_vec))))
		return (NULL);
	new->x = x;
	new->y = y;
	new->z = z;
	return (new);
}

float	fabs(float f)
{
	if (f < 0)
		return (0 - f);
	return (f);
}

void	calc_box(t_box *box, t_poly *p)
{
	int 	i;

	while (p != NULL)
	{
		i = 0;
		while (i < p->nb_ver)
		{
			if (p->ver_list[i].x < box->boxmin.x)
				box->boxmin.x = p->ver_list[i].x;
			if (p->ver_list[i].y < box->boxmin.y)
				box->boxmin.y = p->ver_list[i].y;
			if (p->ver_list[i].z < box->boxmin.z)
				box->boxmin.z = p->ver_list[i].z;
			if (p->ver_list[i].x > box->boxmax.x)
				box->boxmax.x = p->ver_list[i].x;
            if (p->ver_list[i].y > box->boxmax.y)
				box->boxmax.y = p->ver_list[i].y;
			if (p->ver_list[i].z > box->boxmax.z)
				box->boxmax.z = p->ver_list[i].z;
			i++;
		}
		p = p->next;
	}
}

void	free_poly(t_poly *p)
{
	free(p->ver_list);
	free(p->indices);
	free(p);
}
