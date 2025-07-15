/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:52:23 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/15 15:16:11 by yel-qori         ###   ########.fr       */
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

static t_tree	*create_command_with_args(char **tokens, int start, int end)
{
	char	**cmd_args;
	t_tree	*cmd_node;

	cmd_args = collect_command_arguments(tokens, start, end);
	if (!cmd_args)
		return (NULL);
	cmd_node = create_command_node(cmd_args);
	if (!cmd_node)
	{
		free(cmd_args);
		return (NULL);
	}
	return (cmd_node);
}

t_tree	*parse_token_subset(char **tokens, int start, int end)
{
	t_tree	*ast;
	t_tree	*cmd_node;

	end = initialize_end_index(tokens, end);
	if (start > end || !tokens[start])
		return (NULL);
	cmd_node = create_command_with_args(tokens, start, end);
	if (!cmd_node)
		return (NULL);
	ast = process_redirections(cmd_node, tokens, start, end);
	strip_quotes_from_ast(ast);
	return (ast);
}
