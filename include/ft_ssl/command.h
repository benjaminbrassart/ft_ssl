/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 23:44:15 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/27 06:59:43 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
# define COMMAND_H

# define FT_SSL_READ_SIZE (1 << 16)
# define EXIT_FORCE 255

int execute_command(char const* command, int argc, char const** argv);

#endif // COMMAND_H
