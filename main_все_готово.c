/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/04 16:23:42 by ssoraka           #+#    #+#             */
/*   Updated: 2019/03/07 16:22:42 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "mlx.h"
#include "libft.h"

#define ABS(nbr) ((nbr) >= 0 ? (nbr) : (-1) * (nbr))


typedef double REAL;

#define DELTA 10
#define CONST_WIDTH 1000
#define CONST_HEINTH 1000
#define CONST_LEN 9

typedef struct		s_inf
{
	REAL dx;
	REAL dy;
	REAL dx2;
	REAL dy2;
	int jmax;
	int imax;
	char **map;
}					t_inf;

typedef struct		s_vis
{
	int				cam_x;
	int				cam_y;
	int				len;
	REAL			ang_x;
	REAL			ang_y;
	REAL			ang_z;
	int				*addr;
	void			*img;
	void			*mlx;
	void			*win;
	struct s_vektr	*water;
	struct s_vektr	*oxyz;
	struct s_vektr	*points;
	struct s_line	*lines;
	struct s_inf	*inf;
}					t_vis;

typedef struct		s_iter
{
	int				i;
	int				j;
	int				imax;
	int				jmax;
}					t_iter;


int expose_hook(void *param);

/*
** к каждому пикселю присваивать координату z в дополнительном массиве
** проверять позицию, куда ты ее рисуешь
** нужно сделать структуру полигонов, которая будет отрисосывать 2 линии и
** заполнять значения треугольника координатами z, чтоб другие точки там не
** отрисовывались!!!
** работать в векторе, тогда все просто и понятно
** научиться рисовать треугольники
*/


// void *mlx_ptr;
// void *win_ptr;
// void *img_ptr;

// int width;
// int hight;
// int img_width;
// int img_hight;
// int cam_x;
// int cam_y;

// int persp = 0;
// int perspect = 500;
//int perspect_x = 0;//505;
//int perspect_y = 0;//505;

//int max_z = 0;
//int min_z = 0;

//int max_x = 0;
//int max_y = 0;

//int point_x;
//int point_y;

//char *addr;
//int len = 70;

//double ang_x = 0;
//double ang_y = 0;
//double ang_z = 0;



typedef struct		s_vektr
{
	int abs_x;
	int abs_y;
	int abs_z;
	int color;
	double x;
	double y;
	double z;
	int otn_x;
	int otn_y;
	int otn_z;
	struct s_vektr *next;
	struct s_vektr *down;
}					t_vektr;

typedef struct		s_line
{
	int y1;
	int x1;
	int y2;
	int x2;
	int diry;
	int dirx;
	int deltay;
	int deltax;
	int error;
	int color;
	struct s_vektr *p1;
	struct s_vektr *p2;
	struct s_line *next;

}					t_line;


//t_line *g_lines;
//t_vektr *points;

int ft_znak(int num)
{
	int znak;

	znak = 0;
	if (num > 0)
		znak = 1;
	else if (num < 0)
		znak = -1;
	return (znak);
}



void ft_put_pixel_to_img(int *addr, int x, int y, int color)
{
	if (x < 0 || y < 0 || x >= CONST_WIDTH || y >= CONST_HEINTH)
		return ;
	addr[y * CONST_WIDTH + x] = color;
}

int ft_interpolation(int percent, int color1, int color2, int byte)
{
	int deltared;

	while (byte > 1)
	{
		color1 = color1 >> 8;
		color2 = color2 >> 8;
		byte--;
	}
	color1 = color1 & 0xFF;
	color2 = color2 & 0xFF;
	deltared = ((color2 - color1) * percent) / 100 + color1;
	return (deltared);
}

int ft_grad_color(int delta1, int delta2, int color1, int color2)
{
	int color;
	int grad;

	grad = 100 - (delta1 * 100) / delta2;
	color = ft_interpolation(grad, color1, color2, 1);
	color = color + ((ft_interpolation(grad, color1, color2, 2)) << 8);
	color = color + ((ft_interpolation(grad, color1, color2, 3)) << 16);
	return (color);
}


void ft_set_color_to_line(t_line *line, int lower_45)
{
	int delta1;
	int delta2;

	if (lower_45)
	{
		delta1 = ABS(line->x2 - line->x1);
		delta2 = line->deltax;
	}
	else
	{
		delta1 = ABS(line->y2 - line->y1);
		delta2 = line->deltay;
	}
	line->color = ft_grad_color(delta1, delta2, line->p1->color, line->p2->color);
}



