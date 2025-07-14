/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 15:29:18 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/14 15:36:55 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**process_tokens(char *input, char **tokens, char **env, int exit_status)
{
	t_token_ctx	ctx;

	ctx.tokens = tokens;
	ctx.env = env;
	ctx.exit_status = exit_status;
	return (tokenize_loop(input, &ctx));
}

char	**process_tokenization(char *input, char **env, int exit_status)
{
	char	**tokens;
	char	*spaced_input;

	spaced_input = add_delimiter_spaces(input);
	if (!spaced_input)
		return (NULL);
	tokens = initial_tokenization_with_env(spaced_input, env, exit_status);
	if (check_syntax_errors(tokens))
	{
		exit_status = 2;
		return (NULL);
	}
	free(spaced_input);
	if (!tokens)
		return (NULL);
	if (!invalid_pipe(tokens))
	{
		free_token_array(tokens);
		return (NULL);
	}
	return (tokens);
}

char	**tokenize_input(char *input, char **env, int exit_status)
{
	if (!input || !*input)
		return (NULL);
	if (!check_valid_quotes(input))
	{
		printf("minishell: unclosed quotes\n");
		return (NULL);
	}
	if (special_characters(input))
		return (NULL);
	return (process_tokenization(input, env, exit_status));
}

void	process_quote_removal(char *str, char *result)
{
	char	current_quote;
	int		i;
	int		j;

	i = 0;
	j = 0;
	current_quote = 0;
	while (str[i])
	{
		if (!current_quote && (str[i] == '\'' || str[i] == '"'))
		{
			current_quote = str[i];
			i++;
		}
		else if (current_quote && str[i] == current_quote)
		{
			current_quote = 0;
			i++;
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
}

char	*remove_quotes_from_string(char *str)
{
	char	*result;
	int		len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	process_quote_removal(str, result);
	return (result);
}
