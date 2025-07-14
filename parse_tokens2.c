/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 14:22:17 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/14 14:23:10 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_command_array(char **command)
{
	int		i;
	char	*new_str;

	i = 0;
	while (command[i])
	{
		new_str = remove_quotes_from_string(command[i]);
		if (new_str)
		{
			free(command[i]);
			command[i] = new_str;
		}
		i++;
	}
}

void	handle_command_node(t_tree *ast)
{
	if (ast->type == COMMAND && ast->command)
		process_command_array(ast->command);
}

void	handle_heredoc_node(t_tree *ast)
{
	if (ast->type == HEREDOC)
	{
		if (ast->left)
			strip_quotes_from_ast(ast->left);
		return ;
	}
}

void	process_child_nodes(t_tree *ast)
{
	if (ast->left)
		strip_quotes_from_ast(ast->left);
	if (ast->right)
		strip_quotes_from_ast(ast->right);
}

void	strip_quotes_from_ast(t_tree *ast)
{
	if (!ast)
		return ;
	handle_command_node(ast);
	handle_heredoc_node(ast);
	if (ast->type == HEREDOC)
		return ;
	process_child_nodes(ast);
}
