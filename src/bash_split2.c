/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bash_split2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaroux <agaroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 11:56:42 by agaroux           #+#    #+#             */
/*   Updated: 2025/08/10 14:07:52 by agaroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_extract_state(t_extract_state *state, const char *p)
{
	state->p = p;
	state->token = malloc(strlen(p) + 3);
	state->i = 0;
	state->in_single_quote = 0;
	state->in_double_quote = 0;
	state->info.value = NULL;
	state->info.was_quoted = 0;
}

void	handle_special_chars(t_extract_state *state)
{
	if (state->i == 0)
	{
		state->token[state->i++] = *state->p++;
		if ((*state->p == '>' || *state->p == '<')
			&& state->token[0] == *state->p)
			state->token[state->i++] = *state->p++;
	}
}

void	handle_quote(t_extract_state *state, char quote_type)
{
	if ((quote_type == '\'' && !state->in_double_quote) || (quote_type == '\"'
			&& !state->in_single_quote))
	{
		if (quote_type == '\'')
			state->in_single_quote = !state->in_single_quote;
		else
			state->in_double_quote = !state->in_double_quote;
		state->info.was_quoted = 1;
		state->p++;
	}
	else
		state->token[state->i++] = *state->p++;
}

void	handle_backslash(t_extract_state *state)
{
	state->p++;
	if (*state->p)
		state->token[state->i++] = *state->p++;
}

int	should_break(t_extract_state *state)
{
	if (!state->in_single_quote && !state->in_double_quote)
	{
		if (isspace(*state->p) || (is_special_char(*state->p) && state->i > 0))
			return (1);
	}
	return (0);
}
