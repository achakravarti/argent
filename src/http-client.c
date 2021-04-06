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


/*
 * Define the internal payload of an HTTP client object. The payload contains
 * the individual properties that define an HTTP client. All of the properties
 * are optional; 0 indicates the default port number, and each of the other
 * properties may be set to an empty string if not available.
 */
struct payload {
        ag_uint          port;          /* Port number of origin */
        ag_string       *ip;            /* IP address of origin  */
        ag_string       *host;          /* Hostname of origin    */
        ag_string       *agent;         /* User agent            */
        ag_string       *referer;       /* Referring URL         */
};


/*
 * Declare the prototype for the payload_new() helper function. This function is
 * responsible for creating the internal payload of a client object. It has the
 * same parameters as ag_http_client_new().
 */
static struct payload   *payload_new(const char *, ag_uint, const char *,
                            const char *, const char *);


/*
 * Define the ag_http_client object. The ag_http_client type is defined as an
 * object through the AG_OBJECT_DEFINE() macro, which helps to associate the
 * dynamic dispatch callback functions of the client object.
 */

AG_OBJECT_DEFINE(ag_http_client, AG_TYPEID_HTTP_CLIENT);

/*
 * Define the __ag_http_client_clone__() dynamic dispatch function. This function is called by
 * ag_object_clone() when ag_http_client_clone() is invoked. We return a deep
 * copy of the payload of a client object by creating a new payload instance
 * with the same properties as the source.
 */
AG_OBJECT_DEFINE_CLONE(ag_http_client,
        const struct payload *p = _p_;
        return payload_new(p->ip, p->port, p->host, p->agent, p->referer);
);


/*
 * Define the __ag_http_client_release__() dynamic dispatch function. This function is called
 * by ag_object_release() when ag_http_client_release() is invoked. We release
 * the dynamically allocated string fields of the payload structure.
 */
AG_OBJECT_DEFINE_RELEASE(ag_http_client,
        struct payload *p = _p_;
        ag_string_release(&p->ip);
        ag_string_release(&p->host);
        ag_string_release(&p->agent);
        ag_string_release(&p->referer);
);


/*
 * Define the __ag_http_client_cmp__() dynamic dispatch function. This function is called by
 * ag_object_cmp() when ag_http_client_cmp() is invoked. We perform a straight
 * lexicographical comparison between the string representations of both client
 * objects.
 */
AG_OBJECT_DEFINE_CMP(ag_http_client,
        AG_AUTO(ag_string) *s1 = ag_object_str(_o1_);
        AG_AUTO(ag_string) *s2 = ag_object_str(_o2_);

        return ag_string_cmp(s1, s2);
);


/*
 * Define the __ag_http_client_sz__() dynamic dispatch function. This function is called by
 * ag_object_sz() when ag_http_client_sz() is invoked. We consider the size of
 * a client object as the size of its string representation.
 */
AG_OBJECT_DEFINE_SZ(ag_http_client,
        AG_AUTO(ag_string) *s = ag_object_str(_o_);
        return ag_string_sz(s);
);


/*
 * Define the __ag_http_client_len__() dynamic dispatch function. This function is called by
 * ag_object_len() when ag_http_client_len() is invoked. We consider the length
 * of a client object as the length of its string representation.
 */
AG_OBJECT_DEFINE_LEN(ag_http_client,
        AG_AUTO(ag_string) *s = ag_object_str(_o_);
        return ag_string_len(s);
);


/*
 * Define the __ag_http_client_hash__() dynamic dispatch function. This function is called by
 * ag_object_hash() when ag_http_client_hash() is invoked. We consider the hash
 * of a client object as the hash of its string representation.
 */
AG_OBJECT_DEFINE_HASH(ag_http_client,
        AG_AUTO(ag_string) *s = ag_object_str(_o_);
        return ag_hash_new_str(s);
);


/*
 * Define the __ag_http_client_str__() dynamic dispatch function. This function is called by
 * ag_object_str() when ag_http_client_str() is invoked. The string
 * representation follows the format "[<ip>:<port>] host=<host>, agent=<agent>,
 * referer=<referer>". In case the port number is not specified, the port number
 * component does not appear in the string representation.
 */
