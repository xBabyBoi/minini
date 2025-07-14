/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 15:24:25 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/14 15:34:12 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_complete_quoted_token(char *token)
{
	char	first;
	char	last;

	if (!token || strlen(token) < 2)
		return (0);
	first = token[0];
	last = token[strlen(token) - 1];
	return ((first == '\'' || first == '\"') && first == last);
}

int	find_token_end(char *input, int i, char *quote, int *quote_type)
{
	*quote = 0;
	*quote_type = -1;
	while (input[i])
	{
		if (input[i] == '\'' && !(*quote))
		{
			*quote = input[i];
			*quote_type = 0;
		}
		else if (input[i] == '\"' && !(*quote))
		{
			*quote = input[i];
			*quote_type = 1;
		}
		else if (input[i] == *quote)
			*quote = 0;
		else if (!(*quote) && input[i] == ' ')
			break ;
		i++;
	}
	return (i);
}

char	*create_token(char *input, int start, int len, t_token_ctx *ctx)
{
	char	*token;

	token = malloc(len + 1);
	if (!token)
		return (NULL);
	strncpy(token, input + start, len);
	token[len] = '\0';
	token = expand_string(token, ctx->env, ctx->exit_status, 0);
	return (token);
}

int	process_one_token(char *input, int i, t_token_ctx *ctx)
{
	int		start;
	int		len;
	char	quote;
	int		quote_type;

	start = i;
	i = find_token_end(input, i, &quote, &quote_type);
	len = i - start;
	ctx->tokens[ctx->token_count] = create_token(input, start, len, ctx);
	if (!ctx->tokens[ctx->token_count])
		return (-1);
	ctx->token_count++;
	return (i);
}

char	**tokenize_loop(char *input, t_token_ctx *ctx)
{
	int	i;

	i = 0;
	ctx->token_count = 0;
	while (input[i])
	{
		i = skip_whitespace(input, i);
		if (!input[i])
			break ;
		i = process_one_token(input, i, ctx);
		if (i == -1)
			return (cleanup_tokens(ctx->tokens, ctx->token_count));
	}
	ctx->tokens[ctx->token_count] = NULL;
	return (ctx->tokens);
}

char	**initial_tokenization_with_env(char *input, char **env, int exit_status)
{
	t_token_ctx	ctx;

	ctx.tokens = malloc(sizeof(char *) * 1000);
	if (!ctx.tokens)
		return (NULL);
	ctx.env = env;
	ctx.exit_status = exit_status;
	return (tokenize_loop(input, &ctx));
}
