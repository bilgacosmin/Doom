#include "bsp.h"

void init_bportal(t_bportal *bportal)
{
	bportal->index = 0;
	bportal->pointer = 0;
	bportal->inital = NULL;
	bportal->p_list = NULL;
	bportal->iter = NULL;
	bportal->tmp = NULL;
}

void init_min_vec(t_vec *vec)
{
	vec->x = 10000000;
	vec->y = 10000000;
	vec->z = 10000000;
}

void init_max_vec(t_vec *vec)
{
	vec->x = -10000000;
	vec->y = -10000000;
	vec->z = -10000000;
}

void get_poly_bounds(t_poly *poly, t_vec *min, t_vec *max)
{
	int i;

	init_min_vec(min);
	init_max_vec(max);
	i = 0;
	while (i < poly->nb_ver)
	{
		if (poly->ver_list[i].x < min->x)
			min->x = poly->ver_list[i].x;
		if (poly->ver_list[i].y < min->y)
			min->y = poly->ver_list[i].y;
		if (poly->ver_list[i].z < min->z)
			min->z = poly->ver_list[i].z;
		if (poly->ver_list[i].x > max->x)
			max->x = poly->ver_list[i].x;
		if (poly->ver_list[i].y > max->y)
			max->y = poly->ver_list[i].y;
		if (poly->ver_list[i].z > max->z)
			max->z = poly->ver_list[i].z;
		i++;
	}
}

void init_chk_dup(t_chk_dup *chk)
{
	chk->check1 = 0;
	chk->check2 = 0;
	chk->i = 0;
	if (!(chk->min1 = (t_vec*)malloc(sizeof(t_vec))))
		exit(0);
	if (!(chk->min2 = (t_vec*)malloc(sizeof(t_vec))))
		exit(0);
	if (!(chk->max1 = (t_vec*)malloc(sizeof(t_vec))))
		exit(0);
	if (!(chk->max2 = (t_vec*)malloc(sizeof(t_vec))))
		exit(0);
}

int check_duplicate(t_bsp *bsp, t_portal *portal, int *index)
{
	t_chk_dup chk;

	init_chk_dup(&chk);
	chk.check1 = portal->leafs[0];
	chk.check2 = portal->leafs[1];
	while (chk.i < bsp->nb_portals)
	{
		if ((bsp->portal[chk.i]->leafs[0] == chk.check1 && bsp->portal[chk.i]->leafs[1] == chk.check2)
			|| (bsp->portal[chk.i]->leafs[0] == chk.check2 && bsp->portal[chk.i]->leafs[1] == chk.check1))
		{
			get_poly_bounds((t_poly*)portal, chk.min1, chk.max1);
			get_poly_bounds((t_poly*)bsp->portal[chk.i], chk.min2, chk.max2);
			chk.size_new = sqrt((chk.max1->x - chk.min1->x) * (chk.max1->x - chk.min1->x) + (chk.max1->y - chk.min1->y) * (chk.max1->y - chk.min1->y)
				+ (chk.max1->z - chk.min1->z) * (chk.max1->z - chk.min1->z));
			chk.size_old = sqrt((chk.max2->x - chk.min2->x) * (chk.max2->x - chk.min2->x) + (chk.max2->y - chk.min2->y) * (chk.max2->y - chk.min2->y)
				+ (chk.max2->z - chk.min2->z) * (chk.max2->z - chk.min2->z));
			if (fabs(chk.size_new) > fabs(chk.size_old))
			{
				chk.tmp = bsp->portal[chk.i];
				delete_portal(chk.tmp);
				*index = chk.i;
				return (0);
			}
			else
				return (1);
		}
	}
	*index = bsp->nb_portals;
	return (0);
}

void remove_portal(t_portal *portal)
{
	if (portal->prev != NULL)
		portal->prev->next = portal->next;
	if (portal->next != NULL)
		portal->next->prev = portal->prev;
	free(portal);
	//deletes a portal, TO Check
}

void build_portal(t_bsp *bsp, t_bportal *bportal) //function traverses all nodes, creates a portal on them, clips them, 
{
	int 	pointer;

	if (bportal == NULL)
	{
		if (!(bportal = (t_bportal*)malloc(sizeof(bportal))))
			exit(0);
		init_bportal(bportal);
	}
	bportal->inital = calculate_init_portal(bsp, bportal->pointer);
	bportal->p_list = clip_portal(bsp, 0, bportal->inital);
	bportal->iter = bportal->p_list;
	while (bportal->iter != NULL)
	{
		if (bportal->iter->nb_leafs != 2)
		{
			bportal->tmp = bportal->iter->next;
			remove_portal(bportal->iter);
			bportal->iter = bportal->tmp;
		}
		else
		{
			if (check_duplicate(bsp, bportal->iter, &(bportal->index)))
			{
				bportal->tmp = bportal->iter->next;
				remove_portal(bportal->iter);
				bportal->iter = bportal->tmp;
			}
			else
			{
				bsp->portal[bportal->index] = bportal->iter;
				if (bportal->index == bsp->nb_portals)
				{
					bsp->leaf[bportal->iter->leafs[0]].portal[bsp->leaf[bportal->iter->leafs[0]].nbportals] = bsp->nb_portals;
					bsp->leaf[bportal->iter->leafs[1]].portal[bsp->leaf[bportal->iter->leafs[1]].nbportals] = bsp->nb_portals;
					bsp->leaf[bportal->iter->leafs[0]].nbportals++;
					bsp->leaf[bportal->iter->leafs[1]].nbportals++;
					inc_portals(bsp);
				}
			}
		}
	}
	if (bsp->node[bportal->pointer].isleaf != 1)
	{
		pointer = bportal->pointer;
		bportal->pointer = bsp->node[bportal->pointer].front;
		build_portal(bsp, bportal);
	}
	bportal->pointer = pointer;
	if (bsp->node[bportal->pointer].back != -1)
	{
		bportal->pointer = bsp->node[bportal->pointer].back;
		build_portal(bsp, bportal);
	}
}