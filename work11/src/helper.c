/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/04 16:23:42 by ssoraka           #+#    #+#             */
/*   Updated: 2019/03/07 16:22:42 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_mod1.h"

void	ft_prepare_one_buffer(t_buff *buff)
{
	buff->work_size = buff->arr->elems_used;
	buff->buff_used = buff->work_size * buff->arr->elem_size;
}

void	ft_init_buffers(t_buff *buff, t_arr *arr)
{
	if (!arr->elems_used)
		arr->elems_used = 1;
	buff->arr = arr;
	buff->g_work_size = &(arr->elems_used);
	ft_prepare_one_buffer(buff);
}

t_bool	ft_copy_arrs(t_arr *dst, t_arr *src)
{
	if (!ft_realloc_arr(dst, src->elems_used))
		return (FALSE);
	dst->elems_used = src->elems_used;
	ft_memcpy8(dst->elems, src->elems, dst->elem_size * dst->elems_used);
	return (TRUE);
}

t_bool	del_elem(void *elem, void *param)
{
	t_part	*p;

	(void)param;
	p = (t_part *)elem;
	return (p->pos.x < I0 || p->pos.y < J0 || p->pos.z < K0
		|| p->pos.x > IMAX || p->pos.y > JMAX || p->pos.z > KMAX
		|| p->type == NOTHING);
}

void	ft_cycle_cube(void *param, void (*f)(void *, int, int, int),
						t_point *start, t_point *end)
{
	int		k;
	int		j;
	int		i;
	t_point	delta;

	delta.y = 1 + (end->y < start->y) * -2;
	delta.x = 1 + (end->x < start->x) * -2;
	delta.z = 1 + (end->z < start->z) * -2;
	j = start->y;
	while (j != end->y + delta.y)
	{
		i = start->x;
		while (i != end->x + delta.x)
		{
			k = start->z;
			while (k != end->z + delta.z)
			{
				f(param, j, i, k);
				k += delta.z;
			}
			i += delta.x;
		}
		j += delta.y;
	}
}
