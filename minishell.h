/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:23:36 by rhafidi           #+#    #+#             */
/*   Updated: 2025/07/15 21:32:00 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Error codes matching bash behavior
# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1
# define EXIT_BUILTIN_ERROR 2
# define EXIT_EXEC_ERROR 126
# define EXIT_CMD_NOT_FOUND 127
# define EXIT_INVALID_EXIT 128
# define EXIT_SIGINT_CODE 130
# define EXIT_SIGQUIT_CODE 131

# include "libft/libft.h"
# include "parsing.h"
# include <errno.h>
# include <fcntl.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_expand_context
{
	char	**env;
	int		status;
	int		heredoc;
	int		*in_single_quote;
	int		*in_double_quote;
}			t_expand_context;

typedef struct s_pid
{
	int		right_pid;
	int		left_pid;
}			t_pid;

typedef struct s_pipe_data
{
	t_pid	*pid;
	t_fd	*fd;
	char	***env;
	char	***exported;
}			t_pipe_data;

void		close_wait(int *pfd, t_pid *pid, t_fd *fd);
;
void		pipein(int *pfd);
void		handle_left_child(t_pipe_data *data, int *pfd, t_tree *root);
int			find_valid_command(t_tree *root);
void		handle_right_child(t_pipe_data *data, int *pfd, t_tree *root);
void		handle_pipe(t_pipe_data *data, t_tree *root);
void		forker(t_tree *root, t_fd *fd, char ***env, char ***exported);
void		cleanup_fds_and_pid(t_fd *fd, t_pid *pid);
void		execute_command(t_tree *root, int in, int out, char **env);
void		redirecting(int in, int out);
void		free_tree(t_tree **root);
t_tree		*handle_redirections(t_tree *root, int *in, int *out, char **env);
int			is_builtin(char *cmd);
void		rdirectin_out(int in, int out);
int			handle_builtins(t_tree *root, int in, int out, char ***env,
				char ***exported, int status);
void		free_exit(t_pid *pid, t_fd *fd);
void		execution(t_tree *root, t_fd *fd, char ***env, char ***exported);
void		free_array(char **str);
char		**copy_env(char **env);
int			ft_strcmp(char *s1, char *s2);
void		reset_signals_for_child(void);
int			initialize(t_tree *root, t_fd *fd, char ***env, char ***exported);
void		sigint_handler(int sig);
void		ctrl_d_handle(int sig);
void		heredoc_sigint_handler(int sig);
void		child_sigint_handler(int sig);
int			find_equal(char *str);
int			find_start(char *s);
char		**expand(char **argv, char **env, int status);
int			compare_var_env(char *arg, char **env);
void		strip_quotes_from_tokens(char **tokens, int skip_heredoc_delimiter);
extern int	exit_status;

#endif