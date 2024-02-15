/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 11:28:18 by klukiano          #+#    #+#             */
/*   Updated: 2024/02/15 15:39:43 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../lib/libft/libft.h"
# include "../lib/ft_printf/ft_printf.h"
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/wait.h>

char	**ppx_split(char const *str, char c);
int		pipex(int *fd, char **av, char **envp, int *end);
int		child_first(int *fd, char *arg_cmd_1, char **envp, int *end);
char	**find_path(char **envp);
char	*jointhree(char const *s1, char const *s2, char const *s3);
int		child_last(int *fd, char *arg_cmd_1, char **paths, int *end);
void	free_arr_str(char **arr);
int		open_fds(int **fd, char **av);
int		look_for_path(char **envp, char **av);
int		user_cmd_path(char **args, char *arg_cmd, char **paths);
int		free_and_1(char **paths, int **end);
void	delete_pwd_path(char **paths);
int		execute_child(char *arg_cmd, char **paths);
int		pipe_open(int **end, char ***paths);
int		handle_last_process(int *fd, char **av, char ***paths, int **end);
void	close_all_fds(int *fd, int *end);
int		exit_pipex(char ***paths, int **end, int status);

typedef struct s_params
{
	int	error_code;

}	t_params;

#endif
