#include "bsp.h"

void print_polys(t_poly *poly, int nb_polys)
{
	int i;
	int j;
	t_vec *tmp;

	i = 0;
	printf("Nb polys %d\n", nb_polys);
	while (i < nb_polys)
	{
		printf("Polygon %d\n", i);
		printf("Vertices: (%d)\n", poly[i].nb_ver);
		j = 0;
		while (j < poly[i].nb_ver)
		{
			printf("Vertex: %f %f %f\n", poly[i].ver_list[j].x, poly[i].ver_list[j].y, poly[i].ver_list[j].z);
			j++;
		}
		printf("Indices: (%d)\n", poly[i].nb_indices);
		j = 0;
		while (j < poly[i].nb_indices)
		{
			printf("%d ",poly[i].indices[j]);
			j++;
		}
		printf("Was splitter %d\n", poly[i].was_splitter);
		printf("\n");
		i++;
	}
	printf("\n");
}

void print_planes(t_plane *planes, int nb_planes)
{
	int i;
	printf("NB planes %d\n", nb_planes);
	i = 0;
	while (i < nb_planes)
	{
		printf("Plane (%d) -> point (x = %f y = %f z = %f) ; normal (x = %f y = %f z = %f)\n", i, planes[i].point.x, planes[i].point.y, planes[i].point.z, planes[i].normal.x, planes[i].normal.y, planes[i].normal.z);
		i++;
	}
	printf("\n");
}

void print_leafs(t_leaf *leafs, int nb_leafs)
{
	int i;
	printf("NB leafs %d\n", nb_leafs);
	i = 0;
	while (i < nb_leafs)
	{
		printf("Leaf (%d) %d - %d Box: %f %f %f - %f %f %f pvs_index: %d\n",i, leafs[i].start_poly, leafs[i].end_poly - 1, leafs[i].bbox.boxmin.x, leafs[i].bbox.boxmin.y, leafs[i].bbox.boxmin.z
			,leafs[i].bbox.boxmax.x, leafs[i].bbox.boxmax.y, leafs[i].bbox.boxmax.z, leafs[i].pvs_index);
		i++;
	}
	printf("\n");
}

void print_portals(t_portal **portal, int nb_portals)
{
	int i;
	int j;

	printf("NB portals %d\n", nb_portals);
	i = 0;
	while (i < nb_portals)
	{
		printf("Portal (%d) Leafs: %d %d\n", i, portal[i]->leafs[0], portal[i]->leafs[1]);
		j = 0;
		while (j < portal[i]->nb_ver)
		{
			printf("P %d: %f %f %f\n", j, portal[i]->ver_list[j].x, portal[i]->ver_list[j].y, portal[i]->ver_list[j].z);
			j++;
		}
		i++;
	}
	printf("\n");
}

void print_nodes(t_node *node, int nb_nodes)
{
	int i;

	printf("NB nodes %d\n", nb_nodes);
	i = 0;
	while (i <= nb_nodes)
	{
		printf("Node (%d)\nIs leaf: %d Plane: %d\n Front: %d Back %d\n", i, node[i].isleaf, node[i].plane, node[i].front, node[i].back);
		i++;
	}
	printf("\n");
}

char	isleaf;
	int		plane;
	int		front;
	int		back;
	t_box	bbox;

void print_bsp(t_bsp *bsp)
{
	printf("\n");
	printf("Max nodes %d\n", bsp->max_nodes);
	printf("Max polys %d\n", bsp->max_polys);
	printf("Max planes %d\n", bsp->max_planes);
	printf("Max leafs %d\n", bsp->max_leafs);
	printf("Max portals %d\n", bsp->max_portals);

	print_polys(bsp->poly, bsp->nb_polys);
	print_planes(bsp->plane, bsp->nb_planes);
	print_leafs(bsp->leaf, bsp->nb_leafs);
	print_portals(bsp->portal, bsp->nb_portals);
	print_nodes(bsp->node, bsp->nb_nodes);
	print_pvs(bsp);
}

void print_pvs(t_bsp *bsp)
{
	int i;
	int j;
	int c;

	printf("\nPVS\nSize = %d\n", bsp->pvs_size);
	i = 0;
	c = 0;
	while (i < bsp->pvs_size)
	{
		j = 128;
		while (j > 0)
		{
			printf("%d", (bsp->pvs[i] / j) % 2);
			j /= 2;
		}
		printf(" %d\n", bsp->pvs[i]);
		i++;
		c++;
	}
	printf("\n");
}

void print_poly_list(t_poly *poly)
{
	int i;
	int j;
	printf("\n POLY LIST \n\n");
	while (poly != NULL)
	{
		printf("Polygon \n");
		printf("Vertices: (%d)\n", poly->nb_ver);
		j = 0;
		while (j < poly->nb_ver)
		{
			printf("Vertex: %f %f %f\n", poly->ver_list[j].x, poly->ver_list[j].y, poly->ver_list[j].z);
			j++;
		}
		printf("Indices: \n");
		j = 0;
		while (j < poly->nb_indices)
		{
			printf("%d ",poly->indices[j]);
			j++;
		}
		printf("Was splitter %d\n", poly->was_splitter);
		printf("\n");
		poly = poly->next;
	}
}