void draw_line_img_lower_45(t_line *line, int *addr)
{
	int error;

	error = 0;
	while (line->x1 != line->x2)
	{
		ft_set_color_to_line(line, 1);
		ft_put_pixel_to_img(addr, line->x1, line->y1, line->color);
		error = error + line->deltay;
		if (2 * error >= line->deltax)
		{
			line->y1 += line->diry;
			error = error - line->deltax;
		}
		line->x1 += line->dirx;
	}
	ft_put_pixel_to_img(addr, line->x2, line->y2, line->color);
}


void draw_line_img_over_45(t_line *line, int *addr)
{
	int error;

	error = 0;
	while (line->y1 != line->y2)
	{
		ft_set_color_to_line(line, 0);
		ft_put_pixel_to_img(addr, line->x1, line->y1, line->color);
		error = error + line->deltax;
		if (2 * error >= line->deltay)
		{
			line->x1 += line->dirx;
			error = error - line->deltay;
		}
		line->y1 +=line->diry;
	}
	ft_put_pixel_to_img(addr, line->x2, line->y2, line->color);
}


int draw_line_img(t_line *line, int *addr)
{
	line->x1 = line->p1->abs_x;// - max_x * len / 2;
	line->y1 = line->p1->abs_y;// + max_y * len / 2;
	line->x2 = line->p2->abs_x;// - max_x * len / 2;
	line->y2 = line->p2->abs_y;// + max_y * len / 2;
	line->diry = ft_znak(line->y2 - line->y1);
	line->dirx = ft_znak(line->x2 - line->x1);
	line->deltay = line->diry * (line->y2 - line->y1);
	line->deltax = line->dirx * (line->x2 - line->x1);

	if (line->deltax >= line->deltay)
	{
		draw_line_img_lower_45(line, addr);
	}
	else
	{
		draw_line_img_over_45(line, addr);
	}
	return (0);
}




t_vektr *ft_new_vektor(int x, int y, int z);




/*
**	избавление от погрешности
*/

void ft_norm_vektor(t_vektr *vek)
{
	double summ;

	summ = vek->x * vek->x + vek->y * vek->y + vek->z * vek->z;
	summ = sqrt(summ);
	vek->x = vek->x / summ;
	vek->y = vek->y / summ;
	vek->z = vek->z / summ;
}






t_vektr *ft_rot(t_vektr *ox, t_vektr *oy, double ang)
{
	double cosa;
	double sina;
	double temp_x;
	double temp_y;
	double temp_z;

	if (ang == 0)
		return (ox);
	cosa = cos(ang);
	sina = sin(ang);
	temp_x = ox->x * (cosa + (1 - cosa) * oy->x * oy->x);
	temp_x = temp_x + ox->y * ((1 - cosa) * oy->x * oy->y - sina * oy->z);
	temp_x = temp_x + ox->z * ((1 - cosa) * oy->x * oy->z + sina * oy->y);
	temp_y = ox->x * ((1 - cosa) * oy->y * oy->x + sina * oy->z);
	temp_y = temp_y + ox->y * (cosa + (1 - cosa) * oy->y * oy->y);
	temp_y = temp_y + ox->z * ((1 - cosa) * oy->y * oy->z - sina * oy->x);
	temp_z = ox->x * ((1 - cosa) * oy->z * oy->x - sina * oy->y);
	temp_z = temp_z + ox->y * ((1 - cosa) * oy->z * oy->y + sina * oy->x);
	temp_z = temp_z + ox->z * (cosa + (1 - cosa) * oy->z * oy->z);
	ox->x = temp_x;
	ox->y = temp_y;
	ox->z = temp_z;
	ft_norm_vektor(ox);
	return (ox);
}


t_vektr *ft_new_vektor(int x, int y, int z)
{
	t_vektr *tmp;

	tmp = (t_vektr *)malloc(sizeof(t_vektr));
	tmp->x = x;
	tmp->y = y;
	tmp->z = z;
	tmp->otn_x = x;
	tmp->otn_y = y;
	tmp->otn_z = z;
	tmp->color = 0xFFFFFF;
	tmp->next = NULL;
	tmp->down = NULL;
	return (tmp);
}

