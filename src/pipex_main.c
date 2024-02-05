/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 11:24:04 by klukiano          #+#    #+#             */
/*   Updated: 2024/02/05 17:07:03 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int	main(int ac, char **av, char **envp)
{
	int *fd;

	fd = malloc(2 * sizeof(int));
	// while (*envp)
	// {
	// 	printf("The envp is: %s\n", *envp);
	// 	envp += 1;
	// }
	if (ac == 5)
	{
		fd[0] = open(av[1], O_RDONLY);
		fd[1] = open(av[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (fd[0] < 0 || fd[1] < 0)
			return (1);
		pipex(fd, av, envp);
	}
	return (0);
}

void	pipex(int *fd, char **av, char **envp)
{
	pid_t	pid1;
	int		*end;

	end = malloc(2 * sizeof(int));
	if (pipe(end) == -1)
		return (1);
	pid1 = fork();
	if (pid1 < 0)
		return (1);
	if (pid1 == 0)
	{
		child_one(fd, av[2], envp, end);
	}

	//the path is usr/bin and /bin for some others
	//check the path with "which [command]"

	close (fd[0]);
	close (fd[1]);
	waitpid(pid1, NULL, 0);
}

void	child_one(int *fd, char *arg_cmd_1, char **envp, int *end)
{
	char	**args;
	char	**paths;
	char	*cmd;
	int		i;
	//modern pipes are 64k 4k is old standard

	dup2(end[0], STDIN_FILENO);
	dup2(end[1], STDOUT_FILENO); //now the stdout will be routed into the fd[1];
	// if = 0 -> too much info in file; gnl seems like overkill for now
	if (read (fd[0], end[0], 4096) < 0 || \
	close(fd[0]) < 0 || close (fd[1]) < 0)
		return (1);

	args = ft_split(arg_cmd_1, " ");
	paths = find_path(envp);
	if (!paths)
		return (1);
	//execve from buf into fd[1];
	i = 0;
	while (paths[i])
	{
		cmd = ft_strjoin(paths[i], args[0]);
		execve(cmd, args, NULL);
		free (cmd);
		paths ++;
		i ++;
	}
	perror("Couldn't do execve\n");
	return (1);
	// it shall overtake the process completely
	// does it need to be closed?
}

char	**find_path(char **envp)
{
	char **paths;

	paths = NULL;
	while (*envp)
	{
		if (ft_strncmp(*envp, "PATH=", 5) == 0)
		{
			paths = ft_split(*envp + 6, ":");
			break ;
		}
		envp ++;
	}
	return (paths);
}

// fd[0] = open(av[1], O_RDONLY);
		// if (fd[0] < 0)
		// {
		// 	err_management;
		// }
		// fd[1] = open(av[4], O_WRONLY);
		// if (fd[1] < 0)
		// {
		// 	err_management;
		// }
