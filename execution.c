/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:52:00 by rhafidi           #+#    #+#             */
/*   Updated: 2025/07/14 15:25:11 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int get_exit_status()
{
    if (WIFEXITED(exit_status))
        return (WEXITSTATUS(exit_status));
    else if (WIFSIGNALED(exit_status))
        return (128 + WTERMSIG(exit_status)); 
    return (EXIT_FAILURE);
}

void redirecting(int in, int out)
{
    dup2(in, STDIN_FILENO);
    dup2(out, STDOUT_FILENO);
    if (in != STDIN_FILENO)
        close(in);
    if (out != STDOUT_FILENO)
        close(out);
}

void    forker(t_tree *root, t_fd *fd, char ***env, char ***exported)
{
    int pid;
    int status;
    int saved_stdin;
    int saved_stdout;
    
    if (!root->command || !root->command[0])
    {
        saved_stdin = dup(STDIN_FILENO);
        saved_stdout = dup(STDOUT_FILENO);
        redirecting(fd->in, fd->out);
        dup2(saved_stdin, STDIN_FILENO);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdin);
        close(saved_stdout);
        return ;
    }
    if (!is_builtin(root->command[0]))
    {
        saved_stdin = dup(STDIN_FILENO);
        saved_stdout = dup(STDOUT_FILENO);
        redirecting(fd->in, fd->out);
        if (fd->in == -1 || fd->out == -1)
        {
            exit_status = 1;
            dup2(saved_stdin, STDIN_FILENO);
            dup2(saved_stdout, STDOUT_FILENO);
            close(saved_stdin);
            close(saved_stdout);
            return ;
        }
        exit_status = handle_builtins(root, fd->in, fd->out, env, exported, exit_status);
        dup2(saved_stdin, STDIN_FILENO);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdin);
        close(saved_stdout);
        return ;
    }
    pid = fork();
    if (pid == -1)
    {
        perror("fork failed :");
        exit(1);
    }
    if (!pid)
    {
        signal(SIGINT, child_sigint_handler);
        signal(SIGQUIT, SIG_DFL);
        redirecting(fd->in, fd->out);
        if (fd->in == -1 || fd->out == -1)
            exit(EXIT_FAILURE);
        execute_command(root, fd->in, fd->out, *env);
    }
    else
    {
        waitpid(pid, &exit_status, 0);
        if (WIFSIGNALED(exit_status))
        {
            if (WTERMSIG(exit_status) == SIGQUIT)
            {
                ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
                exit_status = 131;
            }
            else if (WTERMSIG(exit_status) == SIGINT)
            {
                ft_putstr_fd("\n", STDERR_FILENO);
                exit_status = 130;
            }
        }
        else if (WIFEXITED(exit_status))
            exit_status = WEXITSTATUS(exit_status);
        signal(SIGINT, sigint_handler);
        signal(SIGQUIT, SIG_IGN);
    }
}

void    pipein(int *pfd)
{
    if (pipe(pfd) == -1)
    {
        perror("pipe failed");
        exit(1);
    }
}

void    close_wait(int *pfd, t_pid *pid, t_fd *fd)
{
    close (pfd[0]);
    close (pfd[1]);
    waitpid(pid->left_pid, NULL, 0);
    waitpid(pid->right_pid, &exit_status, 0);
    exit_status = get_exit_status();
    free(pid);
}

void free_exit (t_pid *pid, t_fd *fd)
{
    free(pid);
    free(fd);
    exit(exit_status);
}

void handle_pipe(t_pid *pid, t_fd *fd, char ***env, char ***exported, t_tree *root)
{
    int pfd[2];
    
    pipein(pfd);
    pid->left_pid = fork();
    if (!pid->left_pid)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        close(pfd[0]);
        fd->out = pfd[1];
        execution(root->left, fd, env, exported);
        free_exit(pid, fd);
    }
    pid->right_pid = fork();
    if (!pid->right_pid)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        close(pfd[1]);
        fd->in = pfd[0];
        execution(root->right, fd, env, exported);
        free_exit(pid, fd);
    }
    close_wait(pfd, pid, fd);
}

void    execution(t_tree *root,t_fd *fd, char ***env, char ***exported)
{
    int pfd[2];
    int ret;
    t_pid *pid;
    t_tree *cmd;

    pid = malloc(sizeof(t_pid));
    if (!pid)
        return ;
    if (!root)
    {
        free(pid);
        return ;
    }
    if (root->type == APPEND || root->type == GREATER || root->type == LESS || root->type == HEREDOC)
    {
        cmd = handle_redirections(root, &fd->in, &fd->out, env[0]);
        if (exit_status != 0)
        {
            if (fd->in != STDIN_FILENO)
                close(fd->in);
            if (fd->out != STDOUT_FILENO)
                close(fd->out);
            free(pid);
            return;
        }
        if (cmd)
            forker(cmd, fd, env, exported);
        else
            forker(NULL, fd, env, exported);
        if (fd->in != STDIN_FILENO)
            close(fd->in);
        if (fd->out != STDOUT_FILENO)
            close(fd->out);
        free(pid);
        return ;
    }
    else if (root->type == COMMAND)
        forker(root, fd, env, exported);
    else if (root->type == PIPE)
        handle_pipe(pid , fd, env, exported, root);
}

int    initialize(t_tree *root, t_fd *fd, char ***env, char ***exported)
{
    execution(root, fd, env, exported);
    return (exit_status);
}