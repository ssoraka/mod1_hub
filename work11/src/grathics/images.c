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

/*
** надо пачку защит маллока наставить ...
*/

void	ft_init_params(t_param *param)
{
	param->cam_x = CAM_X;
	param->cam_y = CAM_Y;
	param->target_x = CAM_X;
	param->target_y = CAM_Y;
	param->len = CONST_LEN;
	param->ang.z = M_PI;
	param->pause = 1;
	param->is_need_print_obstacles = TRUE;
	param->brush = 1;
	param->light.x = 0.5;
	param->light.y = -1;
	param->light.z = 1;
	param->water.x = I0 + 1;
	param->water.y = J0 + 1;
	param->water.z = K0 + 1;
	param->is_need_print_water = TRUE;
	ft_create_xyz(&(param->oxyz));
	param->centr.zoom = (t_point){.x = CAM_X, .y = CAM_Y, .z = 0};
	param->centr.abs = (t_dpoint){.x = IMAX / 2, .y = JMAX / 2, .z = KMAX / 2};
}

int		ft_create_img(t_pict *pic, void *mlx, int width, int heigth)
{
	if (!(pic->img = mlx_new_image(mlx, width, heigth)))
		return (FALSE);
	if (!(pic->addr = (int *)mlx_get_data_addr(pic->img, &(pic->bits_per_pixel), &(pic->size_line), &(pic->endian))))
		return (FALSE);
	pic->count_line = heigth;
	pic->count_byte = width * heigth * sizeof(int);
	if (!(pic->z_buffer = (int *)ft_memalloc(pic->count_byte)))
		return (FALSE);
	if (!(pic->addr_copy = (int *)ft_memalloc(pic->count_byte)))
		return (FALSE);
	if (!(pic->z_buffer_copy = (int *)ft_memalloc(pic->count_byte)))
		return (FALSE);
	if (!(pic->index = (int *)ft_memalloc(pic->count_byte)))
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
	t_vis *vis;

	if (!(vis = ft_memalloc(sizeof(t_vis))))
		return (NULL);
	if (!(vis->mlx = mlx_init()))
		return (ft_destroy_mlx(&vis));
	if (!(vis->win = mlx_new_window(vis->mlx, width, heigth, name)))
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
