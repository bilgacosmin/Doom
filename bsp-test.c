#include "bsp.h"

void bsp_error(t_bsp *bsp, char *s)
{
	ft_putstr(s);
	free_bsp(bsp);
}

int bsp_test()
{
	t_bsp *bsp;
	t_vec *pos;
	t_vec *pos2;
	
	bsp = bsp_compile();
	pos = create_vec(1,3,0);
	pos2 = create_vec(26,3,0);
	//print_poly_list(render_bsp(bsp,pos));
	if (line_of_sight(bsp, pos, pos2, 0))
		printf("IN LOS\n");
	else
		printf("NOT IN LOS\n");
	free(pos);
	free(pos2);
	//free_bsp(bsp);
	//while(1);
	return(0);
}

t_bsp *bsp_compile()
{	
	t_bsp *bsp;

	if (!(bsp = (t_bsp*)malloc(sizeof(t_bsp))))
		bsp_error(NULL, "BSP allocation failed");
	bsp_init(bsp);
	//printf("init done\n");
	build_bsp_tree(0, bsp);
	//printf("tree built\n");
	build_portal(bsp);
	//printf("portal built\n");
	if (!(bsp->pvs = (char*)malloc(bsp->nb_leafs * ((bsp->nb_leafs + 7) / 8))))
		exit(0);
	ft_bzero(bsp->pvs, bsp->nb_leafs * ((bsp->nb_leafs + 7) / 8));
	bsp->pvs_size = calc_pvs(bsp);
	print_bsp(bsp);
	return (bsp);
}