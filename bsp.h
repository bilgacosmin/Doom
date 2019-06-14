/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbilga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/15 08:22:39 by cbilga            #+#    #+#             */
/*   Updated: 2019/03/21 14:53:13 by cbilga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BSP_H
# define BSP_H

#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "get_next_line.h"
#include <math.h>

typedef struct	s_vec t_vec;
typedef struct	s_poly t_poly;
typedef struct	s_inter t_inter;
typedef struct 	s_portal t_portal;

typedef struct	s_vec //vector
{
	float	x;
	float	y;
	float	z;
	int		tx;
	int		ty;
}				t_vec;

typedef struct	s_ver //vertex
{
	float x;
	float y;
	float z;
}				t_ver;

typedef struct	s_box //bounding box
{
	t_vec boxmin;
	t_vec boxmax;
}				t_box;

typedef struct	s_poly //polygon
{
	t_vec		*ver_list;  //TAB
	t_vec		normal;
	int			nb_ver; //points du polygone
	int			nb_indices; 
	int			*indices; //liste de coordonnées des triangles qui forment le polygone
	t_poly		*next;
	char		was_splitter;
	int			texture;
}				t_poly;

typedef struct s_portal
{
	t_vec		*ver_list;
	t_vec		normal;
	int 		nb_ver;
	int 		nb_indices;
	int 		*indices;
	t_portal 	*next;
	t_portal 	*prev;
	int 		nb_leafs;
	int 		leafs[2];
}				t_portal;

typedef struct	s_node //bsp node
{
	char	isleaf;
	int		plane;
	int		front;
	int		back;
	t_box	bbox;
}				t_node;

typedef struct	s_plane //plane
{
	t_vec	point;
	t_vec	normal;
}				t_plane;

typedef	struct	s_leaf // leaf
{
	int		start_poly;
	int		end_poly;
	int		portal[50]; //??
	int		nbportals;
	int 	pvs_index;
	t_box	bbox;
}				t_leaf;

typedef struct	s_tree //used in build_bsp_tree
{
	t_poly	*test;
	t_poly  *front;
	t_poly  *back;
	t_poly  *next;
	t_poly  *front_split;
	t_poly  *back_split;
	t_poly	*temp;
	t_poly	*iter;
	t_poly 	*clone;
	t_vec	v1;
	t_vec	v2;
	t_vec	a;
	t_vec	b;
	float	result;
	char	class;
	int		node;
	int		count;
	t_box	leaf_box;
}				t_tree;

typedef struct	s_select_split //used in select_splitter
{
	t_poly		*splitter;
	t_poly		*current;
	t_poly		*selected;
	int			best_score;
	t_plane		split_plane;
	int			score;
	int			splits;
	int			backfaces;
	int			frontfaces;
	int			result;
}				t_select_split;

typedef struct	s_bsp //global use
{
	int			max_nodes;
	int			max_polys;
	int			max_planes;
	int			max_leafs;
	int			max_portals;
	t_poly 		*poly_list; //liste de poly naive
	t_poly		*poly;  //tableau de polys traités
	t_node		*node;
	t_leaf		*leaf;
	t_plane		*plane;
	t_portal	**portal; //tableau d'adresses de portails
	char	*pvs;
	int 		pvs_size;
	int			nb_polys;
	int			nb_nodes;
	int			nb_leafs;
	int			nb_planes;
	int			nb_portals;
}				t_bsp;

typedef struct	s_cpoly //used in class_poly
{
	int     infront;
	int     behind;
	int     onplane;
	float   result;
	t_vec	v1;
	t_vec	v2;
	t_vec	dir;
	int		i;
}				t_cpoly;

typedef struct	s_inter
{
	t_vec	*start;
	t_vec	*end;
	t_vec	*point;
	t_vec	*normal;
	t_vec	inter;
	t_vec	direction;
	t_vec	*l;
	float	line_len;
	float	dist;
	float	perc;
}				t_inter;

typedef struct s_split //used in split poly
{
	t_vec	front[10];
	t_vec	back[10];
	t_vec	*first;
	t_vec	*normal;
	t_vec	*intersect;
	t_vec	*point;
	t_vec	*a;
	t_vec	*b;
	t_vec	copy;
	int		f_count;
	int		b_count;
	int		loop;
	int		curr_ver;
	int		class;
	int 	i;
	//int		result;
	float	percent;
	t_inter	inter;
	float	deltax;
	float	deltay;
	float	texx;
	float	texy;
	int 	v0;
	int 	v1;
	int 	v2;
}				t_split;

typedef struct	s_spoly //deprec
{
	t_poly	*f;
	t_poly	*b;
}				t_spoly;

typedef struct s_calcp
{
	t_vec	maxp;
	t_vec	minp;
	t_vec	cb; //bbox center
	t_vec	cp; //portal center
	t_vec	plane_normal; //+ u and v vector describe local axis
	t_vec 	a; //X, Y or Z axis used to find portal vectors
	t_vec	u; //u vector on portal
	t_vec	v; //v vector on normal
	float	length;
	float	dist_plane;
	t_portal *portal;
}				t_calcp;

typedef struct s_pclip
{
	t_portal *p_list;
	t_portal *fp_list;
	t_portal *bp_list;
	t_portal *iter;
	t_portal *back_split;
	t_portal *front_split;
	t_portal *tmp;
	int result;
}				t_pclip;

typedef struct s_chk_dup
{
	int check1;
	int check2;
	int i;
	t_vec *min1;
	t_vec *min2;
	t_vec *max1;
	t_vec *max2;
	t_portal *tmp;
	float size_new;
	float size_old;
}				t_chk_dup;

/// test structs 

typedef struct 	s_nodestack 
{
	int node;
	int jumpback;
}				t_nodestack;

typedef struct s_buildp
{
	int stackpointer;
	int portalindex;
	int i;
	int index;
	t_nodestack *nodestack;
	t_portal *init;
	t_portal *p_list;
	t_portal *iter;
	t_portal *tmp;
}				t_buildp;

typedef struct s_rec
{
	int generator_leaf;
	t_vec *slc;
	t_vec *tlc;
	t_portal *source_portal;
	t_portal *generator_portal;
	int gp_index;
	int sll;
	int tll;
	int gen_location;
	t_plane tmp_plane;
	t_portal *source_copy;
	t_portal *target_copy;
	int 	target_leafc;	
}				t_rec;

typedef struct s_cpvs
{
	char 	*leaf_pvs;
	long 	pvs_pointer;
	int 	leaf;
	int 	sp_index;
	int 	dp_index;
	t_portal *source_portal;
	t_portal *target_portal;
	t_plane temp_plane;																																																						
	int 	target_leaf;
}				t_cpvs;

typedef struct s_clipplanes
{
	int nb_planes;
	t_plane *plane;
}				t_clipplanes;

typedef struct s_cap
{
	t_vec edge1;
	t_vec edge2;
	t_vec normal;
	t_portal *front_split;
	t_portal *back_split;
	t_portal *temp_source;
	t_portal *temp_target;
	t_clipplanes clipplanes;
	t_plane  temp_plane;
	int   	portal_location;
	int 	s_portal_location;
	int 	next_vertex;
	int 	i;
	int 	sv; //source vertex
	int 	tmp;
}				t_cap;

typedef struct s_cl 
{
	int i;
	int rep;
	char *dest;
	char *dest_p;
	int bytes_per_set;
}				t_cl;

void 	add_vertices(t_poly *new, char *line, int i);
void 	add_poly(t_poly **list, char *line);
t_vec	*create_vec(float x, float y, float z);
void	bsp_init(t_bsp *bsp);
void	inc_polys(t_bsp *bsp);
void	inc_nodes(t_bsp *bsp);
void    inc_leafs(t_bsp *bsp);
void    inc_planes(t_bsp *bsp);
void	inc_portals(t_bsp *bsp);
void	calc_box(t_box *box, t_poly *p);
void	free_poly(t_poly *p);
int		class_point(t_vec *pos, t_plane *plane);
void	build_bsp_tree(int node, t_bsp *bsp);
void	init_inter(t_inter *inter, t_split	*split);
int		get_inter(t_inter *inter);
int		select_splitter(t_bsp *bsp);
void	split_poly(t_poly *poly, t_plane *plane, t_poly *front, t_poly *back);
t_poly 	*loadMap(char *file);
void 	add_poly(t_poly **list, char *line);
int 	count_vertices(char *line, int i);
void 	add_vertices(t_poly *new, char *line, int i);
void	create_normal(t_poly *list);
void	triangulize(t_poly *list);
char	class_poly(t_plane *plane, t_poly *poly);
int 	ft_abs(int i);
void	print_bsp(t_bsp *bsp);
void	print_nodes(t_node *node, int nb_nodes);
t_poly 	*clone_poly(t_poly *poly);
void 	print_planes(t_plane *planes, int nb_planes);
void	print_polys(t_poly *poly, int nb_polys);
void	vec_cross_prod(t_vec* a, t_vec *b, t_vec *c);
t_portal *calculate_init_portal(t_bsp *bsp, int node);
float	vec_dot_prod(t_vec *a, t_vec *b);
void	vec_normalize(t_vec *a, t_vec *b);
void	init_vec(t_vec *vec);
void	set_vec(t_vec *new, float x, float y, float z);
void	init_portal(t_portal *portal);
t_portal *clip_portal(t_bsp *bsp, int node, t_portal *portal);
void	delete_portal(t_portal *portal);
void	print_portals(t_portal **portal, int nb_portals);
//void	build_portal(t_bsp *bsp, t_bportal *bportal);
void 	build_portal(t_bsp *bsp);
void 	build_portal_start(t_bsp *bsp, t_buildp *buildp);
void 	build_portal_back(t_bsp *bsp, t_buildp *buildp);
void	build_portal_end(t_bsp *bsp, t_buildp *buildp);
void 	print_poly_list(t_poly *poly);
void	split_portal(t_portal *front, t_portal *back, t_portal *portal, t_plane *plane);
t_portal *portal_copy(t_portal *portal);
t_plane get_portal_plane(t_portal *portal);
t_portal *clip_anti_penumbra(t_portal *source_portal, t_portal *target_portal, t_portal *generator_portal);
void 	recurse_pvs(t_bsp *bsp, t_cpvs *cpvs);
void 	set_pvs_bit(char *pvs, int leaf);
int 	compress_leaf(t_bsp *bsp, char *leaf_pvs, int position);
int 	calc_pvs(t_bsp *bsp);
void  	delete_pointer(void **pointer);
void 	print_pvs(t_bsp *bsp);

#endif
