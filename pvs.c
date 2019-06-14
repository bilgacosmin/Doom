#include "bsp.h"

void init_cpvs(t_bsp *bsp, t_cpvs *cpvs)
{
	if (!(cpvs->leaf_pvs = (char *)malloc(bsp->nb_leafs * ((bsp->nb_leafs + 7) / 8))))
		exit(0);
	cpvs->pvs_pointer = 0;
	cpvs->leaf = 0;
	cpvs->sp_index = 0;
}

int calc_pvs(t_bsp *bsp)
{
	t_cpvs cpvs;

	//printf("CPVS\n");
	init_cpvs(bsp, &cpvs);
	while (cpvs.leaf < bsp->nb_leafs)
	{
		//printf("CPVS 1\n");
		bsp->leaf[cpvs.leaf].pvs_index = cpvs.pvs_pointer;
		ft_bzero(cpvs.leaf_pvs, (bsp->nb_leafs + 7) / 8);
		//printf("CPVS 2\n");
		set_pvs_bit(cpvs.leaf_pvs, cpvs.leaf);
		//printf("CPVS 2.1\n");
		cpvs.sp_index = 0;
		while(cpvs.sp_index < bsp->leaf[cpvs.leaf].nbportals)
		{
			//printf("CPVS 3\n");
			cpvs.source_portal = bsp->portal[bsp->leaf[cpvs.leaf].portal[cpvs.sp_index]];
			cpvs.target_leaf = cpvs.source_portal->leafs[0];
			if (cpvs.target_leaf == cpvs.leaf)
				cpvs.target_leaf = cpvs.source_portal->leafs[1];
			set_pvs_bit(cpvs.leaf_pvs, cpvs.target_leaf); ///??
			cpvs.dp_index = 0;
			while(cpvs.dp_index < bsp->leaf[cpvs.target_leaf].nbportals)
			{
				//printf("CPVS4\n");
				cpvs.target_portal = bsp->portal[bsp->leaf[cpvs.target_leaf].portal[cpvs.dp_index]];
				cpvs.temp_plane = get_portal_plane(cpvs.source_portal);
				//printf("CPVS4.1\n");
				if (cpvs.source_portal != cpvs.target_portal && class_poly(&cpvs.temp_plane, (t_poly*)cpvs.target_portal) != 0)
					recurse_pvs(bsp, &cpvs); //??
				//printf("CPVS4.2\n");
				cpvs.dp_index++;
			}
			cpvs.sp_index++;
		}
		//printf("CPVS 5\n");
		cpvs.pvs_pointer += compress_leaf(bsp, cpvs.leaf_pvs, cpvs.pvs_pointer); //??
		//printf("CPVS 6\n");
		cpvs.leaf++;
	}
	free(cpvs.leaf_pvs);
	//printf("CPVS DONE\n");
	return (cpvs.pvs_pointer);
}

void set_pvs_bit(char *pvs, int leaf) // sets the bit of corresponding leaf to 1
{
	int byte_to_set;
	char bit_to_set;

	byte_to_set = leaf / 8;
	//printf("SET BIT %d to leaf %d\n", byte_to_set, leaf);
	//byte_to_set = leaf / 8;
	bit_to_set = (char)(leaf - (byte_to_set * 8));
	pvs[byte_to_set] = pvs[byte_to_set]|(1<<bit_to_set);
	//printf("%d\n", pvs[byte_to_set]); 
}

