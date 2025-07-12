/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 18:24:42 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/12 15:35:38 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	is_pipe(char *token)
{
	if (!token)
		return (0);
	return (ft_strcmp(token, "|") == 0 && token[0] != '"');
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

void	update_quote_state(char c, char *quote)
{
	if (c == '\'' || c == '"')
	{
		if (!*quote)
			*quote = c;
		else if (*quote == c)
			*quote = 0;
	}
}
