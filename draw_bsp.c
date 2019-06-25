#include "bsp.h"

t_poly *visible_polygons(t_bsp *bsp, int leaf)
{
	t_vp vp;

	printf("VISIBLE POLY\n");
	vp.pointer = bsp->pvs + bsp->leaf[leaf].pvs_index;
	vp.curr_leaf = 0;
	vp.list = NULL;
	while (vp.curr_leaf < bsp->nb_leafs)
	{
		//printf("CURRLEAF %d\n", vp.curr_leaf);
		if (*vp.pointer != 0)
		{
			vp.i = 0;
			while (vp.i < 8)
			{
				//printf("VP.I %d\n", vp.i);
				vp.mask = 1 << vp.i;
				vp.pvs = *vp.pointer;
				if (vp.pvs & vp.mask)
				{
					vp.start = bsp->leaf[vp.curr_leaf].start_poly;
					vp.finish = bsp->leaf[vp.curr_leaf].end_poly;
					vp.count = vp.start;
					while (vp.count < vp.finish)
					{
						//printf("CHECK VPCOUNT %d\n", vp.count);
						vp.curr = &bsp->poly[vp.count];
						if (vp.list == NULL)
						{
							vp.curr->next = NULL;
							vp.list = vp.curr;
						}
						else
						{
							vp.curr->next = vp.list;
							vp.list = vp.curr;
						}
						vp.count++;
					}
				}
				vp.curr_leaf++;
				vp.i++;
			}
			vp.pointer++;
		}
		else
		{
			vp.pointer++;
			vp.count = *vp.pointer;
			vp.pointer++;
			vp.curr_leaf += vp.count * 8;
		}
	}
	return (vp.list);
}

t_poly *render_bsp(t_bsp *bsp, t_vec *pos)
{
	int node;
	int leaf;
	int result;
	t_poly *list;

	node = 0;
	leaf = 0;
	//printf("RENDER\n");
	while(1)
	{
		result = class_point(pos, &(bsp->plane[bsp->node[node].plane]));
		//printf("RESULT %d\n", result);
		if (result == 1 || result == 0)
		{
			if (bsp->node[node].isleaf == 1)
			{
				leaf = bsp->node[node].front;
				return (visible_polygons(bsp, leaf));
			}
			else
				node = bsp->node[node].front;
			//break;
		}
		if (result == -1)
		{
			if (bsp->node[node].back == -1)
				return (NULL);
			else
				node = bsp->node[node].back;
			//break;
		}
	}
	return (NULL);
}




/** void RenderTree(D3DVECTOR pos)
{
int Node=0;
int leaf=0;
while(1)
{
switch(ClassifyPoint(&pos,&PlaneArray[NodeArray[Node].Plane]))
{
case CP_FRONT: 
if (NodeArray[Node].IsLeaf!=0)
{
leaf=NodeArray[Node].Front;
DrawTree(leaf);
return;
}
else
{
Node = NodeArray[Node].Front;
}
break;

case CP_BACK: 
if (NodeArray[Node].Back==-1)
{
return;
}
else
{
Node = NodeArray[Node].Back;
}
break;

case CP_ONPLANE:
if (NodeArray[Node].IsLeaf!=0)
{
leaf=NodeArray[Node].Front;
DrawTree(leaf);
return;
}
else
{
Node = NodeArray[Node].Front;
}

default:
break;
}
}// end while
}// End Function **/