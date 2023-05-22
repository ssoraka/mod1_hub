/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 18:12:08 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/27 18:12:08 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_mod1.h"

#define CAM_SCALE 1.1
#define MIN_SCALE 0.01
#define MAX_SCALE 1000.0

int	ft_change_brush(t_param *param, int key)
{
	if (key == KEY_PLUS && param->brush < IMAX / 2)
		param->brush++;
	else if (key == KEY_MINUS && param->brush > 1)
		param->brush--;
	else
		return (FALSE);
	return (TRUE);
}

void	change_gravity(t_param *param)
{
	param->update_mode = !param->update_mode;
	if (!param->update_mode)
	{
		ft_fill_dpoint(&param->g, -CONST_G, 0, 0);
		param->update_status = LAST_UPDATE_PARAM_ON_CL;
	}
	else
	{
		ft_rotate_oxyz_around_v(param, &param->oxyz.ox, 0.0);
	}
}

int	ft_csale_picture(t_param *param, int button, t_point *mouse)
{
	if (button == MIDDLE_FORW_BUTTON && param->len < MAX_SCALE)
		param->len *= CAM_SCALE;
	else if (button == MIDDLE_BACK_BUTTON && param->len > MIN_SCALE)
		param->len /= CAM_SCALE;
	else if (button == LEFT_BUTTON)
	{
		ft_fill_point(&param->first_pos, mouse->y, mouse->x, 0);
		param->left_button_press = TRUE;
	}
	else if (button == MIDDLE_BUTTON)
	{
		ft_fill_point(&param->pos, mouse->y, mouse->x, 0);
		param->right_button_press = TRUE;
	}
	else
		return (FALSE);
	param->need_refresh = TRUE;
	return (TRUE);
}
