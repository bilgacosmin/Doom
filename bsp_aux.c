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
/*	if (poly->was_splitter > 1)
	{
		printf("SUKKAA\n");
		//print_nodes(poly);
		printf("CLONE POLY WAS SPLIT %d\n", clone->was_splitter);
	}*/
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

void	set_vec(t_vec *new, float x, float y, float z)
{
	new->x = x;
	new->y = y;
	new->z = z;
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

int		class_point(t_vec *pos, t_plane *plane)
{
	float	res;
	t_vec	v1;
	t_vec	dir;

	v1 = plane->point;
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

void	vec_cross_prod(t_vec* a, t_vec *b, t_vec *c)
{
	c->x = a->y * b->z - a->z * b->y;
	c->y = a->z * b->x - a->x * b->z;
	c->z = a->x * b->y - a->y * b->x;
}

float	vec_dot_prod(t_vec *a, t_vec *b)
{
	return ((a->x + b->x) * (a->y + b->y) * (a->z + b->z));
}

void	vec_normalize(t_vec *a, t_vec *b)
{
	float magnitude;

	magnitude = sqrt(a->x * a->x + a->y * a->y + a->z * a->z);
	b->x = a->x / magnitude;
	b->y = a->y / magnitude;
	b->z = a->z / magnitude;
}

t_portal *portal_copy(t_portal *portal)
{
	t_portal *copy;

	if (!(copy = (t_portal*)malloc(sizeof(t_portal))))
		exit(0);
	if (!(copy->ver_list = (t_vec*)malloc(sizeof(t_vec) * portal->nb_ver)))
		exit(0);
	if (!(copy->indices = (int*)malloc(sizeof(int) * portal->nb_indices)))
		exit(0);
	copy->normal = portal->normal;
	copy->nb_ver = portal->nb_ver;
	copy->nb_indices = portal->nb_indices;
	copy->nb_leafs = portal->nb_leafs;
	copy->leafs[0] = portal->leafs[0];
	copy->leafs[1] = portal->leafs[1];
	return(copy);
}

t_plane get_portal_plane(t_portal *portal)
{
	t_plane plane;

	plane.point = portal->ver_list[0];
	plane.normal = portal->normal;
	return (plane);
}

void  delete_pointer(void **pointer)
{
	if (*pointer != NULL)
		free(*pointer);
	*pointer = NULL;
}