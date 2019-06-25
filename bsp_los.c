#include "bsp.h"


int line_of_sight(t_bsp *bsp, t_vec *start, t_vec *end, int node)
{
	t_los los;

	//printf("LINE OF SIGHT\n");
	los.curr = &bsp->node[node];
	los.plane = &bsp->plane[los.curr->plane];
	los.point_a = class_point(start, los.plane);
	los.point_b = class_point(end, los.plane);
	//printf("LOS 1\n");
	if (los.point_a == 0 && los.point_b == 0)
	{
		if (los.curr->isleaf == 0)
			return line_of_sight(bsp, start, end, los.curr->front);
		else
			return (1);
	}
	//printf("LOS 2\n");
	if (los.point_a == 1 && los.point_b == -1)
	{
		if (los.curr->back == -1)
			return (0);
		los.inter.start = start;
		los.inter.end = end;
		los.inter.point = &los.plane->point;
		los.inter.normal = &los.plane->normal;//*
		get_inter(&los.inter);
		if (los.curr->isleaf == 0)
			return (line_of_sight(bsp, start, &los.inter.inter, los.curr->front)
					&& line_of_sight(bsp, end, &los.inter.inter, los.curr->back));
		else
			return (line_of_sight(bsp, end, &los.inter.inter, los.curr->back));

	}
	//printf("LOS 3\n");
	if (los.point_a == -1 && los.point_b == 1)
	{
		//printf("LOS 3.1\n");
		if (los.curr->back == -1)
			return (0);
		los.inter.start = start;
		los.inter.end = end;
		los.inter.point = &los.plane->point;
		los.inter.normal = &los.plane->normal;
		//printf("LOS 3.5\n");
		get_inter(&los.inter); //SEGFAULT HERE
		//printf("LOS 3.6\n");
		if (los.curr->isleaf == 0)
			return (line_of_sight(bsp, start, &los.inter.inter, los.curr->back)
					&& line_of_sight(bsp, end, &los.inter.inter, los.curr->front));
		else
			return (line_of_sight(bsp, start, &los.inter.inter, los.curr->back));

	}
	//printf("LOS 4\n");
	if (los.point_a == 1 || los.point_b == 1)
	{
		if (los.curr->isleaf == 0)
			return (line_of_sight(bsp, start, end, los.curr->front));
		else
			return (1);
	}
	else
	{
		if (los.curr->back == -1)
			return (0);
		else
			return (line_of_sight(bsp, start, end, los.curr->back));
	}
	return (1);
}