void recurse_pvs(t_bsp *bsp, t_cpvs *cpvs)
{
	t_rec rec;

	//printf("REC\n");
	rec.generator_leaf = cpvs->target_portal->leafs[0];
	if (rec.generator_leaf == cpvs->target_leaf)
		rec.generator_leaf = cpvs->target_portal->leafs[1];
	set_pvs_bit(cpvs->leaf_pvs, rec.generator_leaf);
	rec.slc = create_vec((bsp->leaf[cpvs->leaf].bbox.boxmax.x + bsp->leaf[cpvs->leaf].bbox.boxmin.x) / 2,
						(bsp->leaf[cpvs->leaf].bbox.boxmax.y + bsp->leaf[cpvs->leaf].bbox.boxmin.y) / 2,
						(bsp->leaf[cpvs->leaf].bbox.boxmax.z + bsp->leaf[cpvs->leaf].bbox.boxmin.z) / 2);
	rec.tlc = create_vec((bsp->leaf[cpvs->target_leaf].bbox.boxmax.x + bsp->leaf[cpvs->target_leaf].bbox.boxmin.x) / 2,
						(bsp->leaf[cpvs->target_leaf].bbox.boxmax.y + bsp->leaf[cpvs->target_leaf].bbox.boxmin.y) / 2,
						(bsp->leaf[cpvs->target_leaf].bbox.boxmax.z + bsp->leaf[cpvs->target_leaf].bbox.boxmin.z) / 2);
	rec.tmp_plane = get_portal_plane(cpvs->source_portal);
	rec.sll = class_point(rec.slc, &rec.tmp_plane);
	rec.tmp_plane = get_portal_plane(cpvs->target_portal);
	rec.tll = class_point(rec.tlc, &rec.tmp_plane);
	rec.gp_index = -1;
	rec.source_portal = NULL;
	rec.generator_portal = NULL;
	while(rec.gp_index < bsp->leaf[rec.generator_leaf].nbportals)
	{
		rec.gp_index++;
		//printf("REC 1 %p %p \n", bsp->portal[bsp->leaf[rec.generator_leaf].portal[rec.gp_index]], cpvs->target_portal);
		if (bsp->portal[bsp->leaf[rec.generator_leaf].portal[rec.gp_index]] == cpvs->target_portal)
		{
			//printf("REC 2\n");
			//delete_pointer(&(rec.generator_portal));
			//delete_pointer(&(rec.source_portal));
			if (rec.source_portal != NULL)
				free(rec.source_portal);
			rec.source_portal = NULL;
			if (rec.generator_portal != NULL)
				free(rec.generator_portal);
			rec.generator_portal = NULL;
			continue;
		}
		//printf("REC 1.1\n");
		delete_pointer(&(rec.source_portal));
		//printf("REC 1.2\n");
		if (cpvs->source_portal != NULL)
			rec.source_portal = portal_copy(cpvs->source_portal);
		else
		{
			rec.source_portal = NULL;
			printf("ERROR, CPVS->SOURCE IS NULL\n");
		}
		delete_pointer(&(rec.generator_portal));
		if (bsp->portal[bsp->leaf[rec.generator_leaf].portal[rec.gp_index]] != NULL)
			rec.generator_portal = portal_copy(bsp->portal[bsp->leaf[rec.generator_leaf].portal[rec.gp_index]]);
		else
		{
			rec.generator_portal = NULL;
			printf("ERROR, REC GEN PORTAL IS NULL\n");
		}
		//printf("REC 1.2.2\n");
		rec.tmp_plane = get_portal_plane(rec.source_portal);
		//printf("REC 1.2.3\n");
		rec.gen_location = class_poly(&rec.tmp_plane, (t_poly*)rec.generator_portal);
		//printf("REC 1.2.4\n");
		if (rec.gen_location == 0 || rec.gen_location == rec.sll)
		{
		//	printf("REC 3\n");
			//delete_pointer(&(rec.generator_portal));
			//delete_pointer(&(rec.source_portal));
			if (rec.generator_portal != NULL)
				free(rec.generator_portal);
			rec.generator_portal = NULL;
			if (rec.source_portal != NULL)
				free(rec.source_portal);
			rec.source_portal = NULL;
		//	printf("REC 3 END\n");
			continue;
		}
		//printf("REC 3.1\n");
		rec.tmp_plane = get_portal_plane(cpvs->target_portal);
		rec.gen_location = class_poly(&rec.tmp_plane, (t_poly*)rec.generator_portal);
		if (rec.gen_location == 0 || rec.gen_location == rec.tll)
		{
		//	printf("REC 4\n");
			free(rec.generator_portal);
			rec.generator_portal = NULL;
			free(rec.source_portal);
			rec.source_portal = NULL;
			continue;
		}
		//printf("REC 5\n");
		rec.generator_portal = clip_anti_penumbra(rec.source_portal, cpvs->target_portal, rec.generator_portal);
		if (rec.generator_portal == NULL)
		{
			if (rec.source_portal)
			{
				free(rec.source_portal);
				rec.source_portal = NULL;
				continue;
			}
			continue;
		}
		rec.source_portal = clip_anti_penumbra(rec.generator_portal, cpvs->target_portal, rec.source_portal);
		if (rec.source_portal == NULL)
		{
			if (rec.generator_portal)
			{
				free(rec.generator_portal);
				rec.generator_portal = NULL;
				continue;
			}
		}
		rec.source_copy = cpvs->source_portal;
		rec.target_copy = cpvs->target_portal;
		rec.target_leafc = cpvs->target_leaf;
		recurse_pvs(bsp, cpvs);
		cpvs->source_portal = rec.source_copy;
		cpvs->target_portal = rec.target_copy;
		cpvs->target_leaf = rec.target_leafc;
		free(rec.generator_portal);
		rec.generator_portal = NULL;
		free(rec.source_portal);
		rec.source_portal = NULL;
		//rec.gp_index++;
	}
	//printf("REC DONE\n");
}

