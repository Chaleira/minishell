/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rteles-f <rteles-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 16:24:58 by rteles-f          #+#    #+#             */
/*   Updated: 2023/06/10 00:00:39 by rteles-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	pipe_output(){}
void	export_builtin(){}
void	unset_builtin(){}

void	do_nothing(void)
{
	return ;
}

t_command	*new_command(t_control *get)
{
	t_command	*new;

	new = ft_calloc(sizeof(t_command), 1);
	new->main = get;
	new->valid = 1;
	new->parse = 1;
	new->in_pipe[0] = get->in_out[0];
	new->out_pipe[1] = get->in_out[1];
	// new->in_pipe[0] = STDIN_FILENO;
	// new->out_pipe[1] = STDOUT_FILENO;
	new->execute = do_nothing;
	return (new);
}

char	*build_executable_path(t_control *get, char *command)
{
	int		i;
	char	*exec_path;

	if ((command[0] == '/' || command[0] == '.') &&
		!(command[0] == '.' && command[1] == '.') && !access(command, F_OK))
		return (ft_strdup(command));
	i = 0;
	while (get->paths[i])
	{
		exec_path = ft_strjoin(get->paths[i++], command);
		if (!access(exec_path, F_OK))
			return (exec_path);
		free(exec_path);
	}
	write (2, "command not found: ", 19);
	i = 0;
	while (command[i])
		write (2, &command[i++], 1);
	write (2, "\n", 1);
	return (NULL);
}

char	**copy_split(char **split)
{
	char	**new;
	int		counter;

	counter = 0;
	while (split[counter] && !split_case(split[counter]))
		counter++;
	new = ft_calloc(sizeof(char *), counter + 1);
	counter = 0;
	while (split[counter] && !split_case(split[counter]))
	{
		new[counter] = ft_strdup(split[counter]);
		counter++;
	}
	return (new);
}

void	try_command(t_command *get, int index)
{
	get->exec_path = build_executable_path(get->main, get->terminal[index]);
	if (!get->exec_path)
	{
		get->valid = 0;
		return ;
	}
	get->flags = copy_split(&get->terminal[index++]);
	while (get->terminal[index] && !split_case(get->terminal[index]))
		get->terminal[index++][0] = 0;
	get->execute = (void *)execve;
}

t_exe	solve(char *find)
{
	int				index;
	int				length;
	static char		*cases[14] = {
		"", ">>", "<<", ">",
		"<", "|", "echo", "cd",
		"pwd", "export", "unset", "env",
		"exit", NULL
	};
	static t_exe	functions[14] = {
		do_nothing, output_redirect, input_redirect, output_redirect,
		input_redirect, pipe_output, echo_prepare, cd_prepare,
		pwd_prepare, export_prepare, unset_prepare, env_prepare,
		exit_execute, try_command
	};

	length = ft_strlen(find);
	index = 0;
	while (cases[index] && ft_strncmp(find, cases[index], length))
		index++;
	return (functions[index]);
}

void	builtin_execute(char *print)
{
	ft_printf("%s", print);
}

void	structure_commands(t_control *get)
{
	t_command	*command;
	int			i;
	int			j;

	i = 0;
	while (get->pieces && get->pieces[i])
	{
		command = new_command(get);
		command->terminal = get->pieces[i];
		j = 0;
		while (get->pieces[i][j] && command->parse)
		{
			(solve(get->pieces[i][j]))(command, j);
			j++;
		}
		if (command->valid)
			ft_lstadd_back(&get->commands, ft_lstnew((void *)command));
		else
			delete_command(command);
		i++;
	}
}