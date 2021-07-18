/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_buttons.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/10 20:59:39 by ssoraka           #+#    #+#             */
/*   Updated: 2019/04/19 13:13:30 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_BUTTONS_H
# define FT_BUTTONS_H

# if defined(__linux__)

#  define KEY_PLUS	18
#  define KEY_MINUS	19
#  define KEY_Q		1738
#  define KEY_A 1734
#  define KEY_W 1731
#  define KEY_S 1753
#  define KEY_E 1749
#  define KEY_D 1751
#  define KEY_K 1740
#  define KEY_L 1732

#  define KEY_LEFT 65361
#  define KEY_RIGHT 65363
#  define KEY_UP 65362
#  define KEY_DOWN 65364

#  define KEY_ESC 65307
#  define KEY_P 1754
#  define KEY_O 1757
#  define KEY_R 1739
#  define KEY_G 1744
#  define KEY_I 1755
#  define KEY_C 1747
#  define KEY_Z 1740
#  define KEY_X 1740

#  define LEFT_BUTTON 1
#  define RIGHT_BUTTON 3
#  define MIDDLE_BUTTON 2
#  define MIDDLE_FORW_BUTTON 4
#  define MIDDLE_BACK_BUTTON 5

#  define KEY_1 49
#  define KEY_2 50
#  define KEY_3 51

# elif defined(__APPLE__) && defined(__MACH__)

#  define KEY_PLUS 69
#  define KEY_MINUS 78
#  define KEY_Q 12
#  define KEY_A 0
#  define KEY_W 13
#  define KEY_S 1
#  define KEY_E 14
#  define KEY_D 2
#  define KEY_K 40
#  define KEY_L 37

#  define KEY_LEFT 123
#  define KEY_RIGHT 124
#  define KEY_UP 126
#  define KEY_DOWN 125

#  define KEY_ESC 53
#  define KEY_P 35
#  define KEY_O 31
#  define KEY_R 15
#  define KEY_G 5
#  define KEY_I 34
#  define KEY_C 8
#  define KEY_Z 6
#  define KEY_X 7

#  define LEFT_BUTTON 1
#  define RIGHT_BUTTON 2
#  define MIDDLE_BUTTON 3
#  define MIDDLE_FORW_BUTTON 4
#  define MIDDLE_BACK_BUTTON 5

#  define KEY_1 18
#  define KEY_2 19
#  define KEY_3 20

# endif
#endif
