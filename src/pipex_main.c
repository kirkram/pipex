/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 11:24:04 by klukiano          #+#    #+#             */
/*   Updated: 2024/02/06 18:59:49 by klukiano         ###   ########.fr       */
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

int	main(int ac, char **av, char **envp)
{
	int	*fd;

	fd = malloc(2 * sizeof(int));
	// while (*envp)
	// {
	// 	printf("The envp is: %s\n", *envp);
	// 	envp += 1;
	// }
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
	}
	return (0);
}

int		pipex(int *fd, char **av, char **envp)
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
	if (pid1 != 0)
	{
		child_one(fd, av[2], paths, end);
	}
	pid1 = fork();
	if (pid1 != 0)
		child_two(fd, av[3], paths, end);

	// char *buffer = malloc(4096);
	// read (end[0], buffer, 495);
	// buffer[494] = 0; // read cannot be -1 to underflow to size max it must be a positive value
	// ft_printf("the buff now contains %s\n", buffer);
	// free (buffer);
	if (close(fd[0]) < 0 || close (fd[1]) < 0 || close(end[0] < 0 || close(end[1]) < 0))
		return (1);
	free (paths); //make it proper free for all splits
	free (end);
	return (0);
}

int		child_two(int *fd, char *arg_cmd_2, char **paths, int *end)
{
	char	**args;
	char	*cmd;
	int		i;

	dup2(end[0], STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	if (close(end[0]) < 0 || close(end[1]) < 0 || close(fd[0]) < 0 || close (fd[1]) < 0)
		return (1);
	args = ft_split(arg_cmd_2, ' ');
	i = 0;
	while (paths[i])
	{
		cmd = jointhree(paths[i], "/", args[0]);
		execve(cmd, args, NULL);
		free (cmd);
		i ++;
	}
	perror("");

	return (1);
}

int		child_one(int *fd, char *arg_cmd_1, char **paths, int *end)
{
	char	**args;
	char	*cmd;
	int		i;
	//modern pipes are 64k 4k is old standard

	dup2(fd[0], STDIN_FILENO);
	dup2(end[1], STDOUT_FILENO); //now the stdout will be routed into the end[1] and then we read from end[0];
	// if = 0 -> too much info in file; gnl seems like overkill for now
	//printf("the 0 is %d and 1 is %d\n", end[0], end[1]);
	if (close(end[0]) < 0 || close(fd[0]) < 0 || close (fd[1]) < 0 || close(end[1]))
		return (1);

	// !!!!!! NOT AN ABSOLUTE PATH (but shell behaves in the same way) !!!!!!!!
	// so when < ./testfilesshell/infile ls -l > ./testfilesshell/outfile it outpputs for the current folder
	args = ft_split(arg_cmd_1, ' ');
	// while (args[i])
	// {
	// 	ft_printf("[%d] - %s\n", i, args[i]);
	// 	i ++;
	// }
	i = 0;
	while (paths[i])
	{
		cmd = jointhree(paths[i], "/", args[0]);
		execve(cmd, args, NULL);
		free (cmd);
		i ++;
	}
	perror("");
	// i = 0;
	// while (args[i])
	// {
	// 	free (args[i]);
	// 	i ++;
	// }
	// free (args);
	// i = 0;
	// while (paths[i])
	// {
	// 	free (paths[i]);
	// 	i ++;
	// }
	// free (paths);
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
