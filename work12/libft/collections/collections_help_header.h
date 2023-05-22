/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collections_help_header.h                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/10 20:59:39 by ssoraka           #+#    #+#             */
/*   Updated: 2019/04/19 13:13:30 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLLECTIONS_HELP_HEADER_H
# define COLLECTIONS_HELP_HEADER_H

# include "collections_header.h"

# define ARR_REALLOC_COEF 1.5
# define ARR_FIRST_COUNT 16
# define SIZE_OF_PTR 8

# define HASHMAP_REALLOC_COEF 2.0
# define HASHMAP_FIRST_COUNT 16
# define HASHMAP_LOAD 0.75

/*
** lnode.c
*/
t_lnode	*ft_create_lnode(void *value);
void	ft_insert_lnode(t_lnode *node, t_lnode *before);
void	*ft_cut_lnode(t_lnode *node);
void	ft_del_lnode(t_lnode **node, void (*func_del)(void *));
t_lnode	*ft_lnode_get(int num, int count, t_lnode *start, t_lnode *end);

/*
** llist_init.c
*/
t_llist	*ft_create_llist(void (*func_del)(void *));
void	ft_init_llist(t_llist	*llist, void (*func_del)(void *));

/*
** llist_stack.c
*/
int		ft_llist_dpush(t_llist *list, void *elem);
int		ft_llist_push(t_llist *list, void *elem);
void	*ft_llist_pop(t_llist *list);
void	*ft_llist_dpop(t_llist *list);

/*
** llist_add.c
*/
int		ft_insert_llist(t_llist *list, void *elem, int num);
int		ft_llist_add(t_llist *list, void *elem);

/*
** llist_get.c
*/
void	ft_for_each_llist(t_llist *list,
			void (*func)(void *, void *), void *param);
void	*ft_llist_get(t_llist *list, int num);
void	*ft_llist_get_next(t_llist *list);

/*
** llist_del.c
*/
void	ft_del_llist_elem(t_llist *list, int num);
void	ft_del_llist(t_llist **list);

/*
** arr_init.c
*/
void	*ft_return_elem(void *elem);
void	*ft_return_ptr(void *elem);
int		ft_init_arr(t_arr *arr, size_t elem_size, size_t elems_count,
			void (*func_del)(void *));
t_arr	*ft_create_arr(size_t elem_size, size_t elems_count,
			void (*func_del)(void *));
t_arr	*ft_create_arr_of_ptr(size_t elems_count, void (*func_del)(void *));

/*
** arr_add.c
*/
t_bool	ft_realloc_arr(t_arr *arr, size_t new_count);
void	*ft_arr_add(t_arr *arr, void *elem);

/*
** arr_get.c
*/
void	*ft_arr_get_addr(t_arr *arr, size_t num);
void	*ft_arr_get(t_arr *arr, size_t num);
void	*ft_arr_get_next(t_arr *arr);

/*
** arr_del.c
*/
void	ft_del_arr(t_arr **arr);
void	ft_del_elem(t_arr *arr, size_t num);
void	ft_del_elems_if(t_arr *arr,
			t_bool (*need_del)(void *, void *), void *param);

/*
** arr_each.c
*/
void	ft_for_each_elem(t_arr *arr,
			void (*func)(void *, void *), void *param);
int		ft_arr_init_by_value(t_arr *arr, int count, void *value);
void	ft_all_arr_init_by_value(t_arr *arr, void *value);

/*
** rbtree_init.c
*/
t_rbtr	*ft_create_rbtree(int (*func_cmp)(t_old_key *, t_new_key *),
			void (*func_del)(t_old_key *, t_old_elem *));
void	ft_init_rbtree(t_rbtr *tree, int (*func_cmp)(t_old_key *, t_new_key *),
			void (*func_del)(t_old_key *, t_old_elem *));

/*
** rbtree_del.c
*/
void	ft_del_rbtree(t_rbtr **tree);
void	ft_del_rbtree_nodes(t_rbtr *tree);

/*
** rbtree_add_get.c
*/
void	ft_rbtree_insert(t_rbtr *tree, t_tnode *node);
t_bool	ft_rbtree_add(t_rbtr *tree, void *key, void *value);
void	*ft_rbtree_get_elem(t_rbtr *tree, void *key);

/*
** tnode_track.c
*/
void	*ft_rbtree_get_next(t_rbtr *tree);
void	ft_rbtree_prefix(t_rbtr *tree,
			void (*func)(t_old_elem *, void *), void *param);
void	ft_rbtree_postfix(t_rbtr *tree,
			void (*func)(t_old_elem *, void *), void *param);
void	ft_rbtree_suffix(t_rbtr *tree,
			void (*func)(t_old_elem *, void *), void *param);

/*
** tnode_init_del.
*/
t_tnode	*ft_create_tnode(void *key, void *value);
void	ft_tnode_del(t_tnode *node, void (*func)(void *, void *));
void	ft_tnode_del_all(t_tnode *node, void (*func)(void *, void *));

/*
** tnode_track
*/
t_tnode	*ft_find_left_value(t_tnode *node);
void	ft_tnode_prefix(t_tnode *node,
			void (*func)(void *, void *), void *param);
void	ft_tnode_postfix(t_tnode *node,
			void (*func)(void *, void *), void *param);
void	ft_tnode_infix(t_tnode *node,
			void (*func)(void *, void *), void *param);

/*
** tnode_rebalance.c
*/
void	ft_case_red_dad_red_uncle(t_tnode *node);
void	ft_case_red_right_dad_and_black_left_uncle(t_tnode *node);
void	ft_case_red_left_dad_and_black_right_uncle(t_tnode *node);
void	ft_tnode_rebalance(t_tnode *node);

/*
** tnode_rbtee_property.c
*/
int		ft_is_red_color(t_tnode *node);
int		ft_is_right_child(t_tnode *node);
t_tnode	*ft_get_uncle(t_tnode *node);
void	ft_tnode_left_rotate(t_tnode *node);
void	ft_tnode_right_rotate(t_tnode *node);

/*
** ilist_setters.c
*/
void	ft_ilist_set_add(t_ilist *ilist, int (*add)(void *, void *, void *));
void	ft_ilist_set_get(t_ilist *ilist, void *(*find)(void *, void *));
void	ft_ilist_set_del(t_ilist *ilist, void (*del)(void *));
void	ft_ilist_set_list(t_ilist *ilist, void *list, int elem_size);
void	ft_ilist_set_get_next(t_ilist *list,
			int (*get_next)(void *, void **, void **));
/*
** ilist_setters2.c
*/
void	ft_ilist_set_func_for_resize_map(t_ilist *list,
			void (*del_list_without_key_value)(void *));

/*
** hmap.c
*/
t_hmap	*ft_create_hashmap(int (*func_hash)(void *), t_ilist *list);
int		ft_hashmap_put(t_hmap *hmap, void *key, void *value);
void	*ft_hashmap_get(t_hmap *hmap, void *key);
void	ft_del_hmap(t_hmap **hmap);

/*
** hmap2.c
*/
int		ft_increase_hmap(t_hmap *hmap);
void	ft_del_arr_without_del_content(t_arr *arr, t_ilist *ilist);
t_arr	*ft_create_arr_of_elems(void *value, int elem_size, int elems_count,
			void (*func_del)(void *));

#endif
