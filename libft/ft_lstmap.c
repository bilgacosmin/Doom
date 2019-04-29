/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbilga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/13 11:14:45 by cbilga            #+#    #+#             */
/*   Updated: 2018/11/16 13:45:42 by cbilga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	t_list	*nlst;

	if ((lst == NULL) || (f == NULL))
		return (NULL);
	nlst = malloc(sizeof(t_list));
	if (nlst == NULL)
		return (NULL);
	nlst = f(lst);
	if (lst->next == NULL)
		return (nlst);
	else
	{
		nlst->next = ft_lstmap(lst->next, f);
		return (nlst);
	}
	return (NULL);
}
