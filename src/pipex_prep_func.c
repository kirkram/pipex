/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_prep_func.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 15:01:42 by klukiano          #+#    #+#             */
/*   Updated: 2024/02/15 15:56:05 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

int	look_for_path(char **envp, char **av)
{
	int	i;

	(void)av;
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (0);
		i ++;
	}
	return (1);
}

int	open_fds(int **fd, char **av)
{
	(*fd)[0] = open(av[1], O_RDONLY);
	if ((*fd)[0] < 0)
		perror("");
	(*fd)[1] = open(av[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if ((*fd)[1] < 0)
	{
		perror("");
		free ((*fd));
		return (1);
	}
	return (0);
}

int	free_and_1(char **paths, int **end)
{
	int	i;

	i = 0;
	if (paths && *paths)
	{
		while (paths[i])
		{
			free (paths[i]);
			i ++;
		}
		*paths = NULL;
		free (paths);
	}
	if (end)
	{
		free (*end);
		(*end) = NULL;
	}
	return (1);
}

int	pipe_open(int **end, char ***paths)
{
	*end = malloc(2 * sizeof(int));
	if (!(*end))
		return (1);
	if (pipe(*end) == -1)
		return (free_and_1(*paths, end));
	return (0);
}
