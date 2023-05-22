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

void	ft_ilist_set_list(t_ilist *ilist, void *list, int elem_size)
{
	if (elem_size <= MAX_LIST_SIZE && elem_size > 0)
	{
		ft_memcpy8((void *)ilist->mem, list, elem_size);
		ilist->size = elem_size;
	}
	else
	{
		ft_bzero8((void *)ilist->mem, MAX_LIST_SIZE);
		ilist->size = 0;
	}
}
