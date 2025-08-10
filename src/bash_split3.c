/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bash_split3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 11:56:42 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/10 15:40:11 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	process_token_char(t_extract_state *state)
{
	if (*state->p == '\'')
		handle_quote(state, '\'');
	else if (*state->p == '\"')
		handle_quote(state, '\"');
	else if (*state->p == '\\' && !state->in_single_quote)
		handle_backslash(state);
	else
		state->token[state->i++] = *state->p++;
}

t_token_info	extract_token_with_quote_info(const char **s)
{
	t_extract_state	state;

	init_extract_state(&state, *s);
	if (!state.token)
		return (state.info);
	while (*state.p)
	{
		if (should_break(&state))
			break ;
		if (!state.in_single_quote && !state.in_double_quote
			&& is_special_char(*state.p))
		{
			handle_special_chars(&state);
			break ;
		}
		else
			process_token_char(&state);
	}
	state.token[state.i] = '\0';
	*s = state.p;
	state.info.value = state.token;
	return (state.info);
}

void	free_tokens(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
		free(tokens[i++]);
	free(tokens);
}

void	free_token_info_array(t_token_info *tokens, int count)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (i < count)
	{
		if (tokens[i].value)
			free(tokens[i].value);
		i++;
	}
	free(tokens);
}
