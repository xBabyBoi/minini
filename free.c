/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:59:58 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/15 21:23:15 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_fds_and_pid(t_fd *fd, t_pid *pid)
{
	if (fd->in != STDIN_FILENO)
		close(fd->in);
	if (fd->out != STDOUT_FILENO)
		close(fd->out);
	free(pid);
}

void	handle_empty_command(t_fd *fd)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	redirecting(fd->in, fd->out);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

void    free_array(char **str)
{
    int i;

    i = 0;
    while (str[i])
    {
        free(str[i]);
        str[i] = NULL;
        i++;
    }
    free(str);
    str = NULL;
}
