/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_token4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:15:39 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/15 15:16:16 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_tree	*create_pipe_structure(char **tokens, int start, int pipe_pos)
{
	t_tree	*pipe_node;

	pipe_node = create_pipe_node();
	pipe_node->left = parse_token_subset(tokens, start, pipe_pos - 1);
	pipe_node->right = parse_tokens(tokens + pipe_pos + 1);
	return (pipe_node);
}

t_tree	*parse_tokens(char **tokens)
{
	int		i;
	int		start;

	start = 0;
	i = 0;
	while (tokens[i])
	{
		if (ft_strcmp(tokens[i], "|") == 0)
			return (create_pipe_structure(tokens, start, i));
		i++;
	}
	return (parse_token_subset(tokens, 0, -1));
}
