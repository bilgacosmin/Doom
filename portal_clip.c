#include "bsp.h"

void delete_portal(t_portal *portal)
{
	if (portal->prev != NULL)
		portal->prev->next = portal->next;
	if (portal->next != NULL)
		portal->next->prev = portal->prev;
	free(portal);
	portal = NULL;
	//deletes a portal, TO DO
}

void init_pclip(t_pclip *pclip)
{
	pclip->p_list = NULL;
	pclip->fp_list = NULL;
	pclip->bp_list = NULL;
	pclip->iter = NULL;
	pclip->back_split = NULL;
	pclip->front_split = NULL;
	pclip->result = 0;
}

void split_portal(t_pclip *pclip, t_portal *portal, t_plane *plane)
{
	//printf("split portal\n");
	split_poly((t_poly*)portal, plane, (t_poly*)(pclip->front_split), (t_poly*)(pclip->back_split));
	//printf("split portal 2\n");
	pclip->front_split->nb_leafs = portal->nb_leafs;
	pclip->back_split->nb_leafs = portal->nb_leafs;
	pclip->front_split->leafs[0] = portal->leafs[0];
	pclip->front_split->leafs[1] = portal->leafs[1];
	pclip->back_split->leafs[0] = portal->leafs[0];
	pclip->back_split->leafs[1] = portal->leafs[1];
	//printf("fin split portal\n");
}

t_portal *clip_portal(t_bsp *bsp, int node, t_portal *portal)
{
	t_pclip pclip;

	//printf("PORTAL CLIP NODE %d\n", node);
	init_pclip(&pclip);
	pclip.result = class_poly(&bsp->plane[bsp->node[node].plane], (t_poly*)portal);
	//printf("PCLIP RES %d\n", pclip.result);
	if (pclip.result == 1)
	{
		//printf("TEST 1 IN\n");
		if (bsp->node[node].isleaf == 0)
		{
			pclip.p_list = clip_portal(bsp, bsp->node[node].front, portal);
			//printf("TEST 1 OUT\n");
			return (pclip.p_list);
		}
		else
		{
			if (portal->nb_leafs > 1)
				exit(0);
			portal->leafs[portal->nb_leafs] = bsp->node[node].front;
			portal->nb_leafs++;
			portal->next = NULL;
			portal->prev = NULL;
			//printf("TEST 1 OUT\n");
			return (portal);
		}
	}
	//printf("clip check 1\n");
	if (pclip.result == -1)
	{
		//printf("TEST -1 IN\n");
		if (bsp->node[node].back != -1)
		{
			pclip.p_list = clip_portal(bsp, bsp->node[node].back, portal);
			//printf("TEST -1 OUT\n");
			return (pclip.p_list);
		}
		else
		{
			delete_portal(portal);
			//printf("TEST -1 OUT\n");
			return NULL;
		}
	}
	//printf("clip check -1\n");
	if (pclip.result == 2)
	{
		//printf("TEST 2 IN\n");
		//printf("check -1\n");
		if (!(pclip.front_split = (t_portal*)malloc(sizeof(t_portal))))
			exit(0);
		if (!(pclip.back_split = (t_portal*)malloc(sizeof(t_portal))))
			exit(0);
		init_portal(pclip.front_split);
		init_portal(pclip.back_split);
		//printf("inits\n");
		split_portal(&pclip, portal, &bsp->plane[bsp->node[node].plane]); ///to check
		//printf("post_split\n");
		delete_portal(portal);
		//printf("post delete\n");
		if (bsp->node[node].isleaf == 0)
		{
			pclip.fp_list = clip_portal(bsp, bsp->node[node].front, pclip.front_split);
		}
		else
		{
			pclip.front_split->leafs[pclip.front_split->nb_leafs] = bsp->node[node].front;
			pclip.front_split->nb_leafs++;
			pclip.front_split->prev = NULL;
			pclip.front_split->next = NULL;
			pclip.fp_list = pclip.front_split;
		}
		//printf("checkyty\n");
		if (bsp->node[node].back != -1)
		{
			pclip.bp_list = clip_portal(bsp, bsp->node[node].back, pclip.back_split);
		}
		else
		{
			//printf("checkyty2\n");
			delete_portal(pclip.back_split);
			//printf("checkyty\n");
		}
		if (pclip.fp_list != NULL)
		{
			pclip.iter = pclip.fp_list;
			while (pclip.iter->next != NULL)
			{
				//printf("343\n");
				pclip.iter = pclip.iter->next;
			}
			if (pclip.bp_list != NULL)
			{
				pclip.iter->next = pclip.bp_list;
				pclip.bp_list->prev = pclip.iter;
			}
			//printf("TEST 2 OUT\n");
			return (pclip.fp_list);
		}
		else
		{
			//printf("TEST 2 OUT\n");
			if (pclip.bp_list != NULL)
				return (pclip.bp_list);
			return (NULL);
		}
		//printf("TEST 2 OUT\n");
		return (NULL);
	}
	//printf("clip check 2\n");
	if (pclip.result == 0)
	{
		//printf("TEST 0 IN\n");
		if (bsp->node[node].isleaf != 0)
		{
			portal->leafs[portal->nb_leafs] = bsp->node[node].front;
			portal->nb_leafs++;
			portal->next = NULL;
			portal->prev = NULL;
			pclip.fp_list = portal;
		}
		else
		{
			pclip.fp_list = clip_portal(bsp, bsp->node[node].front, portal);
		}
		if (pclip.fp_list == NULL)
			{
				//printf("TEST 0 OUT\n");
				return (NULL);
			}
		if (bsp->node[node].back == -1)
		{
			//printf("TEST 0 OUT\n");
			return (pclip.fp_list);
		}
		while (pclip.fp_list != NULL)
		{
			//printf("344\n");
			pclip.tmp = pclip.fp_list->next;
			pclip.bp_list = NULL;
			pclip.bp_list = clip_portal(bsp, bsp->node[node].back, pclip.fp_list);
			if (pclip.bp_list != NULL)
			{
				pclip.iter = pclip.bp_list;
				while (pclip.iter->next != NULL)
				{
					//printf("345\n");
					pclip.iter = pclip.iter->next;
				}
				pclip.iter->next = pclip.p_list;
				if (pclip.p_list != NULL)
					pclip.p_list->prev = pclip.iter;
				pclip.p_list = pclip.bp_list;
			}
			pclip.fp_list = pclip.tmp;
		}
		//printf("TEST 0 OUT\n");
		return (pclip.p_list);
	}
	//printf("TEST 0 OUT\n");
	return (NULL);
}
