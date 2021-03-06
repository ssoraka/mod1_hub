/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solver_3d.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/05 16:29:13 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/05 16:29:13 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"



REAL	ft_time_control(REAL max_c, REAL norm_speed, REAL min_h)
{
	REAL deltat;

	//printf("%lf_%lf_%lf\n", max_c, norm_speed, min_h);
	deltat = CONST_CUR * min_h / (max_c + norm_speed);
	if (deltat != deltat || !deltat || deltat > T_DELTA || deltat < 0)
		deltat = T_DELTA;
	norm_speed = norm_speed * 0.1;
	//max_c = SPEED_OF_SOUND_C;
	//printf("%lf\n", deltat);
	return (deltat);
}
