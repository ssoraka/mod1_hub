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
	map = NULL;
	flags_surface = NULL;
	parts = NULL;

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

	norm_speed = ft_norma_vektora(U_CONST, U_CONST, U_CONST);
	min_h = PART_H;
	deltat = T_DELTA;

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
	if (!(map = (char ***)ft_cube_arr(jmax + 2, imax + 2, kmax + 2, sizeof(char))))
		ft_del_variable();
	if (!(flags_surface = (int ***)ft_cube_arr(jmax + 2, imax + 2, kmax + 2, sizeof(int))))
		ft_del_variable();
	if (!(parts = (t_part ****)ft_cube_arr(jmax + 2, imax + 2, kmax + 2, sizeof(t_part *))))
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
	ft_str_arr_free((char ***)(&ground));
	ft_del_cube_arr((void ****)(&map));
	ft_del_cube_arr((void ****)(&flags_surface));
	ft_del_cube_arr((void ****)(&parts));
	exit(0);
}
