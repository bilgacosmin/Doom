/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp_build.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbilga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/15 08:57:48 by cbilga            #+#    #+#             */
/*   Updated: 2019/03/21 08:41:33 by cbilga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



//TODO FAUT TOUVER bsp->poly qui doit etre remplace par bsp->poly_list !!!!!!
#include "bsp.h"

static t_tree *init_tree(int node)
{
	t_tree *t;

	if (!(t = (t_tree*)malloc(sizeof(t_tree))))
		exit(0);
	t->test = NULL;
	t->front = NULL;
	t->back = NULL;
	t->next = NULL;
	t->front_split = NULL;
	t->back_split = NULL;
	t->node = node;
	t->count = 0;
	return (t);
}

static void class_case0(t_tree *t, t_bsp *bsp) //on plane case
{
	printf("CC0\n");
	t->a = bsp->plane[bsp->node[t->node].plane].normal;
	t->b = t->test->normal;
	t->result = fabs((t->a.x - t->b.x) + (t->a.y - t->b.y) + (t->a.z - t->b.z));
	if (t->result < 0.1)
	{
		t->clone->next = t->front;
		t->front = t->clone;
	}
	else
	{
		t->clone->next = t->back;
		t->back = t->clone;
	}
	//printf("FIN CC0\n");
}

static void class_case1(t_tree *t, t_bsp *bsp) //front or back case
{
	printf("CC1\n");
	if (t->class == 1)
	{
		t->clone->next = t->front;
		t->front = t->clone;
	}
	else
	{
		t->clone->next = t->back;
		t->back = t->clone;
	}
	//printf("FIN CC1\n");
}

static void class_case2(t_tree *t, t_bsp *bsp)
{
	t_poly	*front;
	t_poly	*back;

	printf("CC2\n");
	if (!(front = (t_poly*)malloc(sizeof(t_poly))))
		return ;
	if (!(back = (t_poly*)malloc(sizeof(t_poly))))
		return ;
	//printf("HOP\n");
	split_poly(t->test, &bsp->plane[bsp->node[t->node].plane], front, back);
	//printf("HOP\n");
	front->was_splitter = t->test->was_splitter;
	back->was_splitter = t->test->was_splitter;
	printf("BUILD WAS SPLIT %d %d\n", front->was_splitter, back->was_splitter);
	front->texture = t->test->texture;
	back->texture = t->test->texture;
	free_poly(t->test);
	t->test = NULL;
	front->next = t->front;
	t->front = front;
	back->next = t->back;
	t->back = back;
	//printf("FIN CC2\n");
}

static void class_case(t_tree *t, t_bsp *bsp)
{
	//printf("CLASS CASE\n");
	//printf("CLASS CASE SPLIT %d\n", t->test->was_splitter);
	t->clone = clone_poly(t->test);
	if (t->class == 0)
		class_case0(t, bsp);
	if (t->class == 1 || t->class == -1)
		class_case1(t, bsp);
	if (t->class == 2)
		class_case2(t, bsp);
	//printf("FIN CLASS CASE\n");
}

void	build_bsp_tree(int node, t_bsp *bsp)
{
	t_tree *t;

	//printf("TREE\n");
	//printf("VER %d\n", bsp->poly_list->nb_ver);
	t = init_tree(node);
	//printf("TREE2 %d\n", bsp->node[t->node].plane);
	bsp->node[t->node].plane = select_splitter(bsp);
	if (bsp->node[t->node].plane == -1)
	{
		free(t);
		return ;
	}
	else
	{
		printf("NEW SPLIT\n");
	}
	//printf("TREE %p\n", t->test);
	t->test = bsp->poly_list;
	//printf("TREE\n");
	bsp->node[node].bbox.boxmin = *create_vec(100000, 100000, 100000);
	bsp->node[node].bbox.boxmax = *create_vec(-100000, -100000, -100000);
	//printf("TREE\n");
	while (t->test != NULL)
	{
		//printf("START\n");
		t->next = t->test->next;
		t->class = class_poly(&bsp->plane[bsp->node[t->node].plane], t->test);
		class_case(t, bsp);
		t->test = t->next;
		//printf("END\n");
	}
	//printf("TFRONT\n");
	//print_nodes(t->front);
	t->temp = t->front;
	while (t->temp != NULL)
	{
		if (t->temp->was_splitter == 0)
			t->count++;
		t->temp = t->temp->next;
	}
	calc_box(&bsp->node[t->node].bbox, t->front);
	t->leaf_box = bsp->node[t->node].bbox;
	calc_box(&bsp->node[t->node].bbox, t->back);
	//printf("ICI ??\n");
	if (t->count == 0)
	{
	//	printf("ON EST LAS\n");
		t->iter = t->front;
		bsp->leaf[bsp->nb_leafs].start_poly = bsp->nb_polys;
		while (t->iter != NULL)
		{
			bsp->poly[bsp->nb_polys] = *t->iter;
			inc_polys(bsp);
			t->temp = t->iter;
			t->iter = t->iter->next;
			t->temp = NULL; //leak possible, to check !
		}
		bsp->leaf[bsp->nb_leafs].end_poly = bsp->nb_polys;
		bsp->leaf[bsp->nb_leafs].bbox = t->leaf_box;
		bsp->node[t->node].front = bsp->nb_leafs;
		bsp->node[t->node].isleaf = 1;
		inc_leafs(bsp);
	}
	else
	{
		bsp->node[t->node].isleaf = 0;
		bsp->node[t->node].front = bsp->nb_nodes + 1;
		inc_nodes(bsp);
		bsp->poly_list = t->front;
		build_bsp_tree(bsp->nb_nodes, bsp);
	}
	if (t->back == NULL)
		bsp->node[t->node].back = -1;
	else
	{
		bsp->node[t->node].back = bsp->nb_nodes + 1;
		inc_nodes(bsp);
		bsp->poly_list = t->back;
		build_bsp_tree(bsp->nb_nodes, bsp);
	}
	free(t);
}
