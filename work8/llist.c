/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arr.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:12:06 by ssoraka           #+#    #+#             */
/*   Updated: 2020/01/12 18:12:06 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"


t_llist	*ft_create_llist(void (*func_del)(void *, int))
{
	t_llist	*list;

	list = ft_memalloc(sizeof(t_llist));
	if (list)
	{
		list->start.next = (&list->end);
		list->start.prev = (&list->end);
		list->end.next = (&list->start);
		list->end.prev = (&list->start);
		list->func_del = func_del;
	}
	return (list);
}


void	ft_insert_lnode(t_lnode *node, t_lnode *prev, t_lnode *next)
{
	node->next = next;
	node->prev = prev;
	prev->next = node;
	next->prev = node;
}

void	ft_cut_lnode(t_lnode *node)
{
	t_lnode *prev;
	t_lnode *next;

	prev = node->prev;
	next = node->next;
	prev->next = next;
	next->prev = prev;
}

void	ft_for_each_llist(t_llist *list, void (*func)(void *, void *), void *param)
{
	t_lnode *node;

	if (!list || !func)
		return ;
	node = list->start.next;
	while (node != &(list->end))
	{
		func(node->elem, param);
		node = node->next;
	}
}

void	ft_del_lnode(t_llist *list, t_lnode **node)
{
	ft_cut_lnode(*node);
	if (list->func_del)
		list->func_del((*node)->elem, (*node)->info);
	ft_memdel((void **)node);
	(list->count)--;
}

void	ft_del_llist(t_llist **list)
{
	t_lnode *node;
	t_lnode *end;
	t_lnode *tmp;

	if (!list || !(*list))
		return ;
	node = (*list)->start.next;
	end = &((*list)->end);
	while (node != end)
	{
		tmp = node->next;
		ft_del_lnode(*list, &node);
		node = tmp;
	}
	ft_memdel((void **)list);
}


void	*ft_llist_add(t_llist *list, void *elem)
{
	t_lnode *tmp;

	if (!list || !elem)
		return (NULL);
	if ((tmp = ft_memalloc(sizeof(t_lnode))))
	{
		(list->count)++;
		tmp->elem = elem;
		ft_insert_lnode(tmp, list->end.prev, &(list->end));
	}
	return (tmp);
}



void	*ft_llist_get_next(t_llist *list)
{
	if (!list->current)
		list->current = list->start.next;
	else if (list->current->next != &(list->end))
		list->current = list->current->next;
	else
	{
		list->current = NULL;
		return (NULL);
	}
	return (list->current->elem);
}


// вот это ваще надо тестить...
void	*ft_lnode_get(t_llist *list, int num)
{
	t_lnode *node;

	if (!list || num < 0 || num >= list->count)
		return (NULL);
	if (num <= list->count >> 1)
	{
		node = list->start.next;
		while (num > 0)
		{
			node = node->next;
			num--;
		}
	}
	else
	{
		node = list->end.prev;
		while (num < list->count - 1)
		{
			node = node->prev;
			num++;
		}
	}
	return (node);
}

void	*ft_llist_get(t_llist *list, int num)
{
	t_lnode *node;

	node = ft_lnode_get(list, num);
	if (node)
		return (node->elem);
	return (NULL);
}

void	ft_llist_del_elem(t_llist *list, int num)
{
	t_lnode *node;

	if (!list->count)
		return ;
	node = ft_lnode_get(list, num);
	if (node)
		ft_del_lnode(list, &node);
}


void	*ft_llist_cut_elem(t_llist *list, void *elem)
{
	t_lnode *node;
	t_lnode *end;

	if (!list->count)
		return (NULL);
	node = list->start.next;
	end = &(list->end);
	while (node != end)
	{
		if (node->elem == elem)
		{
			ft_cut_lnode(node);
			free(node);
			(list->count)--;
			return (elem);
		}
		node = node->next;
	}
	return (NULL);
}

void	ft_del_llist_elems(t_llist *list, int (*need_del)(void *))
{
	t_lnode *node;
	t_lnode *tmp;

	if (!list || !need_del)
		return ;
	node = list->start.next;
	while (node != &(list->end))
	{
		tmp = node->next;
		if (need_del(node->elem))
			ft_del_lnode(list, &node);
		node = tmp;
	}
}



/*
**	для стеков и очередей
*/

void	*ft_llist_dpush(t_llist *list, void *elem)
{
	return (ft_llist_add(list, elem));
}

void	*ft_llist_push(t_llist *list, void *elem)
{
	t_lnode *tmp;

	if (!list || !elem)
		return (NULL);
	if ((tmp = ft_memalloc(sizeof(t_lnode))))
	{
		(list->count)++;
		tmp->elem = elem;
		ft_insert_lnode(tmp, &(list->start), list->start.next);
	}
	return (tmp);
}

void	*ft_llist_pop(t_llist *list)
{
	t_lnode *node;
	void *elem;

	if (!list || !list->count)
		return (NULL);
	(list->count)--;
	node = list->start.next;
	elem = node->elem;
	ft_cut_lnode(node);
	free(node);
	return (elem);
}

void	*ft_llist_dpop(t_llist *list)
{
	t_lnode *node;
	void *elem;

	if (!list || !list->count)
		return (NULL);
	(list->count)--;
	node = list->end.prev;
	elem = node->elem;
	ft_cut_lnode(node);
	free(node);
	return (elem);
}
