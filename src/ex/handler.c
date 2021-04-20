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


#include "../../include/argent.h"

#include <regex.h>


/*******************************************************************************
 * `ag_exception_hnd_memblock()` is the exception handler that handles the
 * `AG_ERNO_MEMBLOCK` error. It prints and logs an error message indicating the
 * size of the memory block that could not be allocated, and optionally its
 * alignment.
 *
 * The exception metadata is passed through the first parameter, and the
 * exception details through the second parameter as a generic pointer that is
 * resolved to the `ag_exception_memblock` struct.
 *
 * See include/exception.h for the prototype declaration.
 */

extern void
ag_exception_hnd_memblock(const struct ag_exception *ex, void *opt)
{
        (void)ex;
        struct ag_exception_memblock *o = opt;

        if (o->align) {
                printf("[!] requested %lu bytes alignmed to %lu bytes\n",
                    o->sz, o->align);
                ag_log_err("requested %lu bytes aligned to %lu bytes",
                    o->sz, o->align);
        } else {
                printf("[!] requested %lu bytes\n", o->sz);
                ag_log_err("requested %lu bytes", o->sz);
        }

        ag_exit(EXIT_FAILURE);
}


/*******************************************************************************
 * `ag_exception_hnd_regex()` is the exception handler that handles the
 * `AG_ERNO_REGEX` error. It prints and logs an error message indicating the
 * regular expression, the string being matched, and a diagnostic explanation.
 *
 * The exception metadata is passed through the first parameter, and the
 * exception details through the second parameter as a generic pointer that is
 * resolved to an `ag_exception_regex` struct.
 *
 * See include/exception.h for the prototype declaration.
 */

extern void
ag_exception_hnd_regex(const struct ag_exception *ex, void *opt)
{
        (void)ex;

        struct ag_exception_regex *o = opt;
        const char *msg = "[!] string = %s, regex = %s, ecode = %d";

        printf(msg, o->str, o->regex, o->ecode);
        ag_log_err(msg, o->str, o->regex, o->ecode);

        switch (o->ecode) {
        case REG_BADBR:
                msg = "[!] there was an invalid `\\{...\\}` construct in the"
                    " regular expression";
                break;
        case REG_BADPAT:
                msg = "[!] there was a syntax error in the regular expression";
                break;
        case REG_BADRPT:
                msg = "[!] a repetition operator such as ‘?’ or ‘*’"
                    " appeared in a bad position";
                break;
        case REG_ECOLLATE:
                msg = "[!] the regular expression referred to an invalid"
                    " collating element";
                break;
        case REG_ECTYPE:
                msg = "[!] the regular expression referred to an invalid"
                    " character class name";
                break;
        case REG_EESCAPE:
                msg = "[!] the regular expression ended with `\\’";
                break;
        case REG_ESUBREG:
                msg = "[!] there was an invalid number in the ‘\\digit’"
                    " construct";
                break;
        case REG_EBRACK:
                msg = "[!] there were unbalanced square brackets in the regular"
                    " expression";
                break;
        case REG_EPAREN:
                msg = "[!] an extended regular expression had unbalanced"
                    " parentheses, or a basic regular expression had unbalanced"
                    " ‘\\(’ and ‘\\)’";
                break;
        case REG_EBRACE:
                msg = "[!] the regular expression had unbalanced ‘\\{’ and"
                    " ‘\\}’";
                break;
        case REG_ERANGE:
                msg = "[!] one of the endpoints in a range expression was"
                    " invalid";
                break;
        case REG_ESPACE:
        default:
                msg = "[!] regcomp ran out of memory";
        }

        printf("\n%s\n", msg);
        ag_log_err(msg);

        ag_exit(EXIT_FAILURE);
}


/*******************************************************************************
 * `ag_exception_hnd_parse()` is the exception handler that handles the
 * `AG_ERNO_PARSE` error. It prints and logs the error message, indicating both
 * the context and the string that failed to be parsed.
 *
 * The exception metadata is received through the first parameter, and the parse
 * error details are received through the second parameter as a generic pointer
 * that is resolved to the `ag_exception_parse` struct.
 *
 * See include/exception.h for the prototype declaration.
 */

extern void
ag_exception_hnd_parse(const struct ag_exception *ex, void *opt)
{
        (void)ex;
        struct ag_exception_parse *o = opt;

        printf("[!] parsing context=%s, string=%s\n", o->ctx, o->str);
        ag_log_err("parsing context=%s, string=%s", o->ctx, o->str);

        ag_exit(EXIT_FAILURE);
}

