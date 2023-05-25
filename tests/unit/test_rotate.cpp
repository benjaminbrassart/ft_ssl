/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_rotate.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 21:18:34 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/25 22:41:03 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl/rotate.h"
#include <cassert>

int main()
{
    {
        auto n = 0x01ULL;

        assert(rotate_left_u64(n, 0) == n);
        assert(rotate_left_u64(n, 1) == 0x02ULL);
        assert(rotate_right_u64(n, 0) == n);
        assert(rotate_right_u64(n, 1) == 0x8000000000000000);
    }
}
