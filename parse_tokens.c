/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:52:23 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/14 14:32:06 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	add_argument_to_array(char **tokens, char **cmd_args, int i,
		int arg_count)
{
	cmd_args[arg_count] = ft_strdup(tokens[i]);
	if (!cmd_args[arg_count])
	{
		free_cmd_args(cmd_args, arg_count);
		return (-1);
	}
	return (0);
}

static char	**process_tokens_loop(char **tokens, int start, int end,
		char **cmd_args)
{
	int	i;
	int	arg_count;

	arg_count = 0;
	i = start;
	while (i <= end)
	{
		if (is_redirections(tokens[i]))
		{
			i = handle_redirection_skip(i, end, cmd_args, arg_count);
			if (i == -1)
				return (NULL);
			continue ;
		}
		if (add_argument_to_array(tokens, cmd_args, i, arg_count) == -1)
			return (NULL);
		arg_count++;
		i++;
	}
	cmd_args[arg_count] = NULL;
	return (cmd_args);
}

char	**collect_command_arguments(char **tokens, int start, int end)
{
	char	**cmd_args;

	cmd_args = malloc(sizeof(char *) * (end - start + 2));
	if (!cmd_args)
		return (NULL);
	return (process_tokens_loop(tokens, start, end, cmd_args));
}

t_tree	*parse_token_subset(char **tokens, int start, int end)
{
	int		i;
	int		arg_count;
	t_tree	*ast;
	t_tree	*cmd_node;
	t_tree	*root;
	t_tree	*redir_node;
	char	**cmd_args;

	end = initialize_end_index(tokens, end);
	if (start > end || !tokens[start])
		return (NULL);
	cmd_args = collect_command_arguments(tokens, start, end);
	if (!cmd_args)
		return (NULL);
	cmd_node = create_command_node(cmd_args);
	if (!cmd_node)
	{
		free(cmd_args);
		return (NULL);
	}
	ast = process_redirections(cmd_node, tokens, start, end);
	strip_quotes_from_ast(ast);
	return (ast);
}

t_tree	*parse_tokens(char **tokens)
{
	int		i;
	int		start;
	int		heredoc_flag;
	t_tree	*ast;
	t_tree	*pipe_node;
	t_tree	*cmd_node;
	t_tree	*redirection_node;

	start = 0;
	i = 0;
	while (tokens[i])
	{
		heredoc_flag = 0;
		if (ft_strcmp(tokens[i], "|") == 0)
		{
			pipe_node = create_pipe_node();
			pipe_node->left = parse_token_subset(tokens, start, i - 1);
			start = i + 1;
			pipe_node->right = parse_tokens(tokens + start);
			return (pipe_node);
		}
		i++;
	}
	return (parse_token_subset(tokens, 0, -1));
}
