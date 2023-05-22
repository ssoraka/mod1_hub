/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rbtree_del.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:12:06 by ssoraka           #+#    #+#             */
/*   Updated: 2020/01/12 18:12:06 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collections_header.h"
#include "collections_help_header.h"

void	ft_del_rbtree_nodes(t_rbtr *tree)
{
	if (tree->root.left)
		ft_tnode_del_all(tree->root.left, tree->func_del);
}

void	ft_del_rbtree(t_rbtr **tree)
{
	ft_del_rbtree_nodes(*tree);
	ft_memdel((void **)tree);
}
