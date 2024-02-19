/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 11:28:18 by klukiano          #+#    #+#             */
/*   Updated: 2024/02/19 10:56:20 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../lib/libft/libft.h"
# include "../lib/ft_printf/ft_printf.h"
# include "../include/ppx_split.h"
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/wait.h>

int			pipex(int *fd, char **av, char **envp, int *end);
int			child_first(int *fd, char **av, char **envp, int *end);
char		**find_path(char **envp);
char		*jointhree(char const *s1, char const *s2, char const *s3);
int			child_last(int *fd, char **av, char **paths, int *end);
int			open_fds(int **fd, char **av);
int			look_for_path(char **envp, char **av);
int			user_cmd_path(char **args, char *arg_cmd, char **paths);
int			free_and_1(char **paths, int **end);
void		delete_pwd_path(char **paths);
int			execute_child(char *arg_cmd, char **paths);
int			pipe_open(int **end, char ***paths);
int			handle_last_process(int *fd, char **av, char ***paths, int **end);
void		close_all_fds(int *fd, int *end);
int			exit_pipex(char ***paths, int **end, int status);
int			msg_stderr(char *message, char *cmd, int err_code);
int			handle_failed_execve(char *arg_cmd);

typedef struct s_paths
{
	int		i;
	char	*path;
	char	*pwd;
	char	*bigpath;
	char	**paths;

}	t_paths;

#endif
