/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_paths.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 15:49:05 by klukiano          #+#    #+#             */
/*   Updated: 2024/02/15 15:50:00 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

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

int	user_cmd_path(char **args, char *arg_cmd, char **paths)
{
	int	i;

	i = 0;
	arg_cmd += 2;
	if (args[0] && args[1])
		return (free_and_1(args, NULL));
	free (args[0]);
	args[0] = malloc(ft_strlen(arg_cmd) + 1);
	if (!args[0])
		return (free_and_1(args + 1, NULL));
	while (arg_cmd[i])
	{
		args[0][i] = arg_cmd[i];
		i ++;
	}
	args[0][i] = '\0';
	i = 0;
	while (paths[i])
		i ++;
	i --;
	return (i);
}

void	delete_pwd_path(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
		i ++;
	i --;
	free (paths[i]);
	paths[i] = NULL;
}
