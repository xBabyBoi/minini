/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_operaotrs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 15:22:58 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/12 15:33:19 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_double_operator(t_parser *p)
{
	if (p->j > 0 && p->result[p->j - 1] != ' ')
		p->result[p->j++] = ' ';
	p->result[p->j++] = p->input[p->i];
	p->result[p->j++] = p->input[p->i + 1];
	if (p->i + 2 < p->len && p->input[p->i + 2] != ' ')
		p->result[p->j++] = ' ';
	return (p->j);
}

int	handle_single_operator(t_parser *p)
{
	if (p->j > 0 && p->result[p->j - 1] != ' ')
		p->result[p->j++] = ' ';
	p->result[p->j++] = p->input[p->i];
	if (p->i + 1 < p->len && p->input[p->i + 1] != ' ')
		p->result[p->j++] = ' ';
	return (p->j);
}

t_tree	*handle_redirection(t_tree *root, char **tokens, int i, int end)
{
	t_tree	*redir_node;

	if (i + 1 <= end)
	{
		redir_node = create_redirections(tokens[i], tokens[i + 1]);
		redir_node->left = root;
		root = redir_node;
		return (root);
	}
	else
		return (NULL);
}
