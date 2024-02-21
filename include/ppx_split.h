/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ppx_split.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 12:30:30 by klukiano          #+#    #+#             */
/*   Updated: 2024/02/16 12:31:58 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PPX_SPLIT_H
# define PPX_SPLIT_H

char		**ppx_split(char const *str, char c);
int			splitlen(char *str, char c);
int			countstrings(char *str, char c);
char		**splitter(char *str, char c, char **array, int i);
char		**free_reverse(int i, char **array);
int			skip_chars(int *x, char *str, char stop_char, char c);
char		**return_and_nullterm(char ***array, int i);
void		skip_esc_and_c(char stop_char, int *x, char *str, char c);

#endif