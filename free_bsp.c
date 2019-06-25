#include "bsp.h"

void free_poly_attr(t_poly *p)
{
	free(p->ver_list);
	free(p->indices);
	//free(p);
}

void free_poly(t_poly *p)
{
	free(p->ver_list);
	free(p->indices);
	free(p);
}

void free_poly_rec(t_poly *p)
{
	if (p == NULL)
		return ;
	if (p->next != NULL)
		free_poly_rec(p->next);
	free_poly(p);
}

void free_bsp(t_bsp *bsp)
{
	int i;
	t_poly *tmp;

	i = 0;
	while(i < bsp->nb_polys)
		free_poly_attr(&bsp->poly[i++]);
	free(bsp->poly);
	free_poly_rec(bsp->poly_list);
	free(bsp->node);
	free(bsp->leaf);
	free(bsp->plane);
	i = 0;
	while(i < bsp->nb_portals)
		free_poly((t_poly*)bsp->portal[i++]);
	free(bsp);
}