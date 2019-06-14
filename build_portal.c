#include "bsp.h"

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
	*index = -1;
	//printf("CHECK DUPLICATE\n");
	chk.check1 = portal->leafs[0];
	chk.check2 = portal->leafs[1];
	while (chk.i < bsp->nb_portals)
	{
		//printf("AHA\n");
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
				//printf("REPLACING %f\n", bsp->portal[chk.i]->ver_list[0].z);
				chk.tmp = bsp->portal[chk.i];
				delete_portal(chk.tmp);
				bsp->portal[chk.i] = NULL;
				*index = chk.i;
				return (0);
			}
			else
				return (1);
		}
		chk.i++;
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
	portal = NULL;
	//deletes a portal, TO Check
}

void init_buildp(t_bsp *bsp, t_buildp *buildp)
{
	if (!(buildp->nodestack = (t_nodestack*)malloc(sizeof(t_nodestack) * (bsp->nb_nodes + 1))))
		exit(0);
	buildp->stackpointer = 0;
	buildp->nodestack[buildp->stackpointer].node = 0;
	buildp->nodestack[buildp->stackpointer].jumpback = 0;
}

void build_portal_start(t_bsp *bsp, t_buildp *buildp)
{
	//printf("BUILD PORTAL START %d\n", buildp->nodestack[buildp->stackpointer].node);
	buildp->init = calculate_init_portal(bsp, buildp->nodestack[buildp->stackpointer].node);
	//printf("INITIAL PORTAL\n");
	//print_portals(&buildp->init, 1);
	buildp->p_list = clip_portal(bsp, 0, buildp->init);
	buildp->iter = buildp->p_list;
	while (buildp->iter != NULL)
	{
		if (buildp->iter->nb_leafs != 2)
		{
			buildp->tmp = buildp->iter->next;
			remove_portal(buildp->iter);
			buildp->iter = buildp->tmp;
		}
		else
		{
			//printf("SKOL\n");
			if (check_duplicate(bsp, buildp->iter, &(buildp->portalindex)))
			{
				//printf("SKOL1\n");
				buildp->tmp = buildp->iter->next;
				remove_portal(buildp->iter);
				buildp->iter = buildp->tmp;
			}
			else
			{
				//printf("SKOL2\n");
				bsp->portal[buildp->portalindex] = buildp->iter;
				if (buildp->portalindex == bsp->nb_portals)
				{
					buildp->i = 0;
					while (buildp->i < buildp->iter->nb_leafs)
					{
						buildp->index = buildp->iter->leafs[buildp->i];
						bsp->leaf[buildp->index].portal[bsp->leaf[buildp->index].nbportals] = bsp->nb_portals;
						bsp->leaf[buildp->index].nbportals++;
						buildp->i++;
					}
					inc_portals(bsp);
				}
				buildp->iter = buildp->iter->next;
			}
		}
	}
	//printf("INTER\n");
	if (bsp->node[buildp->nodestack[buildp->stackpointer].node].isleaf == 0)
	{
		//printf("TEST\n");
		buildp->nodestack[buildp->stackpointer + 1].node = bsp->node[buildp->nodestack[buildp->stackpointer].node].front;
		buildp->nodestack[buildp->stackpointer + 1].jumpback = 1;
		buildp->stackpointer++;
		//printf("TEST\n");
		build_portal_start(bsp, buildp);
	}
	//printf("TST))))\n");
	else
		build_portal_back(bsp, buildp);
	//build_portal_end(bsp, buildp);
}

void build_portal_back(t_bsp *bsp, t_buildp *buildp)
{
	//printf("BUILD PORTAL BACK %d stackpointer %d\n", bsp->node[buildp->nodestack[buildp->stackpointer].node].back, buildp->stackpointer);
	if (bsp->node[buildp->nodestack[buildp->stackpointer].node].back != -1)
	{
		buildp->nodestack[buildp->stackpointer + 1].node = bsp->node[buildp->nodestack[buildp->stackpointer].node].back;
		buildp->nodestack[buildp->stackpointer + 1].jumpback = 2;
		buildp->stackpointer++;
		build_portal_start(bsp, buildp);
	}
	else
		build_portal_end(bsp, buildp);
}

void build_portal_end(t_bsp *bsp, t_buildp *buildp)
{
	//printf("PRINT PORTAL END\n");
	buildp->stackpointer--;
	if (buildp->stackpointer > -1)
	{
		if (buildp->nodestack[buildp->stackpointer + 1].jumpback == 1)
			build_portal_back(bsp, buildp);
		else if (buildp->nodestack[buildp->stackpointer + 1].jumpback == 2)
			build_portal_end(bsp, buildp);
	}
}

void build_portal(t_bsp *bsp)
{
	t_buildp buildp;

	init_buildp(bsp, &buildp);
	build_portal_start(bsp, &buildp);
	//build_portal_back(bsp, &buildp);
	//build_portal_end(bsp, &buildp);
	free(buildp.nodestack);
}