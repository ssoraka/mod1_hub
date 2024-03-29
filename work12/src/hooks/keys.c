/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 18:12:08 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/27 18:12:08 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_mod1.h"

#define CAM_SHIFT 20
#define CAM_SCALE 1.1
#define CAM_ROTATE 90

#define MIN_SCALE 0.01
#define MAX_SCALE 1000.0

int	ft_rotate_and_csale(t_param *param, int key)
{
	if ((key == KEY_2) && param->len < MAX_SCALE)
		param->len *= CAM_SCALE;
	else if ((key == KEY_1) && param->len > MIN_SCALE)
		param->len /= CAM_SCALE;
	else if (key == KEY_Q)
		ft_rotate_oxyz_around_v(param, &param->oxyz.oz, M_PI / CAM_ROTATE);
	else if (key == KEY_A)
		ft_rotate_oxyz_around_v(param, &param->oxyz.oz, -M_PI / CAM_ROTATE);
	else if (key == KEY_W)
		ft_rotate_oxyz_around_v(param, &param->oxyz.oy, M_PI / CAM_ROTATE);
	else if (key == KEY_S)
		ft_rotate_oxyz_around_v(param, &param->oxyz.oy, -M_PI / CAM_ROTATE);
	else if (key == KEY_E)
		ft_rotate_oxyz_around_v(param, &param->oxyz.ox, M_PI / CAM_ROTATE);
	else if (key == KEY_D)
		ft_rotate_oxyz_around_v(param, &param->oxyz.ox, -M_PI / CAM_ROTATE);
	else
		return (FALSE);
	return (TRUE);
}

int	ft_shift(t_param *param, int key)
{
	if (key == KEY_RIGHT)
		param->target_x += CAM_SHIFT;
	else if (key == KEY_LEFT)
		param->target_x -= CAM_SHIFT;
	else if (key == KEY_UP)
		param->target_y -= CAM_SHIFT;
	else if (key == KEY_DOWN)
		param->target_y += CAM_SHIFT;
	else
		return (FALSE);
	return (TRUE);
}

int	ft_is_water_cell_shift(t_param *param, int key)
{
	t_point	dir;

	dir = param->water;
	if (key == MOVE_LEFT)
		dir.x++;
	else if (key == MOVE_RIGHT)
		dir.x--;
	else if (key == MOVE_UP)
		dir.y++;
	else if (key == MOVE_DOWN)
		dir.y--;
	else if (key == MOVE_FORW)
		dir.z++;
	else if (key == MOVE_BACKW)
		dir.z--;
	else
		return (FALSE);
	if (dir.x <= I0 || dir.x >= IMAX || dir.y <= J0 || dir.y >= JMAX
		|| dir.z <= K0 || dir.z >= KMAX)
		return (FALSE);
	param->water = dir;
	return (TRUE);
}

void	use_key(int key, t_param *param)
{
	if (key == KEY_ESC)
		ft_del_all(NULL);
	else if (key == KEY_SPACE)
		param->pause = !param->pause;
	else if (key == KEY_R)
		param->rain = WATER;
	else if (key == KEY_T)
		param->rain = MAGMA;
	else if (key == KEY_Z)
		param->is_smooth_relief = (param->is_smooth_relief + 1) % V_EARTH_COUNT;
	else if (key == KEY_X)
		param->is_need_print_obstacles = !param->is_need_print_obstacles;
	else if (key == KEY_C)
		param->grad = !param->grad;
	else if (key == KEY_V)
		param->print_sprite = (param->print_sprite + 1) % V_WATER_COUNT;
	else if (key == KEY_G)
		change_gravity(param);
}

int	deal_key(int key, void *parameters)
{
	t_param	*param;

	param = (t_param *)parameters;
	param->is_water_change = ft_change_brush(param, key)
		+ ft_is_water_cell_shift(param, key);
	if (ft_rotate_and_csale(param, key) || ft_shift(param, key) || key == KEY_Z
		|| key == KEY_C || key == KEY_X || key == KEY_G || key == KEY_V)
		param->need_refresh = TRUE;
	use_key(key, param);
	return (TRUE);
}
