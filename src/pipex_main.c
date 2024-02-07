/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 11:24:04 by klukiano          #+#    #+#             */
/*   Updated: 2024/02/07 17:21:44 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int		pipex(int *fd, char **av, char **envp);
int		child_one(int *fd, char *arg_cmd_1, char **envp, int *end);
char	**find_path(char **envp);
char	*jointhree(char const *s1, char const *s2, char const *s3);
int		child_two(int *fd, char *arg_cmd_1, char **paths, int *end);
void	free_arr_str(char **arr);

int	main(int ac, char **av, char **envp)
{
	int	*fd;

	fd = malloc(2 * sizeof(int));
	if (ac == 5)
	{
		fd[0] = open(av[1], O_RDONLY);
		if (fd[0] < 0)
			perror("");
		fd[1] = open(av[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (fd[1] < 0)
			perror("");
		if (fd[0] < 0 || fd[1] < 0)
			return (1);
		pipex(fd, av, envp);
		free (fd);
		return (0);
	}
	free (fd);
	return (1);
}

int	pipex(int *fd, char **av, char **envp)
{
	pid_t	pid1;
	int		*end;
	char	**paths;

	end = malloc(2 * sizeof(int));
	if (pipe(end) == -1)
		return (1);
	paths = find_path(envp);
	if (!paths)
		return (1);
	pid1 = fork();
	if (pid1 < 0)
		return (1);
	if (pid1 == 0)
		if (child_one(fd, av[2], paths, end) < 0)
			return (1);
	// what happens if execve fails? should it fork still or not?
	pid1 = fork();
	if (pid1 == 0)
		if (child_two(fd, av[3], paths, end) < 0)
			return (1);
	if (close(fd[0]) < 0 || close (fd[1]) < 0 || \
	close(end[0] < 0 || close(end[1]) < 0))
		return (1);
	free_arr_str(paths);
	free (end);
	return (0);
}

int	child_two(int *fd, char *arg_cmd_2, char **paths, int *end)
{
	char	**args;
	char	*cmd;
	int		i;

	dup2(end[0], STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	if (close(end[0]) < 0 || close(end[1]) < 0 || \
	close(fd[0]) < 0 || close (fd[1]) < 0)
		return (1);
	args = ppx_split(arg_cmd_2, ' ');
	i = 0;
	while (paths[i])
	{
		cmd = jointhree(paths[i], "/", args[0]);
		execve(cmd, args, NULL);
		free (cmd);
		i ++;
	}
	free_arr_str(paths);
	free_arr_str(args);
	return (1);
}

int	child_one(int *fd, char *arg_cmd_1, char **paths, int *end)
{
	char	**args;
	char	*cmd;
	int		i;

	dup2(fd[0], STDIN_FILENO);
	dup2(end[1], STDOUT_FILENO); //now the stdin is fd[0], and stdout will be routed into the end[1] and then we read from end[0];
	if (close(end[0]) < 0 || close(fd[0]) < 0 || close (fd[1]) < 0 || close(end[1]))
		return (1);
	// !!!!!! NOT AN ABSOLUTE PATH (but shell behaves in the same way) !!!!!!!!
	// so when < ./testfilesshell/infile ls -l > ./testfilesshell/outfile it outpputs for the current folder
	args = ppx_split(arg_cmd_1, ' ');
	i = 0;
	while (paths[i])
	{
		cmd = jointhree(paths[i], "/", args[0]);
		execve(cmd, args, NULL);
		free (cmd);
		i ++;
	}
	//perror(""); //will be extra, already has error by zsh
	free_arr_str(paths);
	free_arr_str(args);
	return (1);
}

char	**find_path(char **envp)
{
	char	**paths;
	int		i;


	paths = NULL;
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			paths = ft_split(envp[i] + 5, ':');
			break ;
		}
		i ++;
	}
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
		free(arr);
	}
}
