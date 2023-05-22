/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_smooth_earth.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 23:24:26 by ssoraka           #+#    #+#             */
/*   Updated: 2020/02/29 23:24:26 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_mod1.h"

void	ft_print_smooth_relief(t_earth *earth, t_pict *pic, t_param *param)
{
	t_vektr	*v;
	t_plgn	*plgn;
	t_iter	iter;
	t_prop	prop;

	iter = get_arr_iter(earth->points);
	while (iter.get_next_elem(&iter))
	{
		v = (t_vektr *)iter.value;
		ft_rotate_point_around_point(param, v);
	}
	prop = set_param(DEFAULT_POINT | (GRADIENT * param->grad), 0,
			OBSTACLES_FRONT_COLOR);
	iter = get_arr_iter(earth->poligons);
	while (iter.get_next_elem(&iter))
	{
		plgn = (t_plgn *)iter.value;
		ft_prepare_plgn_for_printing(plgn, param);
		ft_print_plgn(plgn, pic, prop);
	}
}

void	ft_print_relief(t_earth *earth, t_arr *cells, t_pict *pic,
						t_param *param)
{
	if (!param->is_need_print_obstacles)
		return ;
	if (param->need_refresh)
	{
		param->need_refresh = FALSE;
		if (param->is_smooth_relief == SMOOTH_EARTH)
			ft_print_smooth_relief(earth, pic, param);
		ft_print_all_cell(cells, pic, param);
		if (param->is_smooth_relief == COLORED_EARTH)
			ft_memcpy((void *)pic->addr, (void *)pic->index, pic->count_byte);
		ft_save_image(pic);
	}
	else
		ft_return_image(pic);
}