t_line *ft_new_line(t_vektr *p1, t_vektr *p2, int color)
{
	t_line *line;

	if (p1 == NULL || p2 == NULL)
		return (NULL);
	line = (t_line *)malloc(sizeof(t_line));
	if (line == NULL)
		return (NULL);
	line->p1 = p1;
	line->p2 = p2;
	if (color == 0)
		line->color = p1->color;
	else
		line->color = color;
	return (line);
}

void ft_ret_abs_xyz(t_vektr *ox, t_vis *vis)
{
	ox->otn_x = (int)(ox->x * vis->len);
	ox->abs_x = ox->otn_x + vis->cam_x;
	ox->otn_y = (int)(ox->y * vis->len);
	ox->abs_y = ox->otn_y + vis->cam_y;
	ox->otn_z = (int)(ox->z * vis->len);
	ox->abs_z = ox->otn_z;
}



void ft_print_oxyz2(t_vis *vis)
{
	t_vektr *ox0 = vis->oxyz;
	t_vektr *oy0 = ox0->next;
	t_vektr *oz0 = oy0->next;

	if (vis->ang_z != 0)
	{
		oy0 = ft_rot(oy0, oz0, vis->ang_z);
		ox0 = ft_rot(ox0, oz0, vis->ang_z);
		vis->ang_z = 0;
	}
	if (vis->ang_x != 0)
	{
		oy0 = ft_rot(oy0, ox0, vis->ang_x);
		oz0 = ft_rot(oz0, ox0, vis->ang_x);
		vis->ang_x = 0;
	}
	if (vis->ang_y != 0)
	{
		ox0 = ft_rot(ox0, oy0, vis->ang_y);
		oz0 = ft_rot(oz0, oy0, vis->ang_y);
		vis->ang_y = 0;
	}
	//ft_ret_abs_xyz(ox0);
	//ft_ret_abs_xyz(oy0);
	//ft_ret_abs_xyz(oz0);
}

/*
void ft_perspektiva(t_vektr *p)
{
	int znak;

	p->abs_x -= cam_x;
	p->abs_y -= cam_y;
	p->abs_x -= perspect_x;
	p->abs_y -= perspect_y;
	znak = ft_znak(p->abs_x);
	//p->abs_x = p->abs_x - (p->abs_z * perspect) * (p->abs_x) / max_z / 50;
	p->abs_x = p->abs_x * (perspect - p->abs_z) / perspect;
	//	if (znak != ft_znak(p->abs_x))
	//		p->abs_x = 0;
	znak = ft_znak(p->abs_y);
	//p->abs_y = p->abs_y - (p->abs_z * perspect) * (p->abs_y) / max_z / 50;
	p->abs_y = p->abs_y * (perspect - p->abs_z) / perspect;
	//	if (znak != ft_znak(p->abs_y))
	//		p->abs_y = 0;
	p->abs_x += perspect_x;
	p->abs_y += perspect_y;
	p->abs_x += cam_x;
	p->abs_y += cam_y;
}


void ft_perspektiva2(t_vektr *p)
{
	int znak;
	double sinx;
	double siny;
	long d;


	p->abs_x -= cam_x;
	p->abs_y -= cam_y;
	p->abs_x -= perspect_x;
	p->abs_y -= perspect_y;
	d = sqrt(p->abs_x * p->abs_x + p->abs_y * p->abs_y + perspect * perspect);
	//выпуклая сфера
	if (persp == 2)
	{
		p->abs_x = (perspect * p->abs_x) / d;
		p->abs_y = (perspect * p->abs_y) / d;
	}
	//вогнутаяая сфера
	if (persp == 3)
	{
		p->abs_x = (d * p->abs_x) / perspect;
		p->abs_y = (d * p->abs_y) / perspect;
	}
	p->abs_x += perspect_x;
	p->abs_y += perspect_y;
	p->abs_x += cam_x;
	p->abs_y += cam_y;
}
*/

void ft_change_points(t_vis *vis)
{
	t_vektr *p;
	t_vektr *ox0 = vis->oxyz;
	t_vektr *oy0 = ox0->next;
	t_vektr *oz0 = oy0->next;

	p = vis->points;
	while (p)
	{
		p->abs_z = ox0->z * p->otn_x * vis->len + oy0->z * p->otn_y * vis->len + oz0->z * p->otn_z * vis->len;
		p->abs_x = ox0->x * p->otn_x * vis->len + oy0->x * p->otn_y * vis->len + oz0->x * p->otn_z * vis->len;
		p->abs_y = ox0->y * p->otn_x * vis->len + oy0->y * p->otn_y * vis->len + oz0->y * p->otn_z * vis->len;

		p->abs_x = p->abs_x + vis->cam_x;
		p->abs_y = p->abs_y + vis->cam_y;
		p = p->next;
	}

	/*p = points;
	while (p)
	{
		if (persp == 1)
			ft_perspektiva(p);
		if (persp > 1)
			ft_perspektiva2(p);
		p = p->next;
	}*/

}





