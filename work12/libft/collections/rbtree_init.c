/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rbtree_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:12:06 by ssoraka           #+#    #+#             */
/*   Updated: 2020/01/12 18:12:06 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collections_header.h"
#include "collections_help_header.h"

void	ft_init_rbtree(t_rbtr *tree, int (*func_cmp)(t_old_key *, t_new_key *),
		void (*func_del)(t_old_key *, t_old_elem *))
{
	if (tree)
	{
		tree->func_cmp = func_cmp;
		tree->func_del = func_del;
	}
}

t_rbtr	*ft_create_rbtree(int (*func_cmp)(t_old_key *, t_new_key *),
		void (*func_del)(t_old_key *, t_old_elem *))
{
	t_rbtr	*tree;

	if (!func_cmp)
		return (NULL);
	tree = ft_memalloc(sizeof(t_rbtr));
	ft_init_rbtree(tree, func_cmp, func_del);
	return (tree);
}
