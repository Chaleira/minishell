/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plopes-c <plopes-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 18:56:31 by plopes-c          #+#    #+#             */
/*   Updated: 2023/08/16 01:15:29 by plopes-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	export_execute_no_input(char *str, char **flags, char **env);
static void	export_execute_with_input(char *str, char **flags, char	**env);
char		**split_with_one_equal(char *str);
char		**env_copy(char **env, char *new_str);

void	export_prepare(t_command *command, int index)
{
	int	i[2];

	if (!command->terminal[index + 1] && !command->status)
		command->execute = export_execute_no_input;
	else
	{
		i[1] = split_size(command->terminal);
		command->flags = ft_calloc(sizeof(char *), i[1]);
		if (!command->flags)
			return ;
		i[0] = 0;
		while (command->terminal[++index])
		{
			if (!ft_isalpha(command->terminal[index][0]))
				ft_printf("Minishell: export: '%s': not a valid identifier\n",
					command->terminal[index]);
			else
				command->flags[i[0]++] = ft_strdup(command->terminal[index]);
			command->terminal[index][0] = 0;
		}
		if (!command->status)
			command->execute = export_execute_with_input;
		if (execute_now(command))
			command->status = PARENT;
	}
}

static void	export_execute_no_input(char *print, char **flags, char **env)
{
	int	i[3];

	(void)print;
	(void)flags;
	i[0] = -1;
	while (env && env[++i[0]])
	{
		i[2] = 0;
		i[1] = -1;
		write(1, "declare -x ", 11);
		while (env[i[0]][++i[1]])
		{
			write(1, &env[i[0]][i[1]], 1);
			if (env[i[0]][i[1]] == '=' && !i[2])
			{
				write(1, "\"", 1);
				i[2]++;
			}
		}
		if (i[2])
			write(1, "\"", 1);
		write(1, "\n", 1);
	}
	update_paths(env, (*control()));
}

int	ft_strlenchr(char *str, char c)
{
	int	i;

	i = 0;
	while (str && str[i] && str[i] != c)
		i++;
	return (i);
}

static void	export_execute_with_input(char *str, char **flags, char **env)
{
	char	**split;
	char	**var;
	int		i;

	(void)str;
	(void)flags;
	i = 0;
	while (flags && flags[i])
	{
		split = split_with_one_equal(flags[i]);
		var = find_var(split[0], (*control())->envp, NULL, NULL);
		if (var && *var)
		{
			if (split[1])
			{
				free(*var);
				*var = ft_strdup(flags[i]);
			}
		}
		else
			(*control())->envp = env_copy((*control())->envp, flags[i]);
		free_split(split);
		i++;
	}
	update_paths(env, (*control()));
}

char	**split_with_one_equal(char *str)
{
	char	**split;
	char	*value;
	char	*name;
	int		i;

	split = ft_calloc(sizeof(char *), 4);
	value = ft_strchr(str, '=');
	if (!value)
		split[0] = ft_strdup(str);
	else
	{
		i = 0;
		while (str && str[i] != '=')
			i++;
		name = ft_substr(str, 0, i);
		split[0] = name;
		split[1] = ft_strdup("=");
		split[2] = ft_strdup(++value);
	}
	return (split);
}
