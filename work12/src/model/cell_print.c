/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cells.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 18:12:08 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/27 18:12:08 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_mod1.h"

#define EDGE 4
#define EDGE_COUNT 6

void	ft_print_one_edge(t_vektr **p, t_pict *pic, t_prop prop, t_plgn *plgn)
{
	if (ft_vekt_cos(plgn->rot_n, (t_dpoint){0.0, 0.0, 1.0}) <= 0.0)
		return ;
	plgn->p[0] = p[0];
	plgn->p[1] = p[1];
	plgn->p[2] = p[2];
	plgn->p[3] = p[3];
	if (!ft_need_print_traing(plgn->p))
		return ;
	ft_print_plgn(plgn, pic, prop);
	plgn->p[0] = p[3];
	plgn->p[1] = p[2];
	plgn->p[2] = p[0];
	ft_print_plgn(plgn, pic, prop);
}

void	ft_fill_points_cell3(t_vektr *p8, t_vektr **p)
{
	p[0] = &p8[0];
	p[1] = &p8[3];
	p[2] = &p8[2];
	p[3] = &p8[1];
	p[4] = &p8[4];
	p[5] = &p8[7];
	p[6] = &p8[3];
	p[7] = &p8[0];
	p[8] = &p8[0];
	p[9] = &p8[1];
	p[10] = &p8[5];
	p[11] = &p8[4];
	p[12] = &p8[4];
	p[13] = &p8[5];
	p[14] = &p8[6];
	p[15] = &p8[7];
	p[16] = &p8[1];
	p[17] = &p8[2];
	p[18] = &p8[6];
	p[19] = &p8[5];
	p[20] = &p8[3];
	p[21] = &p8[7];
	p[22] = &p8[6];
	p[23] = &p8[2];
}

void	ft_fill_points_cell2(t_point cell, t_vektr *p, t_param *param)
{
	ft_fill_dpoint(&p[0].abs, cell.y, cell.x, cell.z);
	ft_fill_dpoint(&p[1].abs, cell.y, cell.x + 1, cell.z);
	ft_fill_dpoint(&p[2].abs, cell.y + 1, cell.x + 1, cell.z);
	ft_fill_dpoint(&p[3].abs, cell.y + 1, cell.x, cell.z);
	ft_fill_dpoint(&p[4].abs, cell.y, cell.x, cell.z + 1);
	ft_fill_dpoint(&p[5].abs, cell.y, cell.x + 1, cell.z + 1);
	ft_fill_dpoint(&p[6].abs, cell.y + 1, cell.x + 1, cell.z + 1);
	ft_fill_dpoint(&p[7].abs, cell.y + 1, cell.x, cell.z + 1);
	p[0].color = ft_grad_color(p[0].abs.y, JMAX - J0, COLOR_UP, COLOR_DOWN);
	p[1].color = p[0].color;
	p[2].color = ft_grad_color(p[2].abs.y, JMAX - J0, COLOR_UP, COLOR_DOWN);
	p[3].color = p[2].color;
	p[4].color = p[0].color;
	p[5].color = p[0].color;
	p[6].color = p[2].color;
	p[7].color = p[2].color;
	ft_rotate_point_around_point(param, &p[0]);
	ft_rotate_point_around_point(param, &p[1]);
	ft_rotate_point_around_point(param, &p[2]);
	ft_rotate_point_around_point(param, &p[3]);
	ft_rotate_point_around_point(param, &p[4]);
	ft_rotate_point_around_point(param, &p[5]);
	ft_rotate_point_around_point(param, &p[6]);
	ft_rotate_point_around_point(param, &p[7]);
}

void	ft_fill_plgns_cell4(t_plgn *plgn, t_param *param)
{
	t_dpoint	p;

	plgn[0].cos = (-param->cos.z > 0.0) * -param->cos.z;
	plgn[1].cos = (-param->cos.x > 0.0) * -param->cos.x;
	plgn[2].cos = (-param->cos.y > 0.0) * -param->cos.y;
	plgn[3].cos = (param->cos.z > 0.0) * param->cos.z;
	plgn[4].cos = (param->cos.x > 0.0) * param->cos.x;
	plgn[5].cos = (param->cos.y > 0.0) * param->cos.y;
	p = param->oxyz.oz;
	plgn[0].rot_n = (t_dpoint){-p.y, -p.x, -p.z};
	plgn[3].rot_n = p;
	p = param->oxyz.ox;
	plgn[1].rot_n = (t_dpoint){-p.y, -p.x, -p.z};
	plgn[4].rot_n = p;
	p = param->oxyz.oy;
	plgn[2].rot_n = (t_dpoint){-p.y, -p.x, -p.z};
	plgn[5].rot_n = p;
}

void	print_one_cell(t_point cell, t_pict *pic, t_param *param, t_prop prop)
{
	t_vektr	*p[EDGE * EDGE_COUNT];
	t_vektr	p8[8];
	t_plgn	plgn[EDGE_COUNT];
	int		i;

	ft_fill_points_cell2(cell, p8, param);
	ft_fill_points_cell3(p8, p);
	ft_fill_plgns_cell4(plgn, param);
	i = 0;
	while (i < EDGE_COUNT)
	{
		ft_print_one_edge(p + i * 4, pic, prop, &plgn[i]);
		i++;
	}
}
