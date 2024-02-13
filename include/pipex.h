/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 11:28:18 by klukiano          #+#    #+#             */
/*   Updated: 2024/02/13 13:49:42 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../lib/libft/libft.h"
# include "../lib/ft_printf/ft_printf.h"

char		**ppx_split(char const *str, char c);

typedef struct s_params
{
	int	error_code;

}	t_params;

#endif
