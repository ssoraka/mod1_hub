/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   llist_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:12:06 by ssoraka           #+#    #+#             */
/*   Updated: 2020/01/12 18:12:06 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collections_header.h"
#include "collections_help_header.h"

void	ft_init_llist(t_llist *llist, void (*func_del)(void *))
{
	if (!llist)
		return ;
	llist->start.next = &llist->end;
	llist->start.prev = &llist->end;
	llist->end.next = &llist->start;
	llist->end.prev = &llist->start;
	llist->func_del = func_del;
	llist->elems_count = 0;
}

t_llist	*ft_create_llist(void (*func_del)(void *))
{
	t_llist	*llist;

	llist = ft_memalloc(sizeof(t_llist));
	if (llist)
		ft_init_llist(llist, func_del);
	return (llist);
}
