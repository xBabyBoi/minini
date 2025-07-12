/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:52:23 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/12 15:33:16 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*process_redirections(t_tree *cmd_node, char **tokens, int start,
		int end)
{
	int		i;
	int		redir_type;
	t_tree	*root;

	root = cmd_node;
	i = start;
	while (i <= end)
	{
		redir_type = is_redirections(tokens[i]);
		if (redir_type)
		{
			root = handle_redirection(root, tokens, i, end);
			if (!root)
				return (NULL);
			i += 2;
		}
		else
			i++;
	}
	return (root);
}

char	**process_arguments(char **cmd_args, char **tokens, int start, int end)
{
	int	i;
	int	arg_count;

	arg_count = 0;
	i = start;
	while (i <= end)
	{
		if (is_redirections(tokens[i]))
		{
			i += 2;
			if (i > end + 1)
				return (handle_error_cleanup(cmd_args, arg_count));
			continue ;
		}
		cmd_args[arg_count] = ft_strdup(tokens[i]);
		if (!cmd_args[arg_count])
			return (handle_error_cleanup(cmd_args, arg_count));
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
	return (process_arguments(cmd_args, tokens, start, end));
}

t_tree	*parse_token_subset(char **tokens, int start, int end)
{
	int		arg_count;
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
	return (process_redirections(cmd_node, tokens, start, end));
}

t_tree	*parse_tokens(char **tokens)
{
	int		i;
	int		start;
	t_tree	*pipe_node;
	t_tree	*cmd_node;
	t_tree	*redirection_node;

	start = 0;
	i = 0;
	while (tokens[i])
	{
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
