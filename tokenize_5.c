/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_5.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 13:08:16 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/12 15:56:00 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**initial_tokenization_with_env(char *input, char **env,
		int exit_status)
{
	size_t			len;
	char			**tokens;
	int				t;
	t_token_data	data;

	len = ft_strlen(input);
	tokens = malloc(sizeof(char *) * (len + 2));
	t = 0;
	init_tokenization_data(&data, tokens, &t);
	set_env_data(&data, env, exit_status);
	process_tokenization_loop(input, len, &data);
	tokens[t] = NULL;
	return (tokens);
}

char	**tokenize_input(char *input, char **env, int exit_status)
{
	char	**tokens;
	char	*spaced_input;

	if (!validate_input(input))
		return (NULL);
	spaced_input = prepare_spaced_input(input);
	if (!spaced_input)
		return (NULL);
	tokens = initial_tokenization_with_env(spaced_input, env, exit_status);
	free(spaced_input);
	if (!tokens || !validate_tokens(tokens))
		return (NULL);
	return (tokens);
}

char	*merge_tokens(char **tokens, int start, int end)
{
	char	*merged;
	char	*tmp;
	int		i;

	merged = ft_strdup("");
	i = start;
	while (i <= end)
	{
		tmp = ft_strjoin(merged, tokens[i]);
		free(merged);
		merged = tmp;
		if (i != end)
		{
			tmp = ft_strjoin(merged, " ");
			free(merged);
			merged = tmp;
		}
		i++;
	}
	return (merged);
}

char	*prepare_spaced_input(char *input)
{
	char	*spaced_input;

	spaced_input = add_delimiter_spaces(input);
	if (!spaced_input)
		return (NULL);
	return (spaced_input);
}

t_segment	*get_next_segment(const char *input, size_t *i)
{
	if (input[*i] == '\'' || input[*i] == '"')
		return (handle_quoted_segment(input, i));
	else
		return (handle_unquoted_segment(input, i));
}
