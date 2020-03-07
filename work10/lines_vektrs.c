/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lines_vektrs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 17:51:18 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/27 17:51:18 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"

t_vektr	*ft_new_vektor2(REAL x, REAL y, REAL z, int color)
{
	t_vektr *tmp;

	tmp = (t_vektr *)ft_memalloc(sizeof(t_vektr));
	if (!tmp)
		return (NULL);
	ft_fill_dpoint(&(tmp->abs), y, x, z);
	tmp->color = color;
	tmp->next = NULL;
	return (tmp);
}


t_line	*ft_new_line(t_vektr *p1, t_vektr *p2, int color)
{
	t_line *line;

	if (p1 == NULL || p2 == NULL)
		return (NULL);
	line = (t_line *)ft_memalloc(sizeof(t_line));
	if (line == NULL)
		return (NULL);
	line->p1 = p1;
	line->p2 = p2;
	line->color = color;
	return (line);
}


void	draw_line_img_lower_452(t_line *line, t_point *p, t_pict *pic, int grad)
{
	t_point error;
	int color;

	ft_fill_point(&error, 0, 0, 0);
	color = line->color;
	while (p->x != line->p2->zoom.x)
	{
		if (grad)
			color = ft_set_color_to_point(line, p, 1);
		ft_put_pixel_to_img2(pic, p, color);
		//printf("%d\n", p->z);
		error.y = error.y + line->delta.y;
		if (2 * error.y >= line->delta.x)
		{
			p->y += line->dir.y;
			error.y = error.y - line->delta.x;
		}
		p->z = ft_int_interpolation(p->x - line->p1->zoom.x, line->p2->zoom.x - line->p1->zoom.x, line->p1->zoom.z, line->p2->zoom.z);
		/*error.z = error.z + line->delta.z;
		if (2 * error.z >= line->delta.x)
		{
			p->z += line->dir.z;
			error.z = error.z - line->delta.x;
		}*/

		p->x += line->dir.x;
	}
	if (grad)
		color = line->p2->color;
	ft_put_pixel_to_img2(pic, &(line->p2->zoom), color);
}


void	draw_line_img_over_452(t_line *line, t_point *p, t_pict *pic, int grad)
{
	t_point error;
	int color;

	ft_fill_point(&error, 0, 0, 0);
	color = line->color;
	while (p->y != line->p2->zoom.y)
	{
		if (grad)
			color = ft_set_color_to_point(line, p, 0);
		ft_put_pixel_to_img2(pic, p, color);
		error.x = error.x + line->delta.x;
		if (2 * error.x >= line->delta.y)
		{
			p->x += line->dir.x;
			error.x = error.x - line->delta.y;
		}
		p->z = ft_int_interpolation(p->y - line->p1->zoom.y, line->p2->zoom.y - line->p1->zoom.y, line->p1->zoom.z, line->p2->zoom.z);
		/*error.z = error.z + line->delta.z;
		if (2 * error.z >= line->delta.y)
		{
			p->z += line->dir.z;
			error.z = error.z - line->delta.y;
		}*/
		p->y +=line->dir.y;
	}
	if (grad)
		color = line->p2->color;
	ft_put_pixel_to_img2(pic, &(line->p2->zoom), color);
}


void	draw_line_img2(t_line *line, t_pict *pic, int grad)
{
	t_point p;

	if (ft_not_need_print(line, pic))
		return ;
	ft_fill_point(&p, line->p1->zoom.y, line->p1->zoom.x, line->p1->zoom.z);
	line->dir.y = ft_znak(line->p2->zoom.y - p.y);
	line->dir.x = ft_znak(line->p2->zoom.x - p.x);
	line->dir.z = ft_znak(line->p2->zoom.z - p.z);
	if (!line->dir.y && !line->dir.x)
		return ;
	line->delta.y = line->dir.y * (line->p2->zoom.y - p.y);
	line->delta.x = line->dir.x * (line->p2->zoom.x - p.x);
	line->delta.z = line->dir.z * (line->p2->zoom.z - p.z);
	//printf("%d_%d\n", p.z, p.z);
	if (line->delta.x >= line->delta.y)
		draw_line_img_lower_452(line, &p, pic, grad);
	else
		draw_line_img_over_452(line, &p, pic, grad);
}
