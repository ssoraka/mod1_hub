/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   images.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 18:28:19 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/27 18:28:20 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_mod1.h"

void	ft_init_params(t_param *param)
{
	param->cam_x = CAM_X;
	param->cam_y = CAM_Y;
	param->target_x = CAM_X;
	param->target_y = CAM_Y;
	param->len = (CONST_HEINTH / JMAX);
	param->pause = 1;
	param->is_need_print_obstacles = TRUE;
	param->is_smooth_relief = SMOOTH_EARTH;
	param->brush = 1;
	ft_fill_dpoint(&param->light, -1.0, 0.5, 1.0);
	ft_fill_point(&param->water, JMAX - 1, I0 + 1, K0 + 1);
	param->is_need_print_water = TRUE;
	ft_create_xyz(&(param->oxyz));
	ft_fill_point(&param->centr.zoom, CAM_Y, CAM_X, 0.0);
	ft_fill_dpoint(&param->centr.abs, JMAX / 2, IMAX / 2, KMAX / 2);
	param->update_mode = FALSE;
	ft_rotate_oxyz_around_v(param, &param->oxyz.oz, M_PI);
	param->need_refresh = TRUE;
	param->print_sprite = BALL_WATER;
}

int	ft_create_img(t_pict *pic, void *mlx, int width, int heigth)
{
	pic->img = mlx_new_image(mlx, width, heigth);
	if (!pic->img)
		return (FALSE);
	pic->addr = (int *)mlx_get_data_addr(pic->img, &(pic->bits_per_pixel),
			&(pic->size_line), &(pic->endian));
	if (!pic->addr)
		return (FALSE);
	pic->count_line = heigth;
	pic->count_byte = width * heigth * sizeof(int);
	pic->z_buffer = (int *)ft_memalloc(pic->count_byte);
	pic->addr_copy = (int *)ft_memalloc(pic->count_byte);
	pic->z_buffer_copy = (int *)ft_memalloc(pic->count_byte);
	pic->index = (int *)ft_memalloc(pic->count_byte);
	if (!pic->z_buffer || !pic->addr_copy || !pic->z_buffer_copy || !pic->index)
		return (FALSE);
	return (TRUE);
}

void	ft_destroy_img(t_pict *pic)
{
	ft_memdel((void **)&(pic->z_buffer));
	ft_memdel((void **)&(pic->addr_copy));
	ft_memdel((void **)&(pic->z_buffer_copy));
	ft_memdel((void **)&(pic->index));
}

t_vis	*ft_create_mlx(int width, int heigth, char *name)
{
	t_vis	*vis;

	vis = ft_memalloc(sizeof(t_vis));
	if (!vis)
		return (NULL);
	vis->mlx = mlx_init();
	if (!vis->mlx)
		return (ft_destroy_mlx(&vis));
	vis->win = mlx_new_window(vis->mlx, width, heigth, name);
	if (!vis->win)
		return (ft_destroy_mlx(&vis));
	if (!(ft_create_img(&(vis->pic), vis->mlx, width, heigth)))
		return (ft_destroy_mlx(&vis));
	ft_init_params(&(vis->param));
	return (vis);
}

t_vis	*ft_destroy_mlx(t_vis **vis)
{
	if (*vis)
	{
		ft_destroy_img(&((*vis)->pic));
		mlx_destroy_image((*vis)->mlx, (*vis)->pic.img);
		mlx_destroy_window((*vis)->mlx, (*vis)->win);
		ft_memdel((void **)vis);
	}
	return (NULL);
}
