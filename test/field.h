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


#ifndef __ARGENT_TEST_FIELD_H__
#define __ARGENT_TEST_FIELD_H__


#define AG_SAMPLE_FIELD(tag, key, val)                          \
        static inline ag_field *SAMPLE_FIELD_ ## tag(void)      \
        {                                                       \
                AG_AUTO(ag_value) *k = key;                     \
                AG_AUTO(ag_value) *v = val;                     \
                return ag_field_new(k, v);                      \
        }


#endif /* !__ARGENT_TEST_FIELD_H__ */

