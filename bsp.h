/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbilga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/15 08:22:39 by cbilga            #+#    #+#             */
/*   Updated: 2019/03/15 18:39:05 by cbilga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BSP_H
# define BSP_H

typedef struct	s_vec //vector
{
	float x;
	float y;
	float z;
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
	t_ver		*ver_list;
	t_vec		normal;
	int			nb_ver;
	int			nb_indices;
	int			*indices;
	t_poly		*next;
	char		was_splitter;
	int			texture;
}				t_poly;

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
	t_poly		*poly;
	t_node		*node;
	t_leaf		*leaf;
	t_plane		*plane;
	//t_portal	**portal;
	//BYTE *PVSData;
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

#endif
