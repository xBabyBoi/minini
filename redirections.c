/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:39:56 by rhafidi           #+#    #+#             */
/*   Updated: 2025/07/12 17:33:04 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_quoted(char *delimiter)
{
	int	len = ft_strlen(delimiter);

	return ((delimiter[0] == '\'' && delimiter[len - 1] == '\'')
		|| (delimiter[0] == '"' && delimiter[len - 1] == '"'));
}

static void	write_heredoc_line(char *line, int quoted, int fd, char **env)
{
	char	*line_arr[2];
	char	**expanded;

	line_arr[0] = line;
	line_arr[1] = NULL;
	if (quoted)
		write(fd, line, ft_strlen(line));
	else
	{
		expanded = expand(line_arr, env, exit_status);
		write(fd, expanded[0], ft_strlen(expanded[0]));
		free_array(expanded);
	}
	write(fd, "\n", 1);
}
static void	process_heredoc_lines(char *delimiter, int pipefd[2], char **env)
{
	char	*line;
	char	*tmp_delimiter;
	int		quoted;

	tmp_delimiter = ft_strdup(delimiter);
	strip_quotes_from_tokens(&tmp_delimiter, 0);
	quoted = is_quoted(delimiter);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			write(STDOUT_FILENO, "\n", 1);
			break ;
		}
		if (!ft_strcmp(line, tmp_delimiter))
			break ;
		write_heredoc_line(line, quoted, pipefd[1], env);
	}
	free(tmp_delimiter);
}
int	handle_heredoc(char *delimiter, char **env)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe failed");
		return (-1);
	}
	signal(SIGINT, heredoc_sigint_handler);
	process_heredoc_lines(delimiter, pipefd, env);
	close(pipefd[1]);
	signal(SIGINT, sigint_handler);
	return (pipefd[0]);
}


void	append(t_tree *root, int *in, int *out)
{
	if (root->type == APPEND && root->file_name)
	{
		if (*out != STDOUT_FILENO)
			close(*out);
		*out = open(root->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (*out == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(root->file_name, 2);
			ft_putstr_fd(": Permission denied\n", 2);
			exit_status = 1;
		}
	}
}

static void	handle_less(t_tree *root, int *in)
{
	if (root->type == LESS && root->file_name)
	{
		if (*in != STDIN_FILENO)
			close(*in);
		*in = open(root->file_name, O_RDONLY);
		if (*in == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(root->file_name, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			exit_status = 1;
		}
	}
}

static void	handle_greater(t_tree *root, int *out)
{
	if (root->type == GREATER && root->file_name)
	{
		if (*out != STDOUT_FILENO)
			close(*out);
		*out = open(root->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (*out == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(root->file_name, 2);
			ft_putstr_fd(": Permission denied\n", 2);
			exit_status = 1;
		}
	}
}
void	less_and_greater(t_tree *root, int *in, int *out)
{
	handle_less(root, in);
	handle_greater(root, out);
}


static t_tree	*handle_heredoc_type(t_tree *root, t_tree *cmd, int *in, char **env)
{
	if (*in != STDIN_FILENO)
		close(*in);
	*in = handle_heredoc(root->file_name, env);
	if (*in == -1)
	{
		perror("heredoc failed");
		exit_status = 1;
		return (NULL);
	}
	return (cmd);
}

static t_tree	*process_redirection_types(t_tree *root, t_tree *cmd, int *in,
		int *out, char **env)
{
	if ((root->type == GREATER || root->type == LESS) && root->file_name)
	{
		less_and_greater(root, in, out);
		if (*in == -1 || *out == -1)
			return (NULL);
		return (cmd);
	}
	else if (root->type == APPEND && root->file_name)
	{
		append(root, in, out);
		if (*in == -1 || *out == -1)
			return (NULL);
		return (cmd);
	}
	else if (root->type == HEREDOC && root->file_name)
		return (handle_heredoc_type(root, cmd, in, env));
	return (cmd);
}

t_tree	*handle_redirections(t_tree *root, int *in, int *out, char **env)
{
	t_tree	*cmd;

	if (!root)
		return (NULL);
	if (root->type == COMMAND)
		return (root);
	if (root->type == LESS || root->type == GREATER || root->type == APPEND
		|| root->type == HEREDOC)
	{
		cmd = handle_redirections(root->left, in, out, env);
		if (!cmd && exit_status == 1)
			return (NULL);
		return (process_redirection_types(root, cmd, in, out, env));
	}
	return (handle_redirections(root->left, in, out, env));
}
