/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 11:24:04 by klukiano          #+#    #+#             */
/*   Updated: 2024/02/14 18:10:15 by klukiano         ###   ########.fr       */
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
int		look_for_path(char **envp);
int		user_cmd_path(char **args, char *arg_cmd, char **paths);
int		free_and_1(char **paths, int **end);
char	*find_cmd_path(char **paths, char *arg_cmd_2);

int	main(int ac, char **av, char **envp)
{
	int			*fd;
	int			error;

	if (look_for_path(envp) != 0)
		return (127);
	if (ac == 5)
	{
		fd = malloc(2 * sizeof(int));
		if (!fd)
			return (1);
		if (open_fds(&fd, av) != 0)
			return (1);
		error = pipex(fd, av, envp);
		if (error != 0)
		{
			free(fd);
			return (error);
		}
		free (fd);
		return (0);
	}
	return (1);
}

int	look_for_path(char **envp)
{
	int	i;

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

int	pipex(int *fd, char **av, char **envp)
{
	pid_t	pid[2];
	int		*end;
	char	**paths;
	int		status;
	int		access_status;

	pid[0] = -42;
	pid[1] = 42;
	access_status = 0;
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
			return (free_and_1(paths, &end));
		if (pid[0] == 0)
		{
			if (child_one(fd, av[2], paths, end) != 0)
			{
				free_and_1(paths, &end);
				exit (127);
			}
		}
	}
	if (pid[0] != 0)
		pid[1] = fork();
	if (pid[1] < 0)
		return (free_and_1(paths, &end));
	else if (pid[1] == 0)
	{
		if (child_two(fd, av[3], paths, end) != 0)
		{
			free_and_1(paths, &end);
			if ((av[3][0] == '.' || av[3][0] == '/') && \
			access(av[3], R_OK) == 1 && access(av[3], X_OK) == -1)
				exit (126);
			else
				exit (127);
		}
	}
	else
		if (close (fd[1]) < 0 || close(end[0]) < 0 || close(end[1]) < 0 || \
		close (fd[0]) < 0)
			;
	waitpid(pid[0], NULL, 0);
	waitpid(pid[1], &status, 0);
	free_and_1(paths, &end);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else
		return (127);
	return (0);
}

char	*find_cmd_path(char **paths, char *arg_cmd_2)
{
	(void)paths;
	if (access(arg_cmd_2, X_OK) == -1)
		ft_printf("the path was %s, coudlnt execute \n", arg_cmd_2);
	else if (access(arg_cmd_2, X_OK) != -1)
		ft_printf("the path was %s, successful exec\n", arg_cmd_2);

	return (arg_cmd_2);
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
	if (ft_strncmp(args[0], "./", 2) == 0)
		i = user_cmd_path(args, arg_cmd_2, paths);
	else if (ft_strncmp(args[0], "./", 2) != 0 && \
	!ft_strchr(args[0], '/'))
	{
		while (paths[i])
			i ++;
		i --;
		free (paths[i]);
		paths[i] = NULL;
		i = 0;
	}
	while (paths[i])
	{
		if (args[0][0] == '/')
			cmd = jointhree("", "", args[0]);
		else
			cmd = jointhree(paths[i], "/", args[0]);
		execve(cmd, args, NULL);
		free (cmd);
		i ++;
	}
	free_arr_str(args);
	perror("");
	return (1);
}

int		user_cmd_path(char **args, char *arg_cmd, char **paths)
{
	int	i;

	i = 0;
	arg_cmd += 2;
	if (args[0] && args[1])
		return (free_and_1(args, NULL));
	free (args[0]);
	args[0] = malloc((ft_strlen(arg_cmd) + 1) * 2);
	// for adding escape chars
	if (!args[0])
		return (free_and_1(args + 1, NULL));
	while (arg_cmd[i])
	{
		// if (arg_cmd[i] == '\"')
		// {
		// 	args[0][i] = '\\';
		// 	i ++;
		// 	arg_cmd = arg_cmd - 1;
		// }
		args[0][i] = arg_cmd[i];
		i ++;
	}
	args[0][i] = '\0';

	// ft_putstr_fd("\n", 2);
	// ft_putstr_fd("the cmd now is ", 2);
	// ft_putstr_fd(args[0], 2);
	// ft_putstr_fd("\n", 2);

	i = 0;
	while (paths[i])
		i ++;
	i --;
	// ft_putstr_fd("the path is ", 2);
	// ft_putstr_fd(paths[i], 2);
	// ft_putstr_fd("\n", 2);
	return (i);
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
	if (!args)
		return (1);
	i = 0;
	if (ft_strncmp(args[0], "./", 2) == 0)
		i = user_cmd_path(args, arg_cmd_1, paths);
	else if (ft_strncmp(args[0], "./", 2) != 0 && \
	!ft_strchr(args[0], '/'))
	{
		while (paths[i])
			i ++;
		i --;
		free (paths[i]);
		paths[i] = NULL;
		i = 0;
	}
	while (paths[i])
	{
		if (args[0][0] == '/' || ft_strncmp(args[0], "./", 2) == 0)
			cmd = jointhree("", "", args[0]);
		else
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
	int		i;
	char	*path;
	char	*pwd;
	char	*bigpath;

	i = 0;
	paths = NULL;
	path = NULL;
	pwd = NULL;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			path = envp[i] + 5;
		else if (ft_strncmp(envp[i], "PWD=", 4) == 0)
			pwd = envp[i] + 4;
		i ++;
	}
	bigpath = jointhree(path, ":", pwd);
	if (pwd)
		paths = ft_split(bigpath, ':');
	else
		paths = ft_split(path, ':');
	free (bigpath);
	return (paths);
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
