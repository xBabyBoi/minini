/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 15:33:16 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/14 15:14:54 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	write_heredoc_line(int pipefd, char *line, char *delimiter,
		char **env)
{
	char	**expanded;

	if ((delimiter[0] == '\'' && delimiter[ft_strlen(delimiter) - 1] == '\'')
		|| (delimiter[0] == '\"' && delimiter[ft_strlen(delimiter)
				- 1] == '\"'))
	{
		write(pipefd, line, ft_strlen(line));
		write(pipefd, "\n", 1);
	}
	else
	{
		expanded = expand(&line, env, exit_status);
		write(pipefd, expanded[0], ft_strlen(expanded[0]));
		write(pipefd, "\n", 1);
		free_array(expanded);
	}
}

static void	process_heredoc_input(int pipefd, char *delimiter, char **env,
		char *tmp_delimiter)
{
	char	*line[2];

	while (1)
	{
		line[0] = readline("> ");
		line[1] = NULL;
		if (!line[0])
		{
			write(STDOUT_FILENO, "\n", 1);
			break ;
		}
		if (!ft_strcmp(line[0], tmp_delimiter))
			break ;
		write_heredoc_line(pipefd, line[0], delimiter, env);
	}
}

int	handle_heredoc(char *delimiter, char **env)
{
	int		pipefd[2];
	char	*tmp_delimiter[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe failed");
		return (-1);
	}
	signal(SIGINT, heredoc_sigint_handler);
	tmp_delimiter[0] = ft_strdup(delimiter);
	tmp_delimiter[1] = NULL;
	tmp_delimiter[0] = remove_quotes_from_string(delimiter);
	process_heredoc_input(pipefd[1], delimiter, env, tmp_delimiter[0]);
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

t_tree	*handle_heredoc_redirection(t_tree *root, int *in, char **env)
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
	return (NULL);
}
