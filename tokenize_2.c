/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 13:02:21 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/12 15:47:52 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_double_quoted_segment(char *result, t_segment *seg,
		char **env, int exit_status)
{
	char	*expanded;

	if (is_operator_char(seg->str[0]) && strlen(seg->str) == 1)
	{
		strcat(result, "\"");
		strcat(result, seg->str);
		strcat(result, "\"");
	}
	else
	{
		expanded = expand_string(seg->str, env, exit_status);
		strcat(result, expanded);
		if (expanded != seg->str)
			free(expanded);
	}
}

void	append_unquoted_segment(char *result, t_segment *seg, char **env,
		int exit_status)
{
	char	*expanded;

	expanded = expand_string(seg->str, env, exit_status);
	strcat(result, expanded);
	if (expanded != seg->str)
		free(expanded);
}

void	append_segment_to_result(char *result, t_segment *seg, char **env,
		int exit_status)
{
	if (seg->quote == '\'')
		append_quoted_segment(result, seg);
	else if (seg->quote == '"')
		append_double_quoted_segment(result, seg, env, exit_status);
	else
		append_unquoted_segment(result, seg, env, exit_status);
}

void	skip_spaces(const char *input, size_t *i, size_t len)
{
	while (*i < len && input[*i] == ' ')
		(*i)++;
}

void	process_segment_list(t_segment *head, t_token_data *data)
{
	char	*merged;

	if (head)
	{
		merged = merge_and_expand_segments(head, data->env, data->exit_status);
		data->tokens[(*(data->t))++] = merged;
		free_segments(head);
	}
}
