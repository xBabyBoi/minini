/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 14:18:22 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/15 15:10:13 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_redirection_syntax(char **tokens, int i)
{
	if (!tokens[i + 1])
	{
		print_syntax_error(NULL);
		return (1);
	}
	if (is_redirection(tokens[i + 1]) || is_pipe(tokens[i + 1]))
	{
		print_syntax_error(tokens[i + 1]);
		return (1);
	}
	return (0);
}

int	check_syntax_errors(char **tokens)
{
	int	token_count;

	if (!tokens || !tokens[0])
		return (0);
	if (check_initial_pipe(tokens))
		return (1);
	token_count = check_tokens_loop(tokens);
	if (token_count == -1)
		return (1);
	if (check_final_pipe(tokens, token_count))
		return (1);
	return (0);
}

int	check_tokens_loop(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		if (is_redirection(tokens[i]))
		{
			if (check_redirection_syntax(tokens, i))
				return (-1);
		}
		else if (is_pipe(tokens[i]))
		{
			if (check_pipe_syntax(tokens, i))
				return (-11);
		}
		i++;
	}
	return (i);
}

int	check_final_pipe(char **tokens, int token_count)
{
	if (token_count > 0 && is_pipe(tokens[token_count - 1]))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (1);
	}
	return (0);
}

int	check_initial_pipe(char **tokens)
{
	if (is_pipe(tokens[0]))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (1);
	}
	return (0);
}
