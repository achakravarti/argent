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


#include "../argent.h"


/*
 * Define the payload for an HTTP URL object. The payload contains the
 * individual components of an HTTP URL, of which only the port number is
 * optional.
 */
struct payload {
        bool             secure;        /* HTTPS used?              */
        ag_uint          port;          /* Port number, 0 if none   */
        ag_string       *host;          /* Host name                */
        ag_string       *path;          /* Path to resource on host */
};


/*
 * Declare the prototype for the payload_new() helper function. This function
 * creates a new payload instance for an ag_http_url object.
 */
static struct payload   *payload_new(bool, const char *, ag_uint, const char *);


/*
 * Define the ag_http_url object. The ag_http_url type is defined as an object
 * by its dynamic dispatch callback functions that are registered with the
 * object registry.
 */
AG_OBJECT_DEFINE(ag_http_url, AG_TYPEID_HTTP_URL);

/*
 * Define the __ag_http_url_clone__() dynamic dispatch callback function. This function
 * is called by ag_object_clone() when ag_http_url_clone() is invoked. We simply
 * return a payload instance with the same fields as that of the contextual
 * payload.
 */
AG_OBJECT_DEFINE_CLONE(ag_http_url,
        const struct payload *p = _p_;
        return payload_new(p->secure, p->host, p->port, p->path);
);


/*
 * Define the __ag_http_url_release__() dynamic dispatch callback function. This function
 * is called by ag_object_release() when ag_http_url_release() is invoked. We
 * release the dynamically allocated string components of the payload.
 */
AG_OBJECT_DEFINE_RELEASE(ag_http_url,
        struct payload *p = _p_;
        ag_string_release(&p->host);
        ag_string_release(&p->path);
);


/*
 * Define the __ag_http_url_cmp__() dynamic dispatch callback function. This function is
 * called by ag_object_cmp() when ag_http_url_cmp() is invoked. We perform a
 * lexicographical comparison of the string representations of the HTTP URL
 * objects.
 */
AG_OBJECT_DEFINE_CMP(ag_http_url,
        AG_AUTO(ag_string) *s1 = ag_object_str(_o1_);
        AG_AUTO(ag_string) *s2 = ag_object_str(_o2_);

        return ag_string_cmp(s1, s2);
);


/*
 * Define the __ag_http_url_sz__() dynamic dispatch callback function. This function is
 * called by ag_object_sz() when ag_http_url_sz() is invoked. The size of an
 * HTTP URL is the size of its string representation.
 */
AG_OBJECT_DEFINE_SZ(ag_http_url,
        AG_AUTO(ag_string) *s = ag_object_str(_o_);
        return ag_string_sz(s);
);


/*
 * Define the __ag_http_url_len__() dynamic dispatch callback function. This function is
 * called by ag_object_len() when ag_http_url_len() is invoked. The length of an
 * HTTP URL is the length of its string representation.
 */
AG_OBJECT_DEFINE_LEN(ag_http_url,
        AG_AUTO(ag_string) *s = ag_object_str(_o_);
        return ag_string_len(s);
);


/*
 * Define the __ag_http_url_hash__() dynamic dispatch callback function. This function is
 * called by ag_object_hash() when ag_http_url_hash() is invoked. The hash of an
 * HTTP URL object is the hash of its string representation.
 */
AG_OBJECT_DEFINE_HASH(ag_http_url,
        AG_AUTO(ag_string) *s = ag_object_str(_o_);
        return ag_hash_new_str(s);
);


/*
 * Define the __ag_http_url_str__() dynamic dispatch callback function. This function is
 * called by ag_object_str() when ag_http_url_str() is invoked.
 */
AG_OBJECT_DEFINE_STR(ag_http_url,
        const struct payload *p = ag_object_payload(_o_);

        if (p->port)
                return ag_string_new_fmt("http%s://%s:%lu%s",
                    p->secure ? "s" : "", p->host, p->port, p->path);
        else
                return ag_string_new_fmt("http%s://%s%s",
                    p->secure ? "s" : "", p->host, p->path);
);



/*
 * Define the ag_http_url_new() interface function. This function creates a new
 * HTTP URL instance defined by ihe parameters. The first parameter indicates
 * whether or not the HTTP URL instance uses the HTTPS protocol, the second
 * instance indicates the hostname, the third parameter indicates the port
 * number, and the fourth parameter indicates the resource path. The port number
 * must be valid, and the path may be an empty string.
 */
extern ag_http_url *
ag_http_url_new(bool secure, const char *host, ag_uint port,
    const char *path)
{
        AG_ASSERT_STR (host);
        AG_ASSERT_PTR (path);
        AG_ASSERT (port && port < 65535);

        return ag_object_new(AG_TYPEID_HTTP_URL,
            payload_new(secure, host, port, path));
}


/*
 * Define the ag_http_url_new_noport() function. This function is similar to the
 * ag_http_url_new() function, except that it allows for the creation a new URL
 * object without specifying a port number.
 */
extern ag_http_url *
ag_http_url_new_noport(bool secure, const char *host, const char *path)
{
        AG_ASSERT_STR (host);
        AG_ASSERT_PTR (path);

        return ag_object_new(AG_TYPEID_HTTP_URL,
            payload_new(secure, host, 0, path));
}


