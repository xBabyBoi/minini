/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 14:31:29 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/14 15:00:25 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*create_command_node(char **cmd_args)
{
	t_tree	*cmd_node;

	cmd_node = create_command();
	if (!cmd_node)
		return (NULL);
	cmd_node->type = COMMAND;
	cmd_node->command = cmd_args;
	return (cmd_node);
}

t_tree	*handle_redirection_token(t_tree *root, char **tokens, int *i, int end)
{
	t_tree	*redir_node;

	if (*i + 1 <= end)
	{
		redir_node = create_redirections(tokens[*i], tokens[*i + 1]);
		redir_node->left = root;
		root = redir_node;
		*i += 2;
	}
	else
		return (NULL);
	return (root);
}

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
			root = handle_redirection_token(root, tokens, &i, end);
			if (!root)
				return (NULL);
		}
		else
			i++;
	}
	return (root);
}

int	initialize_end_index(char **tokens, int end)
{
	if (end == -1)
	{
		end = 0;
		while (tokens[end])
			end++;
		end--;
	}
	return (end);
}

int	handle_redirection_skip(int i, int end, char **cmd_args, int arg_count)
{
	i += 2;
	if (i > end + 1)
	{
		free_cmd_args(cmd_args, arg_count);
		return (-1);
	}
	return (i);
}
