#include "bsp.h"

int main()
{
	t_bsp *bsp;
	bsp_init(bsp);
	//print_poly_list(bsp->poly_list);
	build_bsp_tree(0, bsp);
	//print_nodes(bsp->poly_list);
	//printf("%d %d %d\n", bsp->nb_polys, bsp->nb_nodes, bsp->nb_leafs);
	printf("ON A SURVECU\n");
	build_portal(bsp);
	if (!(bsp->pvs = (char*)malloc(bsp->nb_leafs * ((bsp->nb_leafs + 7) / 8))))
		exit(0);
	ft_bzero(bsp->pvs, bsp->nb_leafs * ((bsp->nb_leafs + 7) / 8));
	printf("Before PVS\n");
	bsp->pvs_size = calc_pvs(bsp);
	printf("PVS DONE %d\n", bsp->pvs_size);
	//PVSCompressedSize=CalculatePVS();	
	//PVSData=(BYTE *) realloc(PVSData,PVSCompressedSize);
	print_bsp(bsp);
	//print_portals(bsp->portal, bsp->nb_portals);
	return(0);
}