/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp_class.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbilga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/15 11:54:21 by cbilga            #+#    #+#             */
/*   Updated: 2019/03/15 13:45:25 by cbilga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsp.h"

static t_vec	vec_sub(t_vec v1, t_vec v2)
{
	t_vec r;

	r.x = v1.x - v2.x;
	r.y = v1.y - v2.y;
	r.z = v1.z - v2.z;
	return (r);
}

static float	dot_prod(t_vec v1, t_vec v2)
{
	float	r;
	r = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
	return (r);
}

char class_poly(t_plane *plane, t_poly *poly)
{
	t_cpoly	c;

	//printf("CLASS POLY\n");
	c.behind = 0;
	c.onplane = 0;
	c.infront = 0;
	c.v1 = plane->point;
	c.i = 0;
	while (c.i < poly->nb_ver)
	{
		c.v2 = poly->ver_list[c.i];
		c.dir = vec_sub(c.v1, c.v2);
		c.result = dot_prod(c.dir, plane->normal);
		if (c.result < -0.001)
			c.infront++;
		else
		{
			if (c.result > 0.001)
				c.behind++;
			else
			{
				c.behind++;
				c.onplane++;
				c.infront++;
			}
		}
		c.i++;
	}
	//printf("FIN CLASS POLY\n");
	if (c.onplane == poly->nb_ver)
		return (0);
	if (c.behind == poly->nb_ver)
		return (-1);
	if (c.infront == poly->nb_ver)
		return (1);
	return (2);
}
