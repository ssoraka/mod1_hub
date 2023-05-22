/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ilist_setters.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:12:06 by ssoraka           #+#    #+#             */
/*   Updated: 2020/01/12 18:12:06 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collections_header.h"
#include "collections_help_header.h"

void	ft_ilist_set_add(t_ilist *ilist, int (*add)(void *, void *, void *))
{
	ilist->add = add;
}

void	ft_ilist_set_get(t_ilist *ilist, void *(*find)(void *, void *))
{
	ilist->find = find;
}

void	ft_ilist_set_del(t_ilist *ilist, void (*del)(void *))
{
	ilist->del = del;
}

void	ft_ilist_set_iterator(t_ilist *list, t_iter (*iterator)(void *))
{
	list->iterator = iterator;
}

void	ft_ilist_set_func_for_resize_map(t_ilist *list,
		void (*del_list_without_key_value)(void *))
{
	list->del_list_without_key_value = del_list_without_key_value;
}
