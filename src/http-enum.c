#include "../include/argent.h"


/*
 * Define the g_method string array. This array holds the string representations
 * of the enumerators of the ag_http_method enumeration. There is a one-to-one
 * correspondence between the enumerators and their string representations, with
 * the array member index being the same as the corresponding enumerator.
 */
static AG_THREADLOCAL const char *g_method[] = {
        "GET",
        "POST",
        "PUT",
        "PATCH",
        "DELETE",
};


/*
 * Define the ag_http_method_parse() interface function. This function is
 * responsible for parsing a given string and returning the HTTP method
 * enumerator represented by the string. In case the string contains something
 * not recognised as a valid enumerator, then AG_HTTP_METHOD_GET is returned.
 *
 * TODO: explore the possiblity of raising a runtime exception.
 */
extern enum ag_http_method
ag_http_method_parse(const char *str)
{
        AG_ASSERT_STR (str);

        AG_AUTO(ag_string) *s = ag_string_new(str);
        AG_AUTO(ag_string) *u = ag_string_upper(s);

        for (register int i = 0; i <= AG_HTTP_METHOD_DELETE; i++)
                if (ag_string_eq(u, g_method[i]))
                        return i;

        return AG_HTTP_METHOD_GET;
}


/*
 * Defien the ag_http_method_str() interface function. This function gets the
 * string representation of a contextual HTTP method enumerator.
 */
extern ag_string *
ag_http_method_str(enum ag_http_method meth)
{
        AG_ASSERT (meth >= AG_HTTP_METHOD_GET && meth <= AG_HTTP_METHOD_DELETE);

        return ag_string_new(g_method[meth]);
}


extern enum ag_http_mime
ag_http_mime_parse(const char *str)
{
}


extern ag_string *
ag_http_mime_str(enum ag_http_mime mime)
{
}

