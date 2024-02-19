/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_children.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 15:47:12 by klukiano          #+#    #+#             */
/*   Updated: 2024/02/19 14:58:54 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

int	child_first(int *fd, char **av, char **paths, int *end)
{
	dup2(fd[0], STDIN_FILENO);
	dup2(end[1], STDOUT_FILENO);
	if (close (end[0]) < 0 || close (fd[0]) < 0 || \
	close (fd[1]) < 0 || close (end[1]) < 0)
	{
		perror("pipex: ");
		return (1);
	}
	execute_child(av[2], paths);
	return (handle_failed_execve(av[2]));
}

int	execute_child(char *arg_cmd, char **paths)
{
	char	**args;
	char	*cmd;
	int		i;

	args = ppx_split(arg_cmd, ' ');
	if (!args)
		return (1);
	i = 0;
	if (ft_strncmp(args[0], "./", 2) == 0)
		i = user_cmd_path(args, arg_cmd, paths);
	else if (ft_strncmp(args[0], "./", 2) != 0 && !ft_strchr(args[0], '/'))
		delete_pwd_path(paths);
	while (paths[i])
	{
		if (args[0][0] == '/')
			cmd = jointhree("", "", args[0]);
		else
			cmd = jointhree(paths[i], "/", args[0]);
		if (!access(cmd, F_OK) && !access(cmd, X_OK))
			execve(cmd, args, NULL);
		free (cmd);
		i ++;
	}
	free_and_1(args, NULL);
	return (1);
}

int	handle_last_process(int *fd, char **av, char ***paths, int **end)
{
	if (child_last(fd, av, *paths, *end) == 126)
	{
		free_and_1(*paths, end);
		exit (126);
	}
	else
	{
		free_and_1(*paths, end);
		exit (127);
	}
	exit (127);
}

int	child_last(int *fd, char **av, char **paths, int *end)
{
	dup2(end[0], STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	if (close(end[0]) < 0 || close(end[1]) < 0 || close (fd[1]) < 0)
	{
		perror("pipex: ");
		return (1);
	}
	execute_child(av[3], paths);
	return (handle_failed_execve(av[3]));
}

int	handle_failed_execve(char *arg_cmd)
{
	if (arg_cmd[0] == 0)
		return (msg_stderr("pipex: permission denied: ", arg_cmd, 126));
	else if (arg_cmd[0] == '.' && arg_cmd[1] == 0)
		return (msg_stderr(".: not enough arguments", NULL, 1));
	else if (access(arg_cmd, F_OK) == -1 && ft_strchr(arg_cmd, '/'))
		return (msg_stderr("pipex: no such file or directory ", arg_cmd, 127));
	else if (access(arg_cmd, F_OK) == -1)
		return (msg_stderr("pipex: command not found: ", arg_cmd, 127));
	else if (access(arg_cmd, X_OK) == -1 || access(arg_cmd, R_OK) == -1 || \
	ft_strncmp(arg_cmd, "./", 3) == 0)
		return (msg_stderr("pipex: permission denied: ", arg_cmd, 126));
	else
		return (msg_stderr("pipex: permission denied: ", arg_cmd, 127));
	return (127);
}