void init_cap(t_cap *cap, t_portal *source_portal, t_portal *target_portal)
{
	cap->clipplanes.nb_planes = 0;
	if (!(cap->clipplanes.plane = (t_plane*)malloc(sizeof(t_plane) * source_portal->nb_ver * target_portal->nb_ver * 2)))
		exit(0);
}

t_portal *clip_anti_penumbra(t_portal *source_portal, t_portal *target_portal, t_portal *generator_portal)
{
	t_cap cap;

	init_cap(&cap, source_portal, target_portal);
	cap.i = 0;
	while (cap.i < 2)
	{
		if (cap.i == 0)
		{
			cap.temp_source = source_portal;
			cap.temp_target = target_portal;
		}
		else
		{
			cap.temp_source = target_portal;
			cap.temp_target = source_portal;
		}
		cap.sv = 0;
		while (cap.sv < cap.temp_source->nb_ver)
		{
			cap.temp_plane = get_portal_plane(cap.temp_target);
			cap.portal_location = class_point(&(cap.temp_source->ver_list[cap.sv]), &(cap.temp_plane));
			if (cap.portal_location == 0)
				continue;
			cap.tmp = 0;
			while (cap.tmp < cap.temp_target->nb_ver)
			{
				if (cap.tmp == cap.temp_target->nb_ver - 1)
					cap.next_vertex = 0;
				else
					cap.next_vertex = cap.tmp + 1;
				set_vec(&(cap.edge1), cap.temp_source->ver_list[cap.sv].x - cap.temp_target->ver_list[cap.tmp].x,
						cap.temp_source->ver_list[cap.sv].y - cap.temp_target->ver_list[cap.tmp].y,
						cap.temp_source->ver_list[cap.sv].z - cap.temp_target->ver_list[cap.tmp].z);
				set_vec(&(cap.edge2), cap.temp_target->ver_list[cap.next_vertex].x - cap.temp_target->ver_list[cap.tmp].x,
						cap.temp_target->ver_list[cap.next_vertex].y - cap.temp_target->ver_list[cap.tmp].y,
						cap.temp_target->ver_list[cap.next_vertex].z - cap.temp_target->ver_list[cap.tmp].z);
				vec_cross_prod(&(cap.edge1), &(cap.edge2), &(cap.normal));
				vec_normalize(&(cap.normal), &(cap.temp_plane.normal));
				cap.temp_plane.point = cap.temp_source->ver_list[cap.sv];
				if (class_poly(&(cap.temp_plane), (t_poly*)cap.temp_source) == 1)
				{
					if (class_poly(&(cap.temp_plane), (t_poly*)cap.temp_target) == -1)
					{
						cap.clipplanes.plane[cap.clipplanes.nb_planes] = cap.temp_plane;
						cap.clipplanes.nb_planes++;
					}
				}
				else
				{
					if (class_poly(&(cap.temp_plane), (t_poly*)cap.temp_source) == -1)
					{
						if (class_poly(&(cap.temp_plane), (t_poly*)cap.temp_target) == 1)
						{
							cap.clipplanes.plane[cap.clipplanes.nb_planes] = cap.temp_plane;
							cap.clipplanes.nb_planes++;
						}
					}
				}
				cap.tmp++;
			}
			cap.sv++;
		}
		cap.i++;
	}
	cap.i = 0;
	while (cap.i < cap.clipplanes.nb_planes)
	{
		cap.portal_location = class_poly(&(cap.clipplanes.plane[cap.i]), (t_poly*)generator_portal);
		cap.s_portal_location = class_poly(&(cap.clipplanes.plane[cap.i]), (t_poly*)source_portal);
		if (cap.portal_location == cap.s_portal_location || cap.portal_location == 0)
		{
			free(generator_portal);
			generator_portal = NULL;
			free(cap.clipplanes.plane);
			cap.clipplanes.plane = NULL;
			return (NULL);
		}
		if ((cap.portal_location == -1 && cap.s_portal_location == 1) || (cap.portal_location == 1 && cap.s_portal_location == -1))
			continue;
		if (cap.portal_location == 2)
		{
			if (!(cap.front_split = (t_portal*)malloc(sizeof(t_portal))))
				exit(0);
			if (!(cap.front_split = (t_portal*)malloc(sizeof(t_portal))))
				exit(0);
			split_portal(cap.front_split, cap.back_split, generator_portal, &(cap.clipplanes.plane[cap.i]));
			if (cap.s_portal_location == 1)
			{
				free(cap.front_split);
				cap.front_split = NULL;
				free(generator_portal);
				generator_portal = cap.back_split;
			}
			else
			{
				if(cap.s_portal_location == -1)
				{
					free(cap.back_split);
					cap.back_split = NULL;
					free(generator_portal);
					generator_portal = cap.front_split;
				}
			}
		}
		cap.i++;
	}
	free(cap.clipplanes.plane);
	return (generator_portal);
}

int compress_leaf(t_bsp *bsp, char *leaf_pvs, int position)
{
	t_cl cl;

	//printf("CL\n");
	cl.dest = &(bsp->pvs[position]);
	cl.dest_p = cl.dest;
	cl.i = 0; //-1 ?
	cl.bytes_per_set = (bsp->nb_leafs + 7) / 8;
	//printf("CL 2 BPS = %d\n", cl.bytes_per_set);
	while (cl.i < cl.bytes_per_set)
	{
	//	printf("CL 3 %d %d\n", cl.i, leaf_pvs[cl.i]);
		//cl.i++;
		*(cl.dest_p) = leaf_pvs[cl.i];
		(cl.dest_p)++;  //??
		cl.i++;
		if(leaf_pvs[cl.i - 1])
			continue;
		cl.rep = 1;
		cl.i++;
		while (cl.i < cl.bytes_per_set)
		{
			//cl.i++;
			if (leaf_pvs[cl.i] || cl.rep == 255)
				break;
			else
				cl.rep++;
			cl.i++;
		}
		*(cl.dest_p) = cl.rep;
		cl.dest_p++;
		//cl.i++;
	}
	//printf("CL DONE\n");
	return (cl.dest_p - cl.dest);
}