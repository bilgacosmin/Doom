#include "bsp.h"

int main()
{
	t_bsp *bsp;
	bsp_init(bsp);
	print_nodes(bsp->poly_list);
	build_bsp_tree(0, bsp);
	print_nodes(bsp->poly_list);
	printf("%d %d %d\n", bsp->nb_polys, bsp->nb_nodes, bsp->nb_leafs);
	printf("ON A SURVECU\n");
	print_bsp(bsp);
	return(0);
}