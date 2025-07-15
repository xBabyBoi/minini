/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:59:58 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/15 15:02:12 by yel-qori         ###   ########.fr       */
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
