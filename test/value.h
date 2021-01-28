/*-
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
 */


#ifndef __ARGENT_TEST_VALUE_H__
#define __ARGENT_TEST_VALUE_H__


#define AG_SAMPLE_VALUE_INT(tag, val)           \
        static inline ag_value *tag(void)       \
        {                                       \
                return ag_value_new_int(val);   \
        }


#define AG_SAMPLE_VALUE_UINT(tag, val)          \
        static inline ag_value *tag(void)       \
        {                                       \
                return ag_value_new_uint(val);  \
        }


#define AG_SAMPLE_VALUE_FLOAT(tag, val)         \
        static inline ag_value *tag(void)       \
        {                                       \
                return ag_value_new_float(val); \
        }


#define AG_SAMPLE_VALUE_STRING(tag, val)                        \
        static inline ag_value *tag(void)                       \
        {                                                       \
                AG_AUTO(ag_string) *s = ag_string_new(val);     \
                return ag_value_new_string(s);                  \
        }


#define AG_SAMPLE_VALUE_OBJECT(tag, type, val)          \
        static inline ag_value *tag(void)               \
        {                                               \
                AG_AUTO(type) *o = val;                 \
                return ag_value_new_object(val);        \
        }


#endif /* !__ARGENT_TEST_VALUE_H__ */

