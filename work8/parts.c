/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parts.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 14:28:01 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/08 14:28:02 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"

# define MAX_NEIGHBORS 64
# define DELTA_H 0.800

//del
void	ft_create_new_water_in_cell(void *param, int j, int i, int k)
{
	return ;
	t_dpoint p;
	int ijk[3];
	int parts_count;
	int n;

	parts_count = PARTS_COUNT;//*((int *)param);
	n = parts_count * parts_count * parts_count;
	while (n > 0)
	{
		n--;
		ijk[0] = n / (parts_count * parts_count);
		ijk[2] = n % parts_count;
		ijk[1] = (n % (parts_count * parts_count)) / parts_count;

		p.x = delta.x / parts_count * ijk[0] + delta.x / parts_count / 2 + delta.x * (i - 1);
		p.y = delta.y / parts_count * ijk[1] + delta.y / parts_count / 2 + delta.y * (j - 1);
		p.z = delta.z / parts_count * ijk[2] + delta.z / parts_count / 2 + delta.z * (k - 1);
		if (!ft_add_part((void *)&(parts[j][i][k]), &p, *((int *)param)))
			ft_del_all_print_error(MSG_ERROR1);
		ft_fill_point(&((parts[j][i][k])->jik), j, i, k);
		//printf("%d_%d_%d_%d\n", n, ijk[1], ijk[0], ijk[2]);
	}
}


void ft_del_parts(void *ptr)
{
	t_part *part;

	part = (t_part *)ptr;
	if (part)
	{
		//надо бы переделать под двусвязный список, чтоб была возможность частицу нормально выпилить
		ft_del_arr(&(part->neigh));
	}
}


t_arr	*ft_init_all_clear_parts(void)
{
	t_arr *parts;
	int parts_count;

	parts_count = (int)(CELL_COUNT * 2.0);
	parts = ft_create_arr(sizeof(t_part), parts_count, &ft_del_parts);
	if (!parts)
		ft_del_all_print_error("need more memory");
	return (parts);
}

void	ft_fill_part(t_part *part, t_dpoint *p, int type)
{
	if (!part || !p)
		return ;
	part->type = type;
	//ft_fill_point(&(part->pos.otn), (int)p->y, (int)p->x, (int)p->z);
	ft_fill_point(&(part->jik), (int)p->y + 0, (int)p->x + 0, (int)p->z + 0);
	ft_fill_dpoint(&(part->pos.abs), p->y, p->x, p->z);
	ft_fill_param_of_part(part, NULL);
	if (type == WATER)
		part->pos.color = WATER_COLOR;
	else
		part->pos.color = OBSTACLES_TOP_COLOR;

	part->neigh = ft_create_arr(sizeof(t_neigh), MAX_NEIGHBORS, NULL);

	if (!part->neigh)
		ft_del_all_print_error("need more memory2");
}

void	ft_insert_part(t_part *part)
{
	t_part *tmp;

	if (!part)
		return ;
	//printf("___%d_%d_%d___\n", part->jik.y, part->jik.x, part->jik.z);
	tmp = parts[part->jik.y][part->jik.x][part->jik.z];
	part->next = tmp;
	if (tmp)
		tmp->prev = part;
	parts[part->jik.y][part->jik.x][part->jik.z] = part;
	part->prev = NULL;
}

void	ft_cut_part(t_part *part)
{
	t_part **tmp;

	if (!part)
		return ;
	tmp = &(parts[part->jik.y][part->jik.x][part->jik.z]);

	if (!part->prev)
		*tmp = part->next;
	else
		part->prev->next = part->next;

	if (part->next)
		part->next->prev = part->prev;

	part->prev = NULL;
	part->next = NULL;
}

void	ft_create_new_water_in_area(t_arr **p_arr, t_dpoint *start)
{
	REAL i;
	REAL k;
	int cell_type;
	t_part part;
	t_dpoint pos;

	ft_bzero((void *)&part, sizeof(t_part));
	i = start->x;
	while (i <= IMAX)
	{
		k = start->z;
		while (k <= KMAX)
		{
			cell_type = map[(int)(start->y)][(int)i][(int)k];
			if (cell_type != EMPTY)
			{
				ft_fill_dpoint(&pos, start->y, i, k);
				ft_fill_part(&part, &pos, cell_type);
				ft_insert_part((t_part *)ft_arr_add(p_arr, (void *)&part));
				//parts[(int)(start->y)][(int)i][(int)k] = ft_arr_add(p_arr, (void *)&part);
			}
			k++;
		}
		i++;
	}
}

void	ft_create_new_area_of_water(t_arr **parts)
{
	t_dpoint start;
	int n;

	n = 0;
	start.y = J0 + 0.5;
	while (start.y <= JMAX)
	{
		if (n % 2)
			ft_fill_dpoint(&start, start.y, I0 + 0.2, K0 + 0.2);
		else
			ft_fill_dpoint(&start, start.y, I0 + 0.7, K0 + 0.7);
		ft_create_new_water_in_area(parts, &start);
		start.y += DELTA_H;
		n++;
	}
}

t_part	*ft_new_part(t_dpoint *p, int type)
{
	t_part *tmp;

	tmp = (t_part *)ft_memalloc(sizeof(t_part));
	ft_fill_part(tmp, p, type);
	return (tmp);
}

t_part	*ft_add_part(void *ptr, t_dpoint *p, int type)
{
	t_part *tmp;
	t_part **begin;

	begin = (t_part **)ptr;
	tmp = ft_new_part(p, type);
	if (!tmp)
		return (NULL);
	tmp->next = *begin;
	*begin = tmp;
	return (tmp);
}


void	ft_del_part(t_part **begin)
{
	t_part *tmp;

	tmp = *begin;
	while (tmp)
	{
		*begin = tmp->next;
		free(tmp);
		tmp = *begin;
	}
	*begin = NULL;
}

void	ft_replace_part(t_part *prev, t_part **part, t_part ****parts)
{
	int i;
	int j;
	int k;

	prev->next = (*part)->next;
	i = (*part)->jik.x;
	j = (*part)->jik.y;
	k = (*part)->jik.z;
	(*part)->next = parts[j][i][k];
	parts[j][i][k] = (*part);
	*part = prev;
}

void	ft_replace_part2(t_part *part)
{
	int i;
	int j;
	int k;

	i = part->jik.x;
	j = part->jik.y;
	k = part->jik.z;
	if (i < I0 || j < J0 || k < K0 || i > IMAX || j > JMAX || k > KMAX)
	{
		part->type = EMPTY;
		part->pos.color = 0xFFFFFF;
		ft_fill_dpoint(&(part->pos.abs), JMAX + 1, IMAX + 1, KMAX + 1);
		ft_fill_point(&(part->jik), JMAX + 1, IMAX + 1, KMAX + 1);
	}
	ft_insert_part(part);
}

int		ft_part_change_cell(t_part *part)
{
	int i;
	int j;
	int k;

	i = (int)(part->pos.abs.x / delta.x) + 0;
	j = (int)(part->pos.abs.y / delta.y) + 0;
	k = (int)(part->pos.abs.z / delta.z) + 0;
	if (i != part->jik.x || j != part->jik.y || k != part->jik.z)
	{
		ft_cut_part(part);
		ft_fill_point(&(part->jik), j, i, k);
		return (TRUE);
	}
	return (FALSE);
}
