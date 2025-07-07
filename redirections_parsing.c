/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_parsing.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:21:08 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/07 15:10:22 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirections(char *tokens)
{
    if (!tokens)
        return (0);
        
    // Don't treat quoted strings as redirections
    if ((tokens[0] == '\'' || tokens[0] == '"') && 
        tokens[strlen(tokens) - 1] == tokens[0])
        return (0);
        
    if (ft_strcmp(tokens, "<") == 0)
        return (1);
    else if (ft_strcmp(tokens, ">") == 0)
        return (2);
    else if (ft_strcmp(tokens, ">>") == 0)
        return (3);
    else if (ft_strcmp(tokens, "<<") == 0)
        return (4);
    else
        return (0);
}

t_tree	*create_redirections(char *operator, char * file_name)
{
	t_tree	*redir;

	redir = create_command();
	if (ft_strcmp(operator, "<") == 0)
	{
		redir->type = LESS;
		redir->file_name = ft_strdup(file_name);
	}
	else if (ft_strcmp(operator, ">") == 0)
	{
		redir->type = GREATER;
		redir->file_name = ft_strdup(file_name);
	}
	else if (ft_strcmp(operator, ">>") == 0)
	{
		redir->type = APPEND;
		redir->file_name = ft_strdup(file_name);
	}
	else if (ft_strcmp(operator, "<<") == 0)
	{
		redir->type = HEREDOC;
		redir->file_name = ft_strdup(file_name);
		
	}
	return (redir);
}


int	check_redirection(char *token)
{
    if (!token)
        return (0);
    
    // Check if token is quoted - if so, it's not a redirection
    if ((token[0] == '\'' || token[0] == '"') && 
        token[strlen(token) - 1] == token[0])
        return (0);
    
    return (ft_strcmp(token, ">") == 0 || ft_strcmp(token, "<") == 0 || 
            ft_strcmp(token, ">>") == 0 || ft_strcmp(token, "<<") == 0);
}