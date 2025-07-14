/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 18:24:42 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/14 15:42:41 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirection(char *token)
{
	return (!ft_strcmp(token, "<") || !ft_strcmp(token, ">")
		|| !ft_strcmp(token, ">>") || !ft_strcmp(token, "<<"));
}

void	syntaxe_error(void)
{
	ft_putstr_fd("bash: syntax error near unexpected token", 2);
	exit_status = 2;
}

int	is_pipe(char *token)
{
	return (!ft_strcmp(token, "|"));
}

int	check_pipe_syntax(char **tokens, int i)
{
	if (!tokens[i + 1] || is_pipe(tokens[i + 1]))
	{
		print_syntax_error(tokens[i + 1]);
		exit_status = 2;
		return (1);
	}
	return (0);
}

void	print_syntax_error(char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (!token)
		ft_putstr_fd("newline", 2);
	else
		ft_putstr_fd(token, 2);
	ft_putstr_fd("'\n", 2);
	exit_status = 2;
}
