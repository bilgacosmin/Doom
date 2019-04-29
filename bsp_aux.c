/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp_aux.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbilga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/15 11:38:45 by cbilga            #+#    #+#             */
/*   Updated: 2019/03/21 08:02:57 by cbilga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsp.h"

t_poly 	*clone_poly(t_poly *poly)
{
	t_poly *clone;
	int i;

	if (!(clone = (t_poly*)malloc(sizeof(t_poly))))
		exit(0);
	if (!(clone->ver_list = (t_vec*)malloc(sizeof(t_vec) * poly->nb_ver)))
		exit(0);
	i = 0;
	while (i < poly->nb_ver)
	{
		clone->ver_list[i] = poly->ver_list[i];
		i++;
	}
	clone->normal = poly->normal;
	clone->nb_ver = poly->nb_ver;
	clone->nb_indices = poly->nb_indices;
	if (!(clone->indices = (int*)malloc(sizeof(int) * poly->nb_indices)))
		exit(0);
	i = 0;
	while (i < poly->nb_indices)
	{
		clone->indices[i] = poly->indices[i];
		i++;
	}
	clone->next = poly->next;
	clone->was_splitter = poly->was_splitter;
	if (poly->was_splitter > 1)
	{
		printf("SUKKAA\n");
		//print_nodes(poly);
		printf("CLONE POLY WAS SPLIT %d\n", clone->was_splitter);
	}
	clone->texture = poly->texture;
	return (clone);
}

t_vec	*create_vec(float x, float y, float z)
{
	t_vec	*new;

	if (!(new = (t_vec*)malloc(sizeof(t_vec))))
		exit(0);
	new->x = x;
	new->y = y;
	new->z = z;
	//new->next = NULL;
	return (new);
}
/*float	fabs(float f)
{
	if (f < 0)
		return (0 - f);
	return (f);
}*/

int ft_abs(int i)
{
	if (i < 0)
		return (-i);
	return (i);
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

int		class_point(t_vec *pos, t_plane *plane)
{
	float	res;
	t_vec	v1;
	t_vec	dir;

	v1 = plane->normal;
	dir.x = v1.x - pos->x;
	dir.y = v1.y - pos->y;
	dir.z = v1.z - pos->z;
	res = dir.x * plane->normal.x + dir.y * plane->normal.y + dir.z * plane->normal.z;
	if (res < -0.001)
		return (1);
	if (res > 0.001)
		return (-1);
	return (0);
}