/*
 * Define the ag_http_url_parse() interface function. This function parses a
 * given string passed as the argument to the parameter of this function, and
 * returns the ag_http_url object instance represented by that string. In case
 * the string does not represent a parsable URL object, then an exception is
 * thrown.
 *
 * We use the sscanf() function to parse the string, taking into account that
 * there are eight different valid string forms:
 *   1. HTTPS, host/IP, port, path
 *   2. HTTP, host/IP, port, path
 *   3. HTTPS, host/IP, path
 *   4. HTTP, host/IP, path
 *   5. HTTPS, host/IP, port
 *   6. HTTP, host/IP, port
 *   7. HTTPS, host/IP
 *   8. HTTP, host/IP.
 *
 * The const format string variables reflect these forms, with their names
 * identifying whether they are (s)ecure, whether they have a (po)rt, and
 * whether they have a (pa)th. In all cases, the hostname/IP is present.
 */
extern ag_http_url *
ag_http_url_parse(const char *src)
{
        AG_ASSERT_STR (src);

        const char *fmt = "http://%263[^\n]";
        const char *fmt_s = "https://%263[^\n]";
        const char *fmt_po = "http://%263[^:]:%lu[^\n]";
        const char *fmt_spo = "https://%263[^:]:%lu[^\n]";
        const char *fmt_pa = "http://%263[^/]/%2047[^\n]";
        const char *fmt_spa = "https://%263[^/]/%2047[^\n]";
        const char *fmt_popa = "http://%263[^:]:%lu/%2047[^\n]";
        const char *fmt_spopa = "https://%263[^:]:%lu/%2047[^\n]";

        AG_AUTO(ag_string) *s = ag_string_new(src);
        char host[264];
        char path[2048];
        ag_uint port;

        if (sscanf(s, fmt_spopa, host, &port, path) == 3)
                return ag_http_url_new(true, host, port, path);
        else if (sscanf(s, fmt_popa, host, &port, path) == 3)
                return ag_http_url_new(false, host, port, path);
        else if (sscanf(s, fmt_spa, host, path) == 2)
                return ag_http_url_new_noport(true, host, path);
        else if (sscanf(s, fmt_pa, host, path) == 2)
                return ag_http_url_new_noport(false, host, path);
        else if (sscanf(s, fmt_spo, host, &port) == 2)
                return ag_http_url_new(true, host, port, "/");
        else if (sscanf(s, fmt_po, host, &port) == 2)
                return ag_http_url_new(false, host, port, "/");
        else if (sscanf(s, fmt_s, host) == 1)
                return ag_http_url_new_noport(true, host, "/");
        else if (sscanf(s, fmt, host) == 1)
                return ag_http_url_new_noport(false, host, "/");

        struct ag_exception_parse x = {.str = src, .ctx = "ag_http_url"};
        AG_REQUIRE_OPT (false, AG_ERNO_PARSE, &x);
        return NULL;
}


extern ag_http_url *
ag_http_url_parse_env(const struct ag_http_env *cgi)
{
        AG_ASSERT_PTR (cgi);

        AG_AUTO(ag_string) *s = ag_string_new(cgi->https);
        AG_AUTO(ag_string) *s2 = ag_string_lower(s);
        bool secure = ag_string_eq(s2, "on");

        AG_AUTO(ag_string) *p = ag_string_new(cgi->request_uri);
        AG_AUTO(ag_string) *p2 = ag_string_split(p, "?");
        AG_AUTO(ag_string) *path = ag_string_split(p2, "#");

        ag_uint port = ag_uint_parse(cgi->server_port);

        return port ? ag_http_url_new(secure, cgi->server_name, port, path)
            : ag_http_url_new_noport(secure, cgi->server_name, path);
}


/*
 * Define the ag_http_url_secure() interface function. This function returns
 * true if an HTTP URL object is using the HTTPS protocol, and false if it is
 * using the HTTP protocol.
 */
extern bool
ag_http_url_secure(const ag_http_url *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ag_object_payload(ctx);
        return p->secure;
}


/*
 * Define the ag_http_url_host() interface function. We return the host name
 * component of an HTTP URL object.
 */
extern ag_string *
ag_http_url_host(const ag_http_url *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ag_object_payload(ctx);
        return ag_string_copy(p->host);
}


/*
 * Define the ag_http_url_port() interface function. We return the port number
 * component of an HTTP URL object.
 */
extern ag_uint
ag_http_url_port(const ag_http_url *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ag_object_payload(ctx);
        return p->port;
}


/*
 * Define the ag_http_url_path() interface function. We return the path
 * component of an HTTP URL object.
 */
extern ag_string *
ag_http_url_path(const ag_http_url *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ag_object_payload(ctx);
        return ag_string_copy(p->path);
}


/*
 * Define the payload_new() helper function. This function is responsible for
 * creating a new payload instance, and has the same parameters as the
 * ag_http_url_new() interface function. Setting the port number to 0 indicates
 * that the default port should be used. This function guarantees that the path
 * name starts at the root (/), even if that is not specified in the argument to
 * the path parameter.
 */
static struct payload *
payload_new(bool secure, const char *host, ag_uint port, const char *path)
{
        AG_ASSERT_STR (host);
        AG_ASSERT_PTR (path);
        AG_ASSERT (port < 65535);

        struct payload *p = ag_memblock_new(sizeof *p);

        p->secure = secure;
        p->port = port;
        p->host = ag_string_new(host);

        char c = *path;
        if (c) {
                p->path = c == '/'
                    ? ag_string_new(path) : ag_string_new_fmt("/%s", path);
        } else
                p->path = ag_string_new("/");

        return p;
}

