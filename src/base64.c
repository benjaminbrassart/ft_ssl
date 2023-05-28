/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 03:44:35 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/28 05:13:26 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl/base64.h"
#include <stdint.h>

static char const BASE64[64] __attribute__((unused)) =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void base64_init(Base64Context* context)
{
    context->output = NULL;
    context->capacity = 0;
    context->size = 0;
}

int base64_update(Base64Context* context, void const* data, size_t len)
{
    uint8_t const* bytes = (uint8_t const*)data;

    (void)context;
    (void)bytes;
    (void)len;
    return 0;
}

int base64_finalize(Base64Context* context)
{
    (void)context;
    return 0;
}
