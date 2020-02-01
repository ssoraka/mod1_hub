/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/04 19:05:18 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/04 19:05:19 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"

void	ft_init_variable(void)
{
	g_cell = NULL;
	g_parts = NULL;
}

void	ft_init_delta_xyz(void)
{
	imax = IMAX;
	jmax = JMAX;
	kmax = KMAX;

	delta.x = DELTA;
	delta.y = DELTA;
	delta.z = DELTA;

	g.x = CONST_GX;
	g.y = CONST_GY;
	g.z = CONST_GZ;

	norm_speed = U_CONST * 1.8;
	//min_h = PART_H;
	deltat = T_DELTA / 100.0;

	renolds = CONST_RE;
	iteration = 0;
}

void ***ft_cube_arr(int jmax, int imax, int kmax, int size)
{
	void ***arr;
	int j;

	if (!(arr = (void ***)ft_memalloc(sizeof(void **) * (jmax + 1))))
		return (NULL);
	j = 0;
	while (j < jmax)
	{
		if (!(arr[j] = (void **)ft_mem_arr_new(imax, kmax, size)))
		{
			ft_del_cube_arr(&arr);
			break ;
		}
		j++;
	}
	return (arr);
}

void	ft_init_map_arrs(void)
{
	if (!(g_parts = ft_init_all_clear_parts()))
		ft_del_variable();
}


void	ft_initialization_of_global_variable(void)
{
	ft_init_variable();
	ft_init_delta_xyz();
	ft_init_map_arrs();
}

void	ft_del_cube_arr(void ****arr)
{
	void	***tmp;
	int		i;

	i = 0;
	tmp = *arr;
	while (tmp[i])
	{
		ft_str_arr_free((char ***)(tmp + i));
		i++;
	}
	ft_memdel((void *)arr);
}

void	ft_del_variable(void)
{
	ft_del_arr(&g_parts);
	ft_str_arr_free((char ***)(&ground));
	ft_del_cell(&g_cell);
	//exit(0);
}
