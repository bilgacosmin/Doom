/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp_build.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbilga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/15 08:57:48 by cbilga            #+#    #+#             */
/*   Updated: 2019/03/15 18:43:03 by cbilga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsp.h"

static void	init_tree(t_tree *t, int node)
{
	t->test = NULL;
	t->front = NULL;
	t->back = NULL;
	t->next = NULL;
	t->front_split = NULL;
	t->back_split = NULL;
	t->node = node;
	t->count = 0;
}

static void class_case0(t_tree *t, t_bsp *bsp)
{
	t->a = bsp->plane[bsp->node[t->node].plane].normal;
	t->b = t->test->normal;
	t->result = fabs((t->a.x - t->b.x) + (t->a.y - t->b.y) + (t->a.z - t->b.z));
	if (t->result < 0.1)
	{
		t->test->next = t->front;
		t->front = t->test;
	}
	else
	{
		t->test->next = t->back;
		t->back = t->test;
	}
}

static void class_case1(t_tree *t, t_bsp *bsp)
{
	if (t->class == 1)
	{
		t->test->next = t->front;
		t->front = t->test;
	}
	else
	{
		t->test->next = t->back;
		t->back = t->test;
	}
}

static void class_case2(t_tree *t, t_bsp *bsp)
{
	t_poly	*fsplit;
	t_poly	*bsplit;

	if (!(fsplit = (t_poly*)malloc(sizeof(t_poly))))
		return ;
	if (!(bsplit = (t_poly*)malloc(sizeof(t_poly))))
		return ;
	split_poly(t->test, bsp->plane[bsp->node[t->node].plane], fsplit, bsplit);
	fsplit->was_splitter = t->test->was_splitter;
	bsplit->was_splitter = t->test->was_splitter;
	fsplit->texture = t->test->texture;
	bsplit->texture = t->test->texture;
	free_poly(t->test);
	t->test = NULL;
	fsplit->next = t->front;
	t->front = fsplit;
	bsplit->next = t->back;
	t->back = bsplit;
}

static void class_case(t_tree *t, t_bsp *bsp)
{
	if (t->class == 0)
		class_case0(t, bsp);
	if (t->class == 1 || t->class == -1)
		class_case1(t, bsp);
	if (t->class == 2)
		class_case2(t, bsp);
}

void	build_bsp_tree(int node, t_bsp *bsp)
{
	t_tree *t;

	init_tree(t, node);
	bsp->node[t->node].plane = select_splitter(bsp);
	t->test = bsp->poly;
	bsp->node[node].bbox.boxmin = create_vec(-100000, -100000, -100000);
	bsp->node[node].bbox.boxmax = create_vec(100000, 100000, 100000);
	while (t->test != NULL)
	{
		t->next = t->test->next;
		t->class = class_poly(&bsp->plane[bsp->node[t->node].plane], t->test);
		class_case(t, bsp);
		t->test = t->next;
	}
	t->temp = t->front;
	while (t->temp != NULL)
	{
		if (t->temp->was_splitter == 0)
			t->count++;
		t->temp = t->temp->next;
	}
	calc_box(&bsp->node[t->node].bbox, t->front);
	t->leaf_box = bsp->node[t->node].bbox;
	calc_box(&bsp->node[t->node].bbox, t-back);
	if (t->count == 0)
	{
		t->iter = t->front;
		bsp->leaf[bsp->nb_leafs].start_poly = bsp->nb_poly;
		while (t->iter != NULL)
		{
			bsp->poly[bsp->nb_polys] = t->iter;
			inc_polys(bsp);
			t->temp = t->iter;
			t->iter = t->iter->next;
			free(temp); //
		}
		
	//TODO TODO
 
	if ( count==0)
	{
		POLYGON *Iterator=FrontList;
		POLYGON *Temp;
		LeafArray[NumberOfLeafs].StartPolygon=NumberOfPolygons;
		while (Iterator!=NULL)
		{
			PolygonArray[NumberOfPolygons]=*Iterator;
			IncreaseNumberOfPolygons();
			Temp=Iterator;
			Iterator=Iterator->Next;
			delete Temp;//
		}
		LeafArray[NumberOfLeafs].EndPolygon=NumberOfPolygons;
		LeafArray[NumberOfLeafs].BoundingBox=LeafBox;
		NodeArray[Node].Front=NumberOfLeafs;
		NodeArray[Node].IsLeaf=1;
		IncreaseNumberOfLeafs(); 
	}
	else 
	{
		NodeArray[Node].IsLeaf=0;
		NodeArray[Node].Front=NumberOfNodes+1;
		IncreaseNumberOfNodes();
		BuildBspTree(NumberOfNodes,FrontList);
	}

	if (BackList==NULL)
	{
		NodeArray[Node].Back=-1;
	}
	else
	{
		NodeArray[Node].Back=NumberOfNodes+1;
		IncreaseNumberOfNodes();
		BuildBspTree(NumberOfNodes,BackList);
	} 

}// end function
