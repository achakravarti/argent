/*******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * Argent---infrastructure for building web services
 * Copyright (C) 2020 Abhishek Chakravarti
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTIBILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <https://www.gnu.org/licenses/>.
 *
 * You can contact Abhishek Chakravarti at <abhishek@taranjali.org>.
 ******************************************************************************/


#include "../include/argent.h"

#include <stdint.h>


extern ag_hash
ag_hash_new(size_t key)
{
        key = (key ^ (key >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
        key = (key ^ (key >> 27)) * UINT64_C(0x94d049bb133111eb);
        key = key ^ (key >> 31);
    
        return key;
}


extern ag_hash
ag_hash_new_str(const char *key)
{
        AG_ASSERT_PTR (key);

        register ag_hash hash = 5381;
        register int c;

        while ((c = *key++))
                hash = ((hash << 5) + hash) + c;

        return hash;
}