void ft_print_lines(t_vis *vis)
{
	t_line *line;

	line = vis->lines;
	//перспектива
	//ft_perspektiva(point);
	//printf("%d_%d_%d_%d\n", g_lines->p1->abs_x, g_lines->p2->abs_x, g_lines->p1->abs_y, g_lines->p2->abs_y);
	//draw_line_img(line);
	while (line)
	{
		//перспектива
		//ft_perspektiva(point->next);
		draw_line_img(line, vis->addr);
		line = line->next;
	}

}


int deal_key(int key, void *param)
{
	t_vis *vis;


	vis = (t_vis *)param;
	/*if (key == 12)
		vis->ang_z += M_PI / 180;
	if (key == 13)
		vis->ang_y += M_PI / 180;
	if (key == 14)
		vis->ang_x += M_PI / 180;*/
	if (key == 53)//ESC
	{
		exit(0);
	}
	/*if (key == 124)
		vis->cam_x+=10;
	if (key == 123)
		vis->cam_x-=10;
	if (key == 126)
		vis->cam_y-=10;
	if (key == 125)
		vis->cam_y+=10;
	if (key == 69)
		vis->len+=1;
	if (key == 78)
		vis->len-=1;*/
	/*if (key == 88)
		perspect_x+=10;
	if (key == 86)
		perspect_x-=10;
	if (key == 91)
		perspect_y-=10;
	if (key == 84)
		perspect_y+=10;

	if (key == 35)
	{
		perspect+=5;
		if (perspect == 0)
			perspect++;
	}
	if (key == 31)
	{
		perspect-=5;
		if (perspect == 0)
			perspect--;
	}
	if (key == 34)
	{
		perspect = 500;
		persp++;
	}
	if (persp > 3)
		persp = 0;*/
	/*ft_put_pixel_to_img(addr, perspect_x, perspect_y, 0xFF0000);
	//printf("%d\n", perspect);
	mlx_clear_window(mlx_ptr, win_ptr);
	ft_print_oxyz2();

	ft_change_points();
	ft_print_lines();
	mlx_put_image_to_window (mlx_ptr, win_ptr, img_ptr, 10, 10);*/
	//expose_hook(param);

	return (0);
}


/*
t_vektr *parser_string(char *str, int row)
{
	int column;
	t_vektr *begin;
	t_vektr *v;

	column = 0;
	begin = ft_new_vektor(column, row, ft_atoi(str));
	while (ft_isdigit(*str) || *str == '-')
		str++;
	v = begin;
	while (*str)
	{
		//надо сделать функции, которые будут возвращать указатель на строку с цифрой и с цветом
		while (ft_isspace(*str))
			str++;
		column++;
		v->next = ft_new_vektor(column, row, ft_atoi(str));
		while (ft_isdigit(*str) || *str == '-')
			str++;
		v = v->next;
	}
	if (max_x < column)
		max_x = column;
	return (begin);
}
*/
/*
t_vektr *pars_file(int fd)
{
	char *line;
	t_vektr *begin;
	t_vektr *v;
	int row;

	row = 0;
	if (get_next_line(fd, &line) < 1)
		return (NULL);
	begin = parser_string(line, row);
	v = begin;
	while (get_next_line(fd, &line))
	{
		row++;
		v->down = parser_string(line, row);
		free(line);
		v = v->down;
	}
	if (max_y < row)
		max_y = row;
	return (begin);
}
*/

void ft_add_line(t_line **begin, t_vektr *p1, t_vektr *p2)
{
	t_line *line;
	t_line *tmp;

	line = ft_new_line(p1, p2, 0xFFFFFF);

	if (!line)
		return ;
	tmp = *begin;
	*begin = line;
	line->next = tmp;
}

