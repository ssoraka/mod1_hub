/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/04 16:23:42 by ssoraka           #+#    #+#             */
/*   Updated: 2019/03/07 16:22:42 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_mod1.h"

t_bool	is_visible(int pic_size, t_point p)
{
	pic_size = pic_size / 2;
	if (p.x + pic_size < 0 || p.y + pic_size < 0
		|| p.x - pic_size >= CONST_WIDTH || p.y - pic_size >= CONST_HEINTH)
		return (FALSE);
	return (TRUE);
}

void	print_img_as_water(t_arr *ipoints, t_vis *vis, t_pict *from)
{
	t_iter	iter;
	t_ipart	*ipart;
	t_shape	printer;
	t_pict	*to;
	t_vektr	v;

	ft_bzero((void *)&v, sizeof(t_vektr));
	ft_init_shape(&printer, IMAGE,
		set_param(DEFAULT_IMAGE, DEFAULT_INDEX, WATER_COLOR));
	printer.pic = from;
	to = &vis->pic;
	iter = get_arr_iter(ipoints);
	while (iter.get_next_elem(&iter))
	{
		ipart = (t_ipart *)iter.value;
		ft_fill_dpoint(&v.abs, ipart->pos.y, ipart->pos.x, ipart->pos.z);
		ft_rotate_point_around_point(&(vis->param), &v);
		printer.params.index = ipart->type;
		if (printer.params.index != NOTHING
			&& is_visible(from[ipart->type].size_line, v.zoom))
			printer.print(to, &v.zoom, &printer);
	}
}

void	print_rect_as_water(t_arr *ipoints, t_vis *vis)
{
	t_iter	iter;
	t_ipart	*ipart;
	t_shape	printer;
	t_pict	*to;
	t_vektr	v;

	ft_bzero((void *)&v, sizeof(t_vektr));
	ft_init_shape(&printer, RECTANGLE,
		set_param(DEFAULT_IMAGE, WATER, WATER_COLOR));
	printer.len = RADIUS;
	to = &vis->pic;
	iter = get_arr_iter(ipoints);
	while (iter.get_next_elem(&iter))
	{
		ipart = (t_ipart *)iter.value;
		ft_fill_dpoint(&v.abs, ipart->pos.y, ipart->pos.x, ipart->pos.z);
		ft_rotate_point_around_point(&(vis->param), &v);
		printer.color = (ipart->type == WATER) * WATER_COLOR
			+ (ipart->type == MAGMA) * MAGMA_COLOR;
		if (printer.params.index != NOTHING)
			printer.print(to, &v.zoom, &printer);
	}
}

void	ft_print_water(t_arr *iparts, t_vis *vis)
{
	if (vis->param.print_sprite == SIMPLE_WATER)
	{
		print_rect_as_water(iparts, vis);
		return ;
	}
	if (!ft_init_picture(vis->fluids + WATER, &vis->param, WATER_COLOR)
		|| !ft_init_picture(vis->fluids + MAGMA, &vis->param, MAGMA_COLOR))
	{
		ft_memdel((void **)&vis->fluids[WATER].addr);
		ft_memdel((void **)&vis->fluids[MAGMA].addr);
		ft_del_all("malloc error\n");
	}
	print_img_as_water(iparts, vis, vis->fluids);
	ft_memdel((void **)&vis->fluids[WATER].addr);
	ft_memdel((void **)&vis->fluids[MAGMA].addr);
}

void	ft_refresh_picture(t_vis *vis, t_arr *cell, t_arr *ipart)
{
	mlx_clear_window(vis->mlx, vis->win);
	ft_clear_image(&(vis->pic));
	if (vis->param.is_water_change)
	{
		ft_move_water_cell(cell, &(vis->param));
		vis->param.is_water_change = FALSE;
	}
	ft_print_relief(g_earth, cell, &(vis->pic), &(vis->param));
	if (vis->param.is_need_print_water)
		ft_print_water_cell(cell, &(vis->pic), &(vis->param));
	ft_print_water(ipart, vis);
	mlx_put_image_to_window(vis->mlx, vis->win, vis->pic.img, 0, 0);
}
