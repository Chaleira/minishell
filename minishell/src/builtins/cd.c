/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plopes-c <plopes-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 16:31:03 by plopes-c          #+#    #+#             */
/*   Updated: 2023/06/07 19:59:25 by plopes-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cd_exec(t_command *command, int index)
{
	char	path[256];
	char	*joined;

	if (command->terminal[index + 1] && command->terminal[index + 1][0] == '/')
		joined = ft_strdup(command->terminal[index + 1]);
	else
	{
		getcwd(path, sizeof(path));
		joined = ft_strjoin(path, "/");
		ft_stradd(&joined, command->terminal[index + 1]);
	}
	if (command->main->pipes > 0)
		return ;
	if (chdir(joined))
		ft_printf("cd: not a directory: %s\n", joined);
	free(joined);
	command->valid = 0;
}

// void	cd_builtin(t_command *command)
// {
// 	command->execute = cd_exec;
// 	command->valid = 0;
// }

// void	folder_exists(char *path, char *folder)
// {
// 	struct stat info;
	
// 	if (!stat(path, &info) && )
		
// }