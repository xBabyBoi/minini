/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 13:04:29 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/12 15:42:30 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_input(char *input)
{
	if (!input || !*input)
		return (0);
	if (!check_valid_quotes(input))
	{
		printf("minishell: unclosed quotes\n");
		return (0);
	}
	if (special_characters(input))
		return (0);
	return (1);
}

int	validate_tokens(char **tokens)
{
	if (!invalid_pipe(tokens))
	{
		free_token_array(tokens);
		return (0);
	}
	if (!invalid_redirections(tokens))
	{
		free_token_array(tokens);
		return (0);
	}
	return (1);
}

void	strip_quotes_from_tokens(char **tokens, int skip_heredoc_delimiter)
{
	(void)tokens;
	(void)skip_heredoc_delimiter;
}

int	handle_long_redirection(char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(&token[2], 2);
	ft_putstr_fd("'\n", 2);
	exit_status = 2;
	return (0);
}

int	handle_missing_redirection(char **tokens, int i)
{
	if (!tokens[i + 1])
	{
		ft_putstr_fd("minishell: syntax error near"
			"unexpected token `newline'\n", 2);
		exit_status = 2;
		return (0);
	}
	if (is_operator_char(tokens[i + 1][0]))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putstr_fd(tokens[i + 1], 2);
		ft_putstr_fd("'\n", 2);
		exit_status = 2;
		return (0);
	}
	return (1);
}