/*
int ft_lines_from_points(t_line **begin, t_vektr *point)
{
	t_line *line;
	t_vektr *p1;
	t_vektr *p2;

	while (point)
	{
		p1 = point;
		point = point->down;
		p2 = point;
		while (p1->next)
		{
			ft_add_line(begin, p1, p1->next);
			ft_add_line(begin, p1, p2);
			p1 = p1->next;
			if (p2 && p2->next)
				p2 = p2->next;
		}
		p1->next = point;
		ft_add_line(begin, p1, p2);
	}
	return (0);
}
*/

/*
void ft_shift_points_to_center()
{
	t_vektr *p;
	int delta_z;

	p = points;
	while (p)
	{
		if (p->otn_z > max_z)
			max_z = p->otn_z;
		if (p->otn_z < min_z)
			min_z = p->otn_z;
		p = p->next;
	}

	//printf("%d_%d_%d\n",delta_z, max_z, min_z);
	delta_z = max_z - min_z;
	p = points;
	while (p)
	{
		p->color = ft_grad_color(p->otn_z - min_z, delta_z, 0xFF0000, 0xFF00);
		p->otn_x = p->otn_x * 2;
		p->otn_y = p->otn_y * 2;
		p->otn_z = p->otn_z;
		p->otn_x = p->otn_x - max_x;
		p->otn_y = p->otn_y - max_y;
		// if (p->otn_z == 0)
		// 	p->color = 0xFF;
		// else
		// 	p->color = 0xFF0000;
		p = p->next;
	}

}
*/



void	ft_iteration_i(void *ptr, void (*f)(void *, int, int), t_iter *iter)
{
	int i;

	i = iter->i;
	while (i <= iter->imax)
	{
		f(ptr, iter->j, i);
		i++;
	}
}

void	ft_iteration_j(void *ptr, void (*f)(void *, int, int), t_iter *iter)
{
	int j;

	j = iter->j;
	while (j <= iter->jmax)
	{
		f(ptr, j, iter->i);
		j++;
	}
}


void	ft_iteration(void *ptr, void (*f)(void *, int, int), t_iter *iter)
{
	int i;
	int j;

	j = iter->j;
	while (j <= iter->jmax)
	{
		i = iter->i;
		while (i <= iter->imax)
		{
			f(ptr, j, i);
			i++;
		}
		j++;
	}
}

void	ft_fill_iterations(t_iter *iter, int j, int jmax, int i, int imax)
{
	iter->j = j;
	iter->i = i;
	iter->jmax = jmax;
	iter->imax = imax;
}


int		ft_read_map_from_file(char *name, t_inf **inf)
{
	int		fd;
	int		i;

	i = 0;
	*inf = (t_inf *)ft_memalloc(sizeof(t_inf));
	if ((fd = open(name, O_RDWR)) < 0 || read(fd, NULL, 0) < 0)
	{
		ft_putstr("error");
		return (FAIL);
	}
	(*inf)->map = (char **)ft_memalloc(sizeof(char *) * 200);
	while (get_next_line(fd, &((*inf)->map[i])))
		i++;
	close(fd);
	(*inf)->jmax = i;
	(*inf)->imax = ft_strlen((*inf)->map[0]);
	(*inf)->dx = DELTA;
	(*inf)->dy = DELTA;
	(*inf)->dx2 = DELTA * DELTA;
	(*inf)->dy2 = DELTA * DELTA;
	return (SUCCESS);
}


t_vektr *ft_add_vektor(void *ptr, int x, int y, int z)
{
	t_vektr *tmp;
	t_vektr **begin;

	begin = (t_vektr **)ptr;
	tmp = ft_new_vektor(x, y, z);
	tmp->next = *begin;
	*begin = tmp;
	return (tmp);
}

void	ft_create_xyz(t_vis *vis)
{
	t_vektr *xyz;

	vis->oxyz = ft_new_vektor(0, 0, 1);
	xyz = ft_new_vektor(0, 1, 0);
	xyz->next = vis->oxyz;
	vis->oxyz = xyz;
	xyz = ft_new_vektor(1, 0, 0);
	xyz->next = vis->oxyz;
	vis->oxyz = xyz;
}



