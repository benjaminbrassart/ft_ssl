/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 22:35:54 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/27 04:49:20 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl/interactive.h"
#include "ft_ssl/command.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const* argv[])
{
    if (argc == 1)
        return run_interactive();

    return execute_command(argv[1], argc - 1, &argv[1]);
}
