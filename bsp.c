/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbilga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 08:49:52 by cbilga            #+#    #+#             */
/*   Updated: 2019/03/15 18:40:26 by cbilga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bsp.h"

void	bsp_init(t_bsp *bsp)
{

	bsp->max_nodes = 100;
    bsp->max_polys = 100;
    bsp->max_planes = 100;
    bsp->max_leafs = 100;
    bsp->max_portals = 100;
    if (!(bsp->poly = (t_poly*)malloc(sizeof(t_poly) * bsp->max_polys)))
		return ;
	if (!(bsp->node = (t_node*)malloc(sizeof(t_node) * bsp->max_nodes)))
		return ;
	if (!(bsp->leaf = (t_leaf*)malloc(sizeof(t_leaf) * bsp->max_leafs)))
		return ;
	if (!(bsp->portal = (t_portal**)malloc(sizeof(t_portal*) * bsp->max_portals)))
			return ;
	bsp->poly_list = loadMap("test_poly.txt");
	triangulize(bsp->poly_list);
    //BYTE *PVSData;
    bsp->nb_polys = 0;
    bsp->nb_nodes = 0;
    bsp->nb_leafs = 0;
    bsp->nb_planes = 0;
    bsp->nb_portals = 0;
}

void	inc_polys(t_bsp *bsp)
{
	t_poly *new;
	int		i;

	bsp->nb_polys++;
	if (bsp->nb_polys == bsp->max_polys)
	{
		bsp->max_polys += 100;
		if (!(new = (t_poly*)malloc(sizeof(t_poly) * bsp->max_polys)))
			return ;
		i = 0;
		while (i < bsp->nb_polys)
		{
			new[i] = bsp->poly[i];
			i++;
		}
		free(bsp->poly);
		bsp->poly = new;
	}
}

void	inc_nodes(t_bsp *bsp)
{
    t_node *new;
    int     i;

    bsp->nb_nodes++;
    if (bsp->nb_nodes == bsp->max_nodes)
    {
		bsp->max_nodes += 100;
		if (!(new = (t_node*)malloc(sizeof(t_node) * bsp->max_nodes)))
			return ;
		i = 0;
		while (i < bsp->nb_nodes)
		{
            new[i] = bsp->node[i];
            i++;
		}
		free(bsp->node);
        bsp->node = new;
    }
}

void    inc_leafs(t_bsp *bsp)
{
    t_leaf *new;
    int     i;

    bsp->nb_leafs++;
    if (bsp->nb_leafs == bsp->max_leafs)
    {
        bsp->max_leafs += 100;
        if (!(new = (t_leaf*)malloc(sizeof(t_leaf) * bsp->max_leafs)))
            return ;
        i = 0;
        while (i < bsp->nb_leafs)
        {
            new[i] = bsp->leaf[i];
            i++;
        }
        free(bsp->leaf);
        bsp->leaf = new;
    }
}

void    inc_planes(t_bsp *bsp)
{
    t_plane *new;
    int     i;

    bsp->nb_planes++;
    if (bsp->nb_planes == bsp->max_planes)
    {
        bsp->max_planes += 100;
        if (!(new = (t_plane*)malloc(sizeof(t_plane) * bsp->max_planes)))
            return ;
        i = 0;
        while (i < bsp->nb_planes)
        {
			new[i] = bsp->plane[i];
            i++;
        }
        free(bsp->plane);
        bsp->plane = new;
    }
}

void	inc_portals(t_bsp *bsp)
{
	t_portal	**new;
	int 		i;

	bsp->nb_portals++;
	if (bsp->nb_portals == bsp->max_portals)
	{
		bsp->max_portals += 100;
		if (!(new = (t_portal**)malloc(sizeof(t_portal*) * bsp->max_portals)))
			return ;
		i = 0;
		while (i < bsp->nb_portals)
		{
			new[i] = bsp->portal[i];
			i++;
		}
		free(bsp->portal);
		bsp->portal = new;
	}
}
/**
BSP algorithm:

Map structure:

Polygon : x1 y1 z1 ; x2 y2 z2 ; .... : "texture.tga"

- build solid node LEAF tree : (polygon plane use as splitter; polygon goes into front list but cannot be used as splitter again)
	- init polygons arrays
	- load polygons from file
	- add them to the list and split them in triangles
	- pick a polygon as splitter
	- check if next polygons in list are in front, back, onplane or spanning
	- in spanning -> split and give to children same BEENUSEDASSPLIT, add each side to corresponding list (front / back)
	- store it in the node
	- mark it as "used as splitter" so it won't be used again
	- calculate bounding box for each front/back list
	? loop through the polygon list and classify each against the splitter (Front / Back)
	- check if all polygons in FRONT list have all been used as splitters - if yes the list is a LEAF
	- store the polygons from LEAF into Polygon Array (realloc array when limit is reached)
	- store number into Leaf Array
	- remove the polygons from the listl
	- if BACK list is empty, store -1 meaning it's a solid space


- calculate PVS (potential visibility set - array stored for each leaf storing visbility information about all other leafs)
	- store data in a common array for all leafs (1 bit per leaf)
	- use zero length run (each time we got an empty byte it's followed by a byte saying how many consecutive bytes have 0)


Runtime:

- traverse BSP to find in which leaf we are

	Code example:
	void DrawTree(long leaf)
	{
		POLYGON *CurrentPoly;
		int i;
		long PVSOFFSET=LeafArray[leaf].PVSIndex;
		BYTE *PVSPointer=PVSData;
		PVSPointer+=PVSOFFSET;
		long currentleaf=0;
		while (currentleaf<NumberOfLeafs)
		{
			if (*PVSPointer!=0)
			{
				for (i=0;i<8;i++)
				{
					BYTE mask=1<<i;
					BYTE pvs=*PVSPointer;
					if (pvs&mask)
					{
						Render The Polygons in Leaf [ currentleaf ]; }
					}// end for if pvsdata
					currentleaf++;
				}// end for i;
				PVSPointer++;
			}
			else
			{// we have hit a zero so read in the next byte and see how long the run of zeros is
				PVSPointer++;
				BYTE RunLength=*PVSPointer;
				PVSPointer++;
				currentleaf+=RunLength*8;
			}
		}
	}


- check PVS data : render only leafs that are potentially visible
- use Z-buffer to store the distance to each leaf 

	Code example:
	void DrawTree (long CameraLeaf)
	{
		for (int i=0;i<NumberOfLeafs;i++)
		{
			long offset=CameraLeaf*numberOfLeafs;
			if (PVSData[offset+i]==1)
			{
				RenderLeaf(i);
			}
		}
	}**/
