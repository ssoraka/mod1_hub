/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 18:28:19 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/27 18:28:20 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_mod1.h"

#define DEFAULT_INDEX 0
#define BACKGROUND_COLOR 0xC0FFFFFF
#define MIN_INTEGER 0x8FFFFFFF

void	ft_clear_image(t_pict *pic)
{
//	ft_memset8((void *)pic->index, DEFAULT_INDEX, pic->count_byte);
	ft_memset8((void *)pic->addr, BACKGROUND_COLOR, pic->count_byte);
	ft_memset8((void *)pic->z_buffer, MIN_INTEGER, pic->count_byte);
}

void	ft_return_image(t_pict *pic)
{
	ft_memcpy8((void *)pic->addr, (void *)pic->addr_copy, pic->count_byte);
	ft_memcpy8((void *)pic->z_buffer, (void *)pic->z_buffer_copy,
			pic->count_byte);
}

void	ft_save_image(t_pict *pic)
{
	ft_memcpy8((void *)pic->addr_copy, (void *)pic->addr, pic->count_byte);
	ft_memcpy8((void *)pic->z_buffer_copy, (void *)pic->z_buffer,
			pic->count_byte);
}
