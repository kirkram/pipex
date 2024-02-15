/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 11:24:04 by klukiano          #+#    #+#             */
/*   Updated: 2024/02/15 15:48:15 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

int	main(int ac, char **av, char **envp)
{
	int			*fd;
	int			exit_stat;
	char		**paths;
	int			*end;

	end = NULL;
	if (ac == 5)
	{
		if (look_for_path(envp, av) != 0)
			return (127);
		fd = malloc(2 * sizeof(int));
		if (!fd)
			return (1);
		if (open_fds(&fd, av) != 0)
			return (1);
		paths = find_path(envp);
		if (!paths)
			return (1);
		exit_stat = pipex(fd, av, paths, end);
		free (fd);
		return (exit_stat);
	}
	return (1);
}

int	pipex(int *fd, char **av, char **paths, int *end)
{
	pid_t	pid[2];
	int		status;

	if (pipe_open(&end, &paths))
		return (1);
	if (fd[0] >= 0)
	{
		pid[0] = fork();
		if (pid < 0)
			return (free_and_1(paths, &end));
		else if (pid[0] == 0)
			if (child_first(fd, av[2], paths, end) && free_and_1(paths, &end))
				exit (127);
	}
	pid[1] = fork();
	if (pid[1] < 0)
		return (free_and_1(paths, &end));
	else if (pid[1] == 0)
		return (handle_last_process(fd, av, &paths, &end));
	else
		close_all_fds(fd, end);
	waitpid(pid[0], NULL, 0);
	waitpid(pid[1], &status, 0);
	return (exit_pipex(&paths, &end, status));
}

int	handle_last_process(int *fd, char **av, char ***paths, int **end)
{
	if (child_last(fd, av[3], *paths, *end) != 0)
		free_and_1(*paths, end);
	if (ft_strncmp(av[3], "./", 2) == 0 && access(av[3], X_OK) == -1)
		exit (126);
	exit (127);
}

void	close_all_fds(int *fd, int *end)
{
	close (fd[1]);
	close(end[0]);
	close(end[1]);
	close (fd[0]);
}

int	exit_pipex(char ***paths, int **end, int status)
{
	free_and_1(*paths, end);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else
		return (127);
}

