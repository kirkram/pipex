/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 11:24:04 by klukiano          #+#    #+#             */
/*   Updated: 2024/02/12 17:08:44 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#include <errno.h>

int		pipex(int *fd, char **av, char **envp);
int		child_one(int *fd, char *arg_cmd_1, char **envp, int *end);
char	**find_path(char **envp);
char	*jointhree(char const *s1, char const *s2, char const *s3);
int		child_two(int *fd, char *arg_cmd_1, char **paths, int *end);
void	free_arr_str(char **arr);
int		open_fds(int **fd, char **av);

int	main(int ac, char **av, char **envp)
{
	int	*fd;

	if (!envp[0])
		return (1);
	if (ac == 5)
	{
		fd = malloc(2 * sizeof(int));
		if (!fd)
			return (1);
		if (open_fds(&fd, av) != 0)
			return (1);
		if (pipex(fd, av, envp) != 0)
		{
			free(fd);
			return (2);
		}
		free (fd);
		return (0);
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
	if (paths)
	{
		while (paths[i])
		{
			free (paths[i]);
			i ++;
		}
		free (paths);
		paths = NULL;
	}
	if (end)
	{
		free (*end);
		(*end) = NULL;
	}
	return (1);
}

int	pipex(int *fd, char **av, char **envp)
{
	pid_t	pid[2];
	int		*end;
	char	**paths;
	int		status;

	pid[0] = -42;
	pid[1] = 42;
	end = malloc(2 * sizeof(int));
	if (!end)
		return (1);
	if (pipe(end) == -1)
		return (free_and_1(NULL, &end));

	paths = find_path(envp);
	if (!paths)
		return (free_and_1(NULL, &end));

	if (fd[0] >= 0)
	{
		pid[0] = fork();
		if (pid < 0)
			return (free_and_1(NULL, &end));
		if (pid[0] == 0)
			if (child_one(fd, av[2], paths, end) != 0)
				return (free_and_1(NULL, &end));
	}

	if (pid[0] != 0)
		pid[1] = fork();
	if (pid[1] < 0)
		return (free_and_1(NULL, &end));
	if (pid[1] == 0)
		if (child_two(fd, av[3], paths, end) != 0)
			return (free_and_1(NULL, &end));

	close (fd[0]);
	if (close (fd[1]) < 0 || close(end[0] < 0 || close(end[1]) < 0))
		return (free_and_1(NULL, &end));
	close (end[0]);
	free_and_1(NULL, &end);
	if (fd[0] >= 0)
		waitpid(pid[0], NULL, 0);
	waitpid(pid[1], &status, 0);
	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
		return (2);
	return (0);
}

int	child_two(int *fd, char *arg_cmd_2, char **paths, int *end)
{
	char	**args;
	char	*cmd;
	int		i;

	dup2(end[0], STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	if (close(end[0]) < 0 || close(end[1]) < 0 || close (fd[1]) < 0)
	{
		perror("");
		return (1);
	}
	args = ppx_split(arg_cmd_2, ' ');
	i = 0;
	while (paths[i])
	{
		cmd = jointhree(paths[i], "/", args[0]);
		execve(cmd, args, NULL);
		free (cmd);
		i ++;
	}
	free_arr_str(args);
	perror("");
	return (1);
}

int	child_one(int *fd, char *arg_cmd_1, char **paths, int *end)
{
	char	**args;
	char	*cmd;
	int		i;

	dup2(fd[0], STDIN_FILENO);
	dup2(end[1], STDOUT_FILENO);
	if (close(end[0]) < 0 || close(fd[0]) < 0 || \
	close (fd[1]) < 0 || close(end[1]))
	{
		perror("");
		return (1);
	}
	args = ppx_split(arg_cmd_1, ' ');
	i = 0;
	while (paths[i])
	{
		cmd = jointhree(paths[i], "/", args[0]);
		execve(cmd, args, NULL);
		free (cmd);
		i ++;
	}
	free_arr_str(args);
	perror("");
	return (1);
}

char	**find_path(char **envp)
{
	char	**paths;
	char	**temp;
	int		i;
	int		strcount;
	int		pwd_index;

	paths = NULL;
	temp = NULL;
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			temp = ft_split(envp[i] + 5, ':');
			if (!temp)
				return (NULL);
			break ;
		}
		i ++;
	}
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PWD=", 4) == 0)
		{
			pwd_index = i;
			i = 0;
			while (temp[i])
				i ++;
			strcount = i + 1;
			paths = malloc((strcount + 1) * sizeof(char *));
			if (!paths)
				return (NULL);
			i = 0;
			while (i < strcount - 1)
			{
				ft_printf("the s is %s\n", temp[i]);
				paths[i] = ft_strdup(temp[i]);
				i ++;
			}
			paths[i] = ft_strdup(envp[pwd_index]);
			paths[i + 1] = NULL;
			free_and_1(temp, NULL);
			return (paths);
		}
		i ++;
	}
	return (temp);
}

char	*jointhree(char const *s1, char const *s2, char const *s3)
{
	char	*newstr;

	if (s1 && s2 && s3)
	{
		newstr = malloc(((ft_strlen(s1) + ft_strlen(s2) \
		+ ft_strlen(s3)) + 1) * sizeof(char));
		if (newstr == NULL)
			return (NULL);
		ft_strlcpy(newstr, (char *)s1, -1);
		ft_strlcpy(newstr + ft_strlen(newstr), (char *)s2, -1);
		ft_strlcpy(newstr + ft_strlen(newstr), (char *)s3, -1);
		return (newstr);
	}
	return (NULL);
}

void	free_arr_str(char **arr)
{
	int	i;

	i = 0;
	if (arr && *arr)
	{
		while (arr[i])
		{
			free(arr[i]);
			i ++;
		}
		free (arr);
	}
	arr = NULL;
}
