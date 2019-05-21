#include "bsp.h"

void init_vec(t_vec *vec)
{
	vec->x = 0;
	vec->y = 0;
	vec->z = 0;
}

void init_portal(t_portal *portal)
{
	portal->ver_list = NULL;
	init_vec(&portal->normal);
	portal->nb_ver = 0;
	portal->nb_indices = 0;
	portal->indices = NULL;
	portal->next = NULL;
	portal->prev = NULL;
	portal->nb_leafs = 0;
	portal->leafs[0] = 0;
	portal->leafs[1] = 0;
}

float	calculate_box_length(t_bsp *bsp, t_calcp *calc)
{
	t_vec tmp;

	tmp.x = calc->maxp.x - calc->cb.x;
	tmp.y = calc->maxp.y - calc->cb.y;
	tmp.z = calc->maxp.z - calc->cb.z;
	return (sqrt(tmp.x * tmp.x + tmp.y * tmp.y + tmp.z * tmp.z));
}

t_portal *calculate_init_portal(t_bsp *bsp, int node)
{
	t_calcp calc;

	printf("CREATE PORTAL\n");
	calc.maxp = bsp->node[node].bbox.boxmax;
	calc.minp = bsp->node[node].bbox.boxmin;
	calc.plane_normal = bsp->plane[bsp->node[node].plane].normal;
	calc.cb.x = (calc.maxp.x + calc.minp.x) / 2;
	calc.cb.y = (calc.maxp.y + calc.minp.y) / 2;
	calc.cb.x = (calc.maxp.z + calc.minp.z) / 2;
	calc.dist_plane = (bsp->plane[bsp->node[node].plane].point.x - calc.cb.x) * calc.plane_normal.x
		+ (bsp->plane[bsp->node[node].plane].point.y - calc.cb.y) * calc.plane_normal.y
		+ (bsp->plane[bsp->node[node].plane].point.z - calc.cb.z) * calc.plane_normal.z;
	calc.cp.x = calc.cb.x + (calc.plane_normal.x * calc.dist_plane);
	calc.cp.y = calc.cb.y + (calc.plane_normal.y * calc.dist_plane);
	calc.cp.z = calc.cb.z + (calc.plane_normal.z * calc.dist_plane);
	calc.a.x = 0;
	calc.a.y = 0;
	calc.a.z = 0;
	if (fabs(calc.plane_normal.y) > fabs(calc.plane_normal.z))
	{
		if (fabs(calc.plane_normal.z) < fabs(calc.plane_normal.x))
			calc.a.z = 1;
		else
			calc.a.x = 1;
	}
	else
	{
		if (fabs(calc.plane_normal.y) < fabs(calc.plane_normal.x))
			calc.a.y = 1;
		else
			calc.a.x = 1;
	}
	vec_cross_prod(&(calc.a), &(calc.plane_normal), &(calc.u));
	vec_normalize(&(calc.u), &(calc.u));
	vec_cross_prod(&(calc.u), &(calc.plane_normal), &(calc.v));
	vec_normalize(&(calc.v), &(calc.v));
	calc.length = calculate_box_length(bsp, &calc);
	calc.u.x *= calc.length;
	calc.u.y *= calc.length;
	calc.u.z *= calc.length;
	calc.v.x *= calc.length;
	calc.v.y *= calc.length;
	calc.v.z *= calc.length;
	if(!(calc.portal = (t_portal*)malloc(sizeof(t_portal))))
		exit(0);
	init_portal(calc.portal);
	if (!(calc.portal->ver_list = (t_vec*)malloc(sizeof(t_vec) * 4)))
		exit(0);
	set_vec(&(calc.portal->ver_list[0]), calc.cp.x + calc.u.x - calc.v.x, calc.cp.y + calc.u.y - calc.v.y, calc.cp.z + calc.u.z - calc.v.z);
	set_vec(&(calc.portal->ver_list[1]), calc.cp.x + calc.u.x + calc.v.x, calc.cp.y + calc.u.y + calc.v.y, calc.cp.z + calc.u.z + calc.v.z);
	set_vec(&(calc.portal->ver_list[2]), calc.cp.x - calc.u.x + calc.v.x, calc.cp.y - calc.u.y + calc.v.y, calc.cp.z - calc.u.z + calc.v.z);
	set_vec(&(calc.portal->ver_list[3]), calc.cp.x - calc.u.x - calc.v.x, calc.cp.y - calc.u.y - calc.v.y, calc.cp.z - calc.u.z - calc.v.z);
	calc.portal->normal = calc.plane_normal;
	calc.portal->nb_ver = 4;
	calc.portal->nb_indices = 6;
	if (!(calc.portal->indices = (int*)malloc(sizeof(int) * 6)))
		exit(0);
	calc.portal->indices[0] = 0;
	calc.portal->indices[1] = 1;
	calc.portal->indices[2] = 3;
	calc.portal->indices[3] = 3;
	calc.portal->indices[4] = 1;
	calc.portal->indices[5] = 2;
	printf("END CREATE PORTAL\n");
	return (calc.portal);
}