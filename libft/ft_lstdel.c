/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbilga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/13 10:54:03 by cbilga            #+#    #+#             */
/*   Updated: 2018/11/16 13:46:54 by cbilga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdel(t_list **alst, void (*del)(void *, size_t))
{
	if ((alst == NULL) || (del == NULL))
		return ;
	if ((*alst) != NULL)
	{
		del((*alst)->content, (*alst)->content_size);
		if ((*alst)->next != NULL)
			ft_lstdel(&((*alst)->next), del);
		free(*alst);
		*alst = NULL;
	}
}
