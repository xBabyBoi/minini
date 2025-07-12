/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 15:53:13 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/12 15:56:06 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_tokenization_data(t_token_data *data, char **tokens, int *t)
{
	data->tokens = tokens;
	data->t = t;
}

void	set_env_data(t_token_data *data, char **env, int exit_status)
{
	data->env = env;
	data->exit_status = exit_status;
}

void	process_tokenization_loop(char *input, size_t len, t_token_data *data)
{
	size_t		i;
	t_segment	*head;

	i = 0;
	while (i < len)
	{
		skip_spaces(input, &i, len);
		if (i >= len)
			break ;
		head = build_segment_list(input, &i, len);
		process_segment_list(head, data);
	}
}