void	ft_create_four_lines_of_cell(void *ptr, int i, int j)
{
	t_vis *vis;
	t_vektr *p1;
	t_vektr *p2;
	t_vektr *p3;
	t_vektr *p4;

	vis = (t_vis *)ptr;
	p1 = ft_add_vektor((void *)(&vis->points), i * vis->inf->dx, j * vis->inf->dy, 0);
	p2 = ft_add_vektor((void *)(&vis->points), i * vis->inf->dx, (j + 1) * vis->inf->dy, 0);
	p3 = ft_add_vektor((void *)(&vis->points), (i + 1) * vis->inf->dx, j * vis->inf->dy, 0);
	p4 = ft_add_vektor((void *)(&vis->points), (i + 1) * vis->inf->dx, (j + 1) * vis->inf->dy, 0);
	ft_add_line(&(vis->lines), p1, p2);
	ft_add_line(&(vis->lines), p2, p4);
	ft_add_line(&(vis->lines), p4, p3);
	ft_add_line(&(vis->lines), p3, p1);
}



void	ft_create_water_in_cell(void *vis, int i, int j)
{
	int x0;
	int y0;
	int delta;
	void *points;

	delta = DELTA / 4;
	x0 = i * ((t_vis *)vis)->inf->dx + delta;
	y0 = j * ((t_vis *)vis)->inf->dy + delta;
	points = ((t_vis *)vis)->water;
	ft_add_vektor((void *)(&points), x0, y0, 0);
	ft_add_vektor((void *)(&points), x0 + delta, y0, 0);
	ft_add_vektor((void *)(&points), x0, y0 + delta, 0);
	ft_add_vektor((void *)(&points), x0 + delta, y0 + delta, 0);
}

void	ft_create_points_in_cells(t_vis *vis)
{
	t_iter iter;

	//создаем по 4 точки воды в каждой клетке
	ft_fill_iterations(&iter, 1, vis->inf->jmax, 1, vis->inf->imax);
	ft_iteration((void *)vis, &ft_create_water_in_cell, &iter);
	//рисуем по 4 линии на клетку
	ft_fill_iterations(&iter, 1, vis->inf->jmax - 1, 1, vis->inf->imax - 1);
	ft_iteration((void *)vis, &ft_create_four_lines_of_cell, &iter);
}


int ft_create_img(t_vis *vis)
{
	int bits_per_pixel = 0;
	int size_line = 0;
	int endian = 0;

	vis->mlx = mlx_init();
	vis->win = mlx_new_window(vis->mlx, CONST_WIDTH, CONST_HEINTH, "mlx 42");
	vis->img = mlx_new_image(vis->mlx, CONST_WIDTH, CONST_HEINTH);
	vis->cam_x = 0;//CONST_WIDTH / 2;
	vis->cam_y = 0;//CONST_HEINTH / 2;
	vis->len = CONST_LEN;
	vis->addr = (int *)mlx_get_data_addr(vis->img, &bits_per_pixel, &size_line, &endian);
	return (0);
}





int expose_hook(void *param)
{
	t_vis *vis;

	vis = (t_vis *)param;
	//ft_bzero(vis->addr, CONST_WIDTH * CONST_HEINTH * 4);
	if (vis->cam_x < CONST_WIDTH)
		vis->cam_x++;
	else
		vis->cam_y++;
	ft_put_pixel_to_img(vis->addr, vis->cam_x, vis->cam_y, 0xffffff);

	//mlx_hook(vis->win, 2, 0, deal_key, (void *)vis);


	//ft_print_oxyz2(vis);
	//ft_change_points(vis);
	//ft_print_lines(vis);
	mlx_put_image_to_window(vis->mlx, vis->win, vis->img, 0, 0);
	//sleep(1);
	return (0);
}


int main()
{
	t_vis *vis;

	vis = ft_memalloc(sizeof(t_vis));
/*	if (ft_read_map_from_file("text.txt", &(vis->inf)) == FAIL)
		return (0);

	ft_create_xyz(vis);
	ft_create_points_in_cells(vis);*/

	//ft_shift_points_to_center(vis);
	ft_create_img(vis);




	//

	//
	//while (1)
	//{
		//mlx_clear_window(vis->mlx, vis->win);


	//mlx_hook(vis->win, 12, 0, expose_hook, (void *)vis);
	//
	//mlx_loop(vis->mlx);
	//
	//}
	//mlx_loop_hook (vis->win, deal_key, (void *)vis);
	//mlx_expose_hook(vis->win, expose_hook, (void *)vis);

	mlx_hook(vis->win, 2, 0, deal_key, (void *)vis);
	mlx_loop_hook(vis->win, expose_hook, (void *)vis);
	mlx_loop(vis->mlx);


	return (0);
}
