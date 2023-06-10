/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rteles-f <rteles-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 10:34:23 by rteles-f          #+#    #+#             */
/*   Updated: 2023/06/10 03:24:18 by rteles-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	end_shell(t_control *get)
{
	input_reset(get);
	free_split(get->paths);
	safe_free_null(&get->pwd);
	exit(0);
}

void	delete_command(void *command)
{
	t_command	*get;

	get = (t_command *)command;
	free_split(get->flags);
	safe_free_null(&get->exec_path);
	safe_free_null(&get->open);
	free(command);
	// safe_close_fd(get->in_pipe[0], get->in_pipe[1]);
	// safe_close_fd(get->out_pipe[0], get->out_pipe[1]);
}

void	input_reset(t_control *get)
{
	ft_lstclear(&get->commands, (void *)delete_command);
	free_shellsplit(&get->pieces);
	safe_free_null(&get->input);
	if (get->tempfile)
	{
		unlink(get->tempfile);
		safe_free_null(&get->tempfile);
	}
}

void	safe_free_null(char **string)
{
	if (*string)
	{
		free(*string);
		*string = NULL;
	}
}