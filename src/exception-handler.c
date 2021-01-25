#include "../include/argent.h"

#include <regex.h>

        
extern void
ag_exception_memblock_hnd(const struct ag_exception *ex, void *opt)
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


extern void
ag_exception_regex_hnd(const struct ag_exception *ex, void *opt)
{
        (void)ex;

        struct ag_exception_regex *o = opt;
        const char *msg = "[!] string = %s, regex = %s, ecode = %d";

        printf(msg, o->str, o->regex, o->ecode);
        ag_log_err(msg, o->str, o->regex, o->ecode);

        switch (o->ecode) {
        case REG_BADBR:
                msg = "[!] there was an invalid `\\{…\\}` construct in the"
                    " regular expression";
                break;
        case REG_BADPAT:
                msg = "[!] there was a syntax error in the regular expression";
                break;
        case REG_BADRPT:
                msg = "[!] a repetition operator such as ‘?’ or ‘*’ appeared in"
                    " a bad position";
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

        
extern void
ag_exception_parse_hnd(const struct ag_exception *ex, void *opt)
{
        (void)ex;
        struct ag_exception_parse *o = opt;

        printf("[!] parsing context=%s, string=%s", o->ctx, o->str);
        ag_log_err("[!] parsing context=%s, string=%s", o->ctx, o->str);

        ag_exit(EXIT_FAILURE);
}


