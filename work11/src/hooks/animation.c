/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 18:28:19 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/27 18:28:20 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_mod1.h"

int		ft_move_camera(t_param *param)
{
	if (param->target_x == param->cam_x && param->target_y == param->cam_y)
		return (FALSE);
	param->cam_x = param->target_x;
	param->cam_y = param->target_y;
	return (TRUE);
}

int		ft_is_cam_moved(t_param *param)
{
	if (param->right_button_press)
	{
		param->target_x = param->target_x + param->mouse.x - param->pos.x;
		param->target_y = param->target_y + param->mouse.y - param->pos.y;
		param->pos.x = param->mouse.x;
		param->pos.y = param->mouse.y;
		return (TRUE);
	}
	return (FALSE);
}

int		ft_is_cam_rotated(t_param *param)
{
	double len;

	if (param->left_button_press)
	{
		len = sqrt(pow(param->mouse.y - param->first_pos.y, 2.0)
				+ pow(param->mouse.x - param->first_pos.x, 2.0));
		ft_fill_point(&param->rot_pos,
				param->mouse.y - param->first_pos.y,
				param->mouse.x - param->first_pos.x, 0);
		param->rounds = (int)(len / 25);
		return (TRUE);
	}
	return (FALSE);
}

int		ft_auto_rotate(t_param *param)
{
	t_dpoint v;

	if (param->rounds > 0)
	{
		(param->rounds)--;
		ft_fill_dpoint(&v, -param->rot_pos.x, param->rot_pos.y, 0.0);
		ft_rotate_xyz_around_v(&param->oxyz, &v, ROTATE_ANGLE);
		calc_light(param);
		return (TRUE);
	}
	return (FALSE);
}
