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
		printf("Leaf (%d) %d - %d\n",i, leafs[i].start_poly, leafs[i].end_poly);
		i++;
	}
	printf("\n");
}

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
	//print_portals(bsp->portals);
	/*int			max_nodes;
	int			max_polys;
	int			max_planes;
	int			max_leafs;
	int			max_portals;
	t_poly		*poly;  //liste de polygons naive
	t_node		*node;
	t_leaf		*leaf;
	t_plane		*plane;
	//t_portal	**portal;
	//BYTE *PVSData;
	int			nb_polys;
	int			nb_nodes;
	int			nb_leafs;
	int			nb_planes;
	int			nb_portals;*/
}

void print_nodes(t_poly *poly)
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