AG_OBJECT_DEFINE_STR(ag_http_client,

        const struct payload *p = ag_object_payload(_o_);

        if (p->port)
                return ag_string_new_fmt(
                   "[%s:%lu] host=%s, agent=%s, referer=%s",
                    p->ip, p->port, p->host, p->agent, p->referer);
        else
                return ag_string_new_fmt("[%s] host=%s, agent=%s, referer=%s",
                    p->ip, p->host, p->agent, p->referer);
);




/*
 * Define the ag_http_client_new() interface function. This function creates a
 * new HTTP client object instance with all its properties defined. The IP
 * address of the client is passed through the first parameter, the port number
 * through the second parameter, the hostname through the third parameter, the
 * user agent through the fourth parameter, and the referring URL through the
 * last parameter. 
 *
 * In case no port number is to be specified (indicating the default port), then
 * 0 should be passed as the argument to the second parameter. All the other
 * parameters can safely accept empty strings.
 */
extern ag_http_client *
ag_http_client_new(const char *ip, ag_uint port, const char *host,
    const char *agent, const char *referer)
{
        AG_ASSERT_PTR (ip);
        AG_ASSERT_PTR (host);
        AG_ASSERT_PTR (agent);
        AG_ASSERT_PTR (referer);
        AG_ASSERT (port < 65535);

        return ag_object_new(AG_TYPEID_HTTP_CLIENT,
            payload_new(ip, port, host, agent, referer));
}


extern ag_http_client *
ag_http_client_parse_env(const struct ag_http_env *cgi)
{
        AG_ASSERT_PTR (cgi);

        ag_uint port = ag_uint_parse(cgi->remote_port);

        return ag_http_client_new(cgi->remote_addr, port, cgi->remote_host,
            cgi->http_user_agent, cgi->http_referer);
}


/*
 * Define the ag_http_client_ip() interface function. This function gets a
 * string representing the IP address from which the contextual HTTP client
 * makes the HTTP request.
 */
extern ag_string *
ag_http_client_ip(const ag_http_client *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ag_object_payload(ctx);
        return ag_string_copy(p->ip);
}


/*
 * Define the ag_http_client_port() interface function. This function gets the
 * port number through which the contextual HTTP client made the HTTP request.
 * In case the default port is used, then 0 is returned.
 */
extern ag_uint
ag_http_client_port(const ag_http_client *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ag_object_payload(ctx);
        return p->port;
}


/*
 * Define the ag_http_client_host() interface function. This function gets the
 * hostname of the machine from which the contextual HTTP client made the HTTP
 * request.
 */
extern ag_string *
ag_http_client_host(const ag_http_client *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ag_object_payload(ctx);
        return ag_string_copy(p->host);
}


/*
 * Define the ag_http_client_agent() interface function. This function gets the
 * user agent software through which the contextual HTTP client made the HTTP
 * request.
 */
extern ag_string *
ag_http_client_agent(const ag_http_client *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ag_object_payload(ctx);
        return ag_string_copy(p->agent);
}


/*
 * Define the ag_http_client_referer() interface function. This function gets
 * the referring URL from where the contextual HTTP client made the HTTP
 * request. Note that the misspelling 'referer' originates from the original RFC
 * request.
 *
 * TODO: return ag_http_url instead of string
 */
extern ag_string *
ag_http_client_referer(const ag_http_client *ctx)
{
        AG_ASSERT_PTR (ctx);

        const struct payload *p = ag_object_payload(ctx);
        return ag_string_copy(p->referer);
}


/*
 * Define the payload_new() helper function. This function creates an instance
 * of the internal payload of a client object with the properties of the client
 * specified through its parameters. The parameters are the same as that of the
 * ag_http_client_new() interface function, and have the same constraints.
 */
static struct payload *
payload_new(const char *ip, ag_uint port, const char *host, const char *agent,
    const char *referer)
{
        AG_ASSERT_PTR (ip);
        AG_ASSERT_PTR (host);
        AG_ASSERT_PTR (agent);
        AG_ASSERT_PTR (referer);
        AG_ASSERT (port < 65535);

        struct payload *p = ag_memblock_new(sizeof *p);

        p->port = port;
        p->ip = ag_string_new(ip);
        p->host = ag_string_new(host);
        p->agent = ag_string_new(agent);
        p->referer = ag_string_new(referer);

        return p;
}


