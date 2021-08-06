/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   earth.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 23:24:26 by ssoraka           #+#    #+#             */
/*   Updated: 2020/02/29 23:24:26 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_mod1.h"
#define SMALL_FIRST_VALUE 16

void	ft_del_earth(t_earth **earth)
{
	ft_del_arr(&(*earth)->points);
	ft_del_arr(&(*earth)->poligons);
	ft_memdel((void **)earth);
}

t_earth	*ft_create_earth(void)
{
	t_earth	*earth;

	earth = (t_earth *)ft_memalloc(sizeof(t_earth));
	if (!earth)
		return (NULL);
	earth->points = ft_create_arr(sizeof(t_vektr), SMALL_FIRST_VALUE, NULL);
	earth->poligons = ft_create_arr(sizeof(t_plgn), SMALL_FIRST_VALUE, NULL);
	if (!earth->points || !earth->poligons)
		ft_del_earth(&earth);
	return (earth);
}
