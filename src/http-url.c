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


#include "../include/argent.h"


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
 * Declare the prototypes for the dynamic dispatch callbak functions for the
 * ag_http_url object. We are providing callback functions for all polymorphic
 * object functions.
 */
static ag_memblock      *virt_clone(const ag_memblock *);
static void              virt_release(ag_memblock *);
static enum ag_cmp       virt_cmp(const ag_object *, const ag_object *);
static bool              virt_valid(const ag_object *);
static size_t            virt_sz(const ag_object *);
static size_t            virt_len(const ag_object *);
static ag_hash           virt_hash(const ag_object *);
static ag_string        *virt_str(const ag_object *);


/*
 * Define the ag_http_url object. The ag_http_url type is defined as an object
 * by its dynamic dispatch callback functions that are registered with the
 * object registry.
 */
AG_OBJECT_DEFINE(ag_http_url)
{
        struct ag_object_vtable vt = {
                .clone = virt_clone, .release = virt_release, .cmp = virt_cmp,
                .valid = virt_valid, .sz = virt_sz,           .len = virt_len,
                .hash = virt_hash,   .str = virt_str,
        };

        ag_object_registry_set(AG_TYPEID_HTTP_URL, &vt);
}


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


/*
 * Define the virt_clone() dynamic dispatch callback function. This function
 * is called by ag_object_clone() when ag_http_url_clone() is invoked. We simply
 * return a payload instance with the same fields as that of the contextual
 * payload.
 */
static ag_memblock *
virt_clone(const ag_memblock *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ctx;
        return payload_new(p->secure, p->host, p->port, p->path);
}


/*
 * Define the virt_release() dynamic dispatch callback function. This function
 * is called by ag_object_release() when ag_http_url_release() is invoked. We
 * release the dynamically allocated string components of the payload.
 */
static void
virt_release(ag_memblock *ctx)
{
        AG_ASSERT_PTR (ctx);

        struct payload *p = ctx;
        ag_string_release(&p->host);
        ag_string_release(&p->path);
}


/*
 * Define the virt_cmp() dynamic dispatch callback function. This function is
 * called by ag_object_cmp() when ag_http_url_cmp() is invoked. We perform a
 * lexicographical comparison of the string representations of the HTTP URL
 * objects.
 */
static enum ag_cmp
virt_cmp(const ag_object *ctx, const ag_object *cmp)
{
        AG_ASSERT_PTR (ctx);
        AG_ASSERT_PTR (cmp);

        AG_AUTO(ag_string) *s = virt_str(ctx);
        AG_AUTO(ag_string) *s2 = virt_str(cmp);

        return ag_string_cmp(s, s2);
}


/*
 * Define the virt_valid() dynamic dispatch callback function. This function is
 * called by ag_object_valid() when ag_http_url_valid() is invoked. An HTTP URL
 * is guaranteed to be valid when constructed through ag_http_url_new().
 */
static bool
virt_valid(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        (void) ctx;
        return true;
}


/*
 * Define the virt_sz() dynamic dispatch callback function. This function is
 * called by ag_object_sz() when ag_http_url_sz() is invoked. The size of an
 * HTTP URL is the size of its string representation.
 */
static size_t
virt_sz(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        AG_AUTO(ag_string) *s = virt_str(ctx);
        return ag_string_sz(s);
}


/*
 * Define the virt_len() dynamic dispatch callback function. This function is
 * called by ag_object_len() when ag_http_url_len() is invoked. The length of an
 * HTTP URL is the length of its string representation.
 */
static size_t
virt_len(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        AG_AUTO(ag_string) *s = virt_str(ctx);
        return ag_string_len(s);
}


/*
 * Define the virt_hash() dynamic dispatch callback function. This function is
 * called by ag_object_hash() when ag_http_url_hash() is invoked. The hash of an
 * HTTP URL object is the hash of its string representation.
 */
static ag_hash
virt_hash(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        AG_AUTO(ag_string) *s = virt_str(ctx);
        return ag_hash_new_str(s);
}


/*
 * Define the virt_str() dynamic dispatch callback function. This function is
 * called by ag_object_str() when ag_http_url_str() is invoked.
 */
static ag_string *
virt_str(const ag_object *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ag_object_payload(ctx);

        if (p->port) {
                return ag_string_new_fmt("http%s://%s:%lu%s",
                    p->secure ? "s" : "", p->host, p->port, p->path);
        } else {
                return ag_string_new_fmt("http%s://%s%s",
                    p->secure ? "s" : "", p->host, p->path);
        }
}

