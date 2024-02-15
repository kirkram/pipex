/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 15:04:39 by klukiano          #+#    #+#             */
/*   Updated: 2024/02/15 19:12:09 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

char		**ppx_split(char const *str, char c);
static int	splitlen(char *str, char c);
static int	countstrings(char *str, char c);
static char	**splitter(char *str, char c, char **array);
static char	**free_reverse(int i, char **array);

char	**ppx_split(char const *str, char c)
{
	char	**array;

	if (str)
	{
		array = malloc((countstrings((char *)str, c) + 1) * sizeof(char *));
		if (array == NULL)
			return (NULL);
		if (countstrings((char *)str, c) == 0)
		{
			free (array);
			return (NULL);
		}
		else
			splitter((char *)str, c, array);
		return (array);
	}
	return (NULL);
}

static char	**splitter(char *str, char c, char **array)
{
	int		i;
	int		j;
	char	stop_char;

	i = 0;
	while (*str != '\0')
	{
		j = 0;
		while (*str == c)
			str ++;
		array[i] = malloc(splitlen((char *)str, c) + 1 * sizeof(char));
		if (array[i] == NULL)
			return (free_reverse(i, array));
		if (*str == '\'' || *str == '\"')
		{
			stop_char = *str;
			str ++;
			while (*str != stop_char && *str != '\0')
			{
				array[i][j] = *str;
				j ++;
				str ++;
			}
			if (*str)
				str ++;
			array[i][j] = '\0';
			i ++;
		}
		else
		{
			while (*str != c && *str != '\0')
			{
				array[i][j] = *str;
				j ++;
				str ++;
			}
			array[i][j] = '\0';
			while (*str == c && *str != '\0')
				str ++;
			i ++;
		}
	}
	array[i] = NULL;
	return (array);
}

static int	splitlen(char *str, char c)
{
	int		i;
	char	stop_char;

	i = 0;
	if (str[i] == '\'' || str[i] == '\"')
	{
		stop_char = *str;
		str ++;
		while (str[i] != stop_char && str[i] != '\0')
			i ++;
		return (i);
	}
	while (str[i] != c && str[i] != '\0')
		i ++;
	return (i);
}

static	int	countstrings(char *str, char c)
{
	int		count;
	char	stop_char;

	count = 0;
	while (*str != '\0')
	{
		while (*str == c && *str != '\0' && *str != '\'' && *str != '\"')
			str ++;
		if (*str != '\0')
			count ++;
		if (*str == '\'' || *str == '\"')
		{
			stop_char = *str;
			str ++;
			while (*str != stop_char && *str)
				str ++;
			str ++;
		}
		else
			while (*str != c && *str != '\0')
				str ++;
	}
	return (count);
}

static char	**free_reverse(int i, char **array)
{
	while (i >= 0)
	{
		free (array[i]);
		i --;
	}
	free (array);
	return (NULL);
}
