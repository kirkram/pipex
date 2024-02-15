/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_children.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 15:47:12 by klukiano          #+#    #+#             */
/*   Updated: 2024/02/15 15:47:22 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

int	child_first(int *fd, char *arg_cmd, char **paths, int *end)
{
	dup2(fd[0], STDIN_FILENO);
	dup2(end[1], STDOUT_FILENO);
	if (close(end[0]) < 0 || close(fd[0]) < 0 || \
	close (fd[1]) < 0 || close(end[1]))
	{
		perror("");
		return (1);
	}
	return (execute_child(arg_cmd, paths));
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
		execve(cmd, args, NULL);
		free (cmd);
		i ++;
	}
	free_and_1(args, NULL);
	perror("");
	return (1);
}

int	child_last(int *fd, char *arg_cmd, char **paths, int *end)
{
	dup2(end[0], STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	if (close(end[0]) < 0 || close(end[1]) < 0 || close (fd[1]) < 0)
	{
		perror("");
		return (1);
	}
	return (execute_child(arg_cmd, paths));
}
