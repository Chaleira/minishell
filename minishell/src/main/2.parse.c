/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2.parse.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plopes-c <plopes-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 15:56:24 by plopes-c          #+#    #+#             */
/*   Updated: 2023/08/01 18:23:21 by plopes-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <minishell.h>

int last_split_index(char **split)
{
	int	i;
	

	i = 0;
	while (split && split[i] && split[i + 1])
		i++;
	return (i);
}

int	check_near_special_char(char **split)
{
	int	i;

	i = 0;
	while (split && split[i])
	{
		if (split[i] && split_case(split[i]))
		{
			if (split[i + 1] && split_case(split[i + 1]))
			{
				ft_printf("minishell: syntax error near unexpected token `%s'\n",
					split[i + 1]);
				return (0);
			}
		}
		i++;
	}
	return (1);
}

int	check_alone_char(char **split)
{
	if (*split && split_case(*split) && **split && **split != '(' && !split[1])
	{
		if (*(short *)(*split) == *(short *)">>" || *(short *)(*split) == *(short *)"<<"
			|| **split == '>' || **split == '<')
			ft_printf("minishell: syntax error near unexpected token `newline'\n");
		else
			ft_printf("minishell: syntax error near unexpected token `%s'\n", *split);
		return (0);
	}
	return (1);
}

int	check_first_char(char **split)
{
	if (*split && split[1] && (!ft_strncmp(*split, "|", 1)
			|| !ft_strncmp(*split, ")", 1) || !ft_strncmp(*split, "&&", 2)
			|| !ft_strncmp(*split, ";", 1) || !ft_strncmp(*split, "||", 2)))
	{
		ft_printf("minishell: syntax error near unexpected token `%s'\n",
			*split);
		return (0);
	}
	return (1);
}

int	check_last_char(char **split)
{
	int	last_split;
	
	last_split = last_split_index(split);
	if (split[last_split] && !split[last_split - 1] && split_case(split[last_split]) && *(short *)split[last_split] != *(short *)"||"
		&& *(short *)split[last_split] != *(short *)"&&" && *split[last_split]
			&& *split[last_split] != '|' && *split[last_split] != ';')
	{
		if ((!ft_strncmp(split[last_split], ">>", 2)
				|| !ft_strncmp(split[last_split], "<<", 2) || !ft_strncmp(split[last_split], ">", 1)
				|| !ft_strncmp(split[last_split], "<", 1) || !ft_strncmp(split[last_split], ")", 1)))
			ft_printf("minishell: syntax error near unexpected token `newline'\n");
		if (!ft_strncmp(split[last_split], ")", 1))
			ft_printf("minishell: syntax error near unexpected token `%s'\n", split[last_split]);
		return (0);
	}
	return (1);
}

char	*(*goto_here_doc(char **split))()
{
	int		last_split;

	last_split = last_split_index(split);
	if (!ft_strncmp(split[last_split], "||", 2) || !ft_strncmp(split[last_split], "&&", 2)
		|| !ft_strncmp(split[last_split], "(", 1) || !ft_strncmp(split[last_split], "|", 1))
		return (catch_one);
	return (NULL);
}

char	**parse(char **split, t_control *get)
{
	char	**add;
	char	*(*tmp)();

	add = NULL;
	if (!*split)
		return (NULL);
	if (!check_alone_char(split) || !check_first_char(split)
		|| !check_near_special_char(split) || !check_last_char(split))
		return (NULL);
	tmp = goto_here_doc(split);
	if (!tmp)
		return (split);
	add = shell_split(tmp(get));
	if (!add || !parse(add, get))
		return (NULL);
	return (ft_split_join(split, add, last_split_index(split)));
}

//  (   )    >    >>    <    <<    |    &&    ||	;

// Ver o par dos parenteses (syntax errror quando apareçer ")" sem um "(" antes).
// Ver o segfault quando executado "||"
// Tratar do here_doc


// // OUTROS ERROS
// Ver o "cd ~"

/*
check if executable or folder, else command not found
*/

	// while (split && split[i])
	// {
	// 	if (!ft_strncmp(split[i], "<<", 2))
	// 	{
	// 		*eof = split[i + 1];
	// 		return (here_doc);
	// 	}
	// 	i++;
	// }