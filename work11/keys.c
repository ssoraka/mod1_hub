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

#include "ft_mod1.h"

#define CAM_SHIFT 20
#define CAM_SCALE 1.1
#define CAM_ROTATE 90

#define MIN_SCALE 0.01
#define MAX_SCALE 1000.0

#define KEY_PLUS 18
#define KEY_MINUS 19
#define KEY_Q 12
#define KEY_A 0
#define KEY_W 13
#define KEY_S 1
#define KEY_E 14
#define KEY_D 2
#define KEY_K 40
#define KEY_L 37

#define KEY_LEFT 123
#define KEY_RIGHT 124
#define KEY_UP 126
#define KEY_DOWN 125

#define KEY_ESC 53
#define KEY_P 35
#define KEY_O 31
#define KEY_R 15
#define KEY_G 5
#define KEY_I 34

#define MAX_BRUSH (IMAX / 2)


int		ft_rotate_and_csale(t_param *vis, int key)
{
	if ((key == KEY_PLUS || key == 69) && vis->len < MAX_SCALE)
		vis->len *= CAM_SCALE;
	else if ((key == KEY_MINUS || key == 78) && vis->len > MIN_SCALE)
		vis->len /= CAM_SCALE;
	else if (key == KEY_Q)
		vis->ang.z += M_PI / CAM_ROTATE;
	else if (key == KEY_A)
		vis->ang.z -= M_PI / CAM_ROTATE;
	else if (key == KEY_W)
		vis->ang.y += M_PI / CAM_ROTATE;
	else if (key == KEY_S)
		vis->ang.y -= M_PI / CAM_ROTATE;
	else if (key == KEY_E)
		vis->ang.x += M_PI / CAM_ROTATE;
	else if (key == KEY_D)
		vis->ang.x -= M_PI / CAM_ROTATE;
	else
		return (FALSE);
	ft_rotate_xyz(&(vis->oxyz), &(vis->ang));
	vis->cos.y = ft_vekt_cos(vis->oxyz.oy, vis->light);
	vis->cos.x = ft_vekt_cos(vis->oxyz.ox, vis->light);
	vis->cos.z = ft_vekt_cos(vis->oxyz.oz, vis->light);
	printf("%lf_%lf_%lf\n", vis->cos.y, vis->cos.x, vis->cos.z);
	return (TRUE);
}


int		ft_shift(t_param *vis, int key)
{
	if (key == KEY_RIGHT)
		vis->cam_x += CAM_SHIFT;
	else if (key == KEY_LEFT)
		vis->cam_x -= CAM_SHIFT;
	else if (key == KEY_UP)
		vis->cam_y -= CAM_SHIFT;
	else if (key == KEY_DOWN)
		vis->cam_y += CAM_SHIFT;
	else
		return (FALSE);
	return (TRUE);
}

#define MOVE_LEFT 86
#define MOVE_RIGHT 88
#define MOVE_UP 91
#define MOVE_DOWN 87
#define MOVE_FORW 83
#define MOVE_BACKW 84

int		ft_is_water_cell_shift(t_param *param, int key)
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

int		ft_change_brush(t_param *param, int key)
{
	if (key == KEY_K && param->brush < MAX_BRUSH)
		param->brush++;
	else if (key == KEY_L && param->brush > 1)
		param->brush--;
	else
		return (FALSE);
	return (TRUE);
}

int		deal_key(int key, void *param)
{
	t_param *vis;

	vis = (t_param *)param;
	vis->is_water_change = ft_change_brush(vis, key)
	+ ft_is_water_cell_shift(vis, key);
	if (ft_rotate_and_csale(vis, key) || ft_shift(vis, key) || key == KEY_O
	|| key == KEY_G)
		vis->is_obstacles_change = TRUE;
	if (key == KEY_ESC)
		ft_del_all(NULL);
	if (key == KEY_P)
		vis->pause = !vis->pause;
	if (key == KEY_R)
		vis->rain = NEED_STOP_PRINT_FOR_RAIN;
	if (key == KEY_O)
		vis->is_smooth_relief = !vis->is_smooth_relief;
	if (key == KEY_I)
	{
		vis->is_need_print_obstacles = !vis->is_need_print_obstacles;
		if (vis->is_need_print_obstacles) //возможно следует изменить эту херь
			vis->is_obstacles_change = TRUE;
	}
	if (key == KEY_G)
		vis->grad = !vis->grad;
	printf("\n%d\n", key);
	return (0);
}
