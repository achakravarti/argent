/*-
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * Argent - infrastructure for building web services
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


#include "./object.h"
#include "./http-url.h"
#include "./test.h"


#define __AG_TEST_SUITE_ID__ 8


#define SAMPLE(tag, secure, host, port, path)                           \
        static inline ag_http_url *tag(void)                            \
        {                                                               \
                return ag_http_url_new(secure, host, port, path);       \
        }                                                               \
        static inline size_t tag ## _LEN(void)                          \
        {                                                               \
                AG_AUTO(ag_http_url) *u = tag();                        \
                AG_AUTO(ag_string) *s = ag_http_url_str(u);             \
                return ag_string_len(s);                                \
        }                                                               \
        static inline size_t tag ## _SZ(void)                           \
        {                                                               \
                AG_AUTO(ag_http_url) *u = tag();                        \
                AG_AUTO(ag_string) *s = ag_http_url_str(u);             \
                return ag_string_sz(s);                                 \
        }                                                               \
        static inline size_t tag ## _HASH(void)                         \
        {                                                               \
                AG_AUTO(ag_http_url) *u = tag();                        \
                AG_AUTO(ag_string) *s = ag_http_url_str(u);             \
                return ag_hash_new_str(s);                              \
        }


SAMPLE(HTTP_LOCALHOST_8080_NOPATH, false, "127.0.0.1", 8080, "");
SAMPLE(HTTPS_LOCALHOST_8080_NOPATH, true, "127.0.0.1", 8080, "");
SAMPLE(HTTP_LOCALHOST_8080_FOO, false, "localhost", 8080, "foo");
SAMPLE(HTTPS_LOCALHOST_8080_FOO, true, "localhost", 8080, "foo");
SAMPLE(HTTPS_DOMAIN_FOO, true, "www.domain.com", 0, "/foo");
SAMPLE(HTTPS_DOMAIN_FOO_BAR, true, "www.domain.com", 0, "foo/bar");


AG_METATEST_OBJECT_COPY(ag_http_url, HTTP_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_COPY(ag_http_url, HTTPS_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_COPY(ag_http_url, HTTP_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_COPY(ag_http_url, HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_COPY(ag_http_url, HTTPS_DOMAIN_FOO());
AG_METATEST_OBJECT_COPY(ag_http_url, HTTPS_DOMAIN_FOO_BAR());

AG_METATEST_OBJECT_CLONE(ag_http_url, HTTP_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_CLONE(ag_http_url, HTTPS_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_CLONE(ag_http_url, HTTP_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_CLONE(ag_http_url, HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_CLONE(ag_http_url, HTTPS_DOMAIN_FOO());
AG_METATEST_OBJECT_CLONE(ag_http_url, HTTPS_DOMAIN_FOO_BAR());

AG_METATEST_OBJECT_RELEASE(ag_http_url, HTTP_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_RELEASE(ag_http_url, HTTPS_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_RELEASE(ag_http_url, HTTP_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_RELEASE(ag_http_url, HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_RELEASE(ag_http_url, HTTPS_DOMAIN_FOO());
AG_METATEST_OBJECT_RELEASE(ag_http_url, HTTPS_DOMAIN_FOO_BAR());


AG_METATEST_OBJECT_CMP(ag_http_url, HTTP_LOCALHOST_8080_NOPATH(),
    HTTPS_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_LT(ag_http_url, HTTP_LOCALHOST_8080_NOPATH(),
    HTTPS_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_EQ(ag_http_url, HTTP_LOCALHOST_8080_NOPATH(),
    HTTPS_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_GT(ag_http_url, HTTP_LOCALHOST_8080_NOPATH(),
    HTTPS_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_CMP(ag_http_url, HTTP_LOCALHOST_8080_FOO(),
    HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_LT(ag_http_url, HTTP_LOCALHOST_8080_FOO(),
    HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_EQ(ag_http_url, HTTP_LOCALHOST_8080_FOO(),
    HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_GT(ag_http_url, HTTP_LOCALHOST_8080_FOO(),
    HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_CMP(ag_http_url, HTTPS_DOMAIN_FOO(), HTTPS_DOMAIN_FOO_BAR());
AG_METATEST_OBJECT_LT(ag_http_url, HTTPS_DOMAIN_FOO(), HTTPS_DOMAIN_FOO_BAR());
AG_METATEST_OBJECT_EQ(ag_http_url, HTTPS_DOMAIN_FOO(), HTTPS_DOMAIN_FOO_BAR());
AG_METATEST_OBJECT_GT(ag_http_url, HTTPS_DOMAIN_FOO(), HTTPS_DOMAIN_FOO_BAR());


AG_METATEST_OBJECT_EMPTY_NOT(ag_http_url, HTTP_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_EMPTY_NOT(ag_http_url, HTTPS_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_EMPTY_NOT(ag_http_url, HTTP_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_EMPTY_NOT(ag_http_url, HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_EMPTY_NOT(ag_http_url, HTTPS_DOMAIN_FOO());
AG_METATEST_OBJECT_EMPTY_NOT(ag_http_url, HTTPS_DOMAIN_FOO_BAR());


AG_METATEST_OBJECT_VALID(ag_http_url, HTTP_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_VALID(ag_http_url, HTTPS_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_VALID(ag_http_url, HTTP_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_VALID(ag_http_url, HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_VALID(ag_http_url, HTTPS_DOMAIN_FOO());
AG_METATEST_OBJECT_VALID(ag_http_url, HTTPS_DOMAIN_FOO_BAR());


AG_METATEST_OBJECT_TYPEID(ag_http_url, HTTP_LOCALHOST_8080_NOPATH(),
    AG_TYPEID_HTTP_URL);
AG_METATEST_OBJECT_TYPEID(ag_http_url, HTTPS_LOCALHOST_8080_NOPATH(),
    AG_TYPEID_HTTP_URL);
AG_METATEST_OBJECT_TYPEID(ag_http_url, HTTP_LOCALHOST_8080_FOO(),
    AG_TYPEID_HTTP_URL);
AG_METATEST_OBJECT_TYPEID(ag_http_url, HTTPS_LOCALHOST_8080_FOO(),
    AG_TYPEID_HTTP_URL);
AG_METATEST_OBJECT_TYPEID(ag_http_url, HTTPS_DOMAIN_FOO(), AG_TYPEID_HTTP_URL);
AG_METATEST_OBJECT_TYPEID(ag_http_url, HTTPS_DOMAIN_FOO_BAR(),
    AG_TYPEID_HTTP_URL);


AG_METATEST_OBJECT_UUID(ag_http_url, HTTP_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_UUID(ag_http_url, HTTPS_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_UUID(ag_http_url, HTTP_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_UUID(ag_http_url, HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_UUID(ag_http_url, HTTPS_DOMAIN_FOO());
AG_METATEST_OBJECT_UUID(ag_http_url, HTTPS_DOMAIN_FOO_BAR());


AG_METATEST_OBJECT_REFC(ag_http_url, HTTP_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_REFC(ag_http_url, HTTPS_LOCALHOST_8080_NOPATH());
AG_METATEST_OBJECT_REFC(ag_http_url, HTTP_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_REFC(ag_http_url, HTTPS_LOCALHOST_8080_FOO());
AG_METATEST_OBJECT_REFC(ag_http_url, HTTPS_DOMAIN_FOO());
AG_METATEST_OBJECT_REFC(ag_http_url, HTTPS_DOMAIN_FOO_BAR());


AG_METATEST_OBJECT_LEN(ag_http_url, HTTP_LOCALHOST_8080_NOPATH(),
    HTTP_LOCALHOST_8080_NOPATH_LEN());
AG_METATEST_OBJECT_LEN(ag_http_url, HTTPS_LOCALHOST_8080_NOPATH(),
    HTTPS_LOCALHOST_8080_NOPATH_LEN());
AG_METATEST_OBJECT_LEN(ag_http_url, HTTP_LOCALHOST_8080_FOO(),
    HTTP_LOCALHOST_8080_FOO_LEN());
AG_METATEST_OBJECT_LEN(ag_http_url, HTTPS_LOCALHOST_8080_FOO(),
    HTTPS_LOCALHOST_8080_FOO_LEN());
AG_METATEST_OBJECT_LEN(ag_http_url, HTTPS_DOMAIN_FOO(), HTTPS_DOMAIN_FOO_LEN());
AG_METATEST_OBJECT_LEN(ag_http_url, HTTPS_DOMAIN_FOO_BAR(),
    HTTPS_DOMAIN_FOO_BAR_LEN());


AG_METATEST_OBJECT_SZ(ag_http_url, HTTP_LOCALHOST_8080_NOPATH(),
    HTTP_LOCALHOST_8080_NOPATH_SZ());
AG_METATEST_OBJECT_SZ(ag_http_url, HTTPS_LOCALHOST_8080_NOPATH(),
    HTTPS_LOCALHOST_8080_NOPATH_SZ());
AG_METATEST_OBJECT_SZ(ag_http_url, HTTP_LOCALHOST_8080_FOO(),
    HTTP_LOCALHOST_8080_FOO_SZ());
AG_METATEST_OBJECT_SZ(ag_http_url, HTTPS_LOCALHOST_8080_FOO(),
    HTTPS_LOCALHOST_8080_FOO_SZ());
AG_METATEST_OBJECT_SZ(ag_http_url, HTTPS_DOMAIN_FOO(), HTTPS_DOMAIN_FOO_SZ());
AG_METATEST_OBJECT_SZ(ag_http_url, HTTPS_DOMAIN_FOO_BAR(),
    HTTPS_DOMAIN_FOO_BAR_SZ());


AG_METATEST_OBJECT_HASH(ag_http_url, HTTP_LOCALHOST_8080_NOPATH(),
    HTTP_LOCALHOST_8080_NOPATH_HASH());
AG_METATEST_OBJECT_HASH(ag_http_url, HTTPS_LOCALHOST_8080_NOPATH(),
    HTTPS_LOCALHOST_8080_NOPATH_HASH());
AG_METATEST_OBJECT_HASH(ag_http_url, HTTP_LOCALHOST_8080_FOO(),
    HTTP_LOCALHOST_8080_FOO_HASH());
AG_METATEST_OBJECT_HASH(ag_http_url, HTTPS_LOCALHOST_8080_FOO(),
    HTTPS_LOCALHOST_8080_FOO_HASH());
AG_METATEST_OBJECT_HASH(ag_http_url, HTTPS_DOMAIN_FOO(),
    HTTPS_DOMAIN_FOO_HASH());
AG_METATEST_OBJECT_HASH(ag_http_url, HTTPS_DOMAIN_FOO_BAR(),
    HTTPS_DOMAIN_FOO_BAR_HASH());


AG_METATEST_OBJECT_STR(ag_http_url, HTTP_LOCALHOST_8080_NOPATH(),
    "http://127.0.0.1:8080/");
AG_METATEST_OBJECT_STR(ag_http_url, HTTPS_LOCALHOST_8080_NOPATH(),
    "https://127.0.0.1:8080/");
AG_METATEST_OBJECT_STR(ag_http_url, HTTP_LOCALHOST_8080_FOO(),
    "http://localhost:8080/foo");
AG_METATEST_OBJECT_STR(ag_http_url, HTTPS_LOCALHOST_8080_FOO(),
    "https://localhost:8080/foo");
AG_METATEST_OBJECT_STR(ag_http_url, HTTPS_DOMAIN_FOO(),
    "https://www.domain.com/foo");
AG_METATEST_OBJECT_STR(ag_http_url, HTTPS_DOMAIN_FOO_BAR(),
    "https://www.domain.com/foo/bar");


AG_METATEST_HTTP_URL_HOST(HTTP_LOCALHOST_8080_NOPATH(), "127.0.0.1");
AG_METATEST_HTTP_URL_HOST(HTTPS_LOCALHOST_8080_NOPATH(), "127.0.0.1");
AG_METATEST_HTTP_URL_HOST(HTTP_LOCALHOST_8080_FOO(), "localhost");
AG_METATEST_HTTP_URL_HOST(HTTPS_LOCALHOST_8080_FOO(), "localhost");
AG_METATEST_HTTP_URL_HOST(HTTPS_DOMAIN_FOO(), "www.domain.com");
AG_METATEST_HTTP_URL_HOST(HTTPS_DOMAIN_FOO_BAR(), "www.domain.com");


AG_METATEST_HTTP_URL_SECURE(HTTP_LOCALHOST_8080_NOPATH(), false);
AG_METATEST_HTTP_URL_SECURE(HTTPS_LOCALHOST_8080_NOPATH(), true);
AG_METATEST_HTTP_URL_SECURE(HTTP_LOCALHOST_8080_FOO(), false);
AG_METATEST_HTTP_URL_SECURE(HTTPS_LOCALHOST_8080_FOO(), true);
AG_METATEST_HTTP_URL_SECURE(HTTPS_DOMAIN_FOO(), true);
AG_METATEST_HTTP_URL_SECURE(HTTPS_DOMAIN_FOO_BAR(), true);


AG_METATEST_HTTP_URL_PORT(HTTP_LOCALHOST_8080_NOPATH(), 8080);
AG_METATEST_HTTP_URL_PORT(HTTPS_LOCALHOST_8080_NOPATH(), 8080);
AG_METATEST_HTTP_URL_PORT(HTTP_LOCALHOST_8080_FOO(), 8080);
AG_METATEST_HTTP_URL_PORT(HTTPS_LOCALHOST_8080_FOO(),8080);
AG_METATEST_HTTP_URL_PORT(HTTPS_DOMAIN_FOO(), 0);
AG_METATEST_HTTP_URL_PORT(HTTPS_DOMAIN_FOO_BAR(), 0);


AG_METATEST_HTTP_URL_PATH(HTTP_LOCALHOST_8080_NOPATH(), "/");
AG_METATEST_HTTP_URL_PATH(HTTPS_LOCALHOST_8080_NOPATH(), "/");
AG_METATEST_HTTP_URL_PATH(HTTP_LOCALHOST_8080_FOO(), "/foo");
AG_METATEST_HTTP_URL_PATH(HTTPS_LOCALHOST_8080_FOO(), "/foo");
AG_METATEST_HTTP_URL_PATH(HTTPS_DOMAIN_FOO(), "/foo");
AG_METATEST_HTTP_URL_PATH(HTTPS_DOMAIN_FOO_BAR(), "/foo/bar");


extern ag_test_suite *
test_suite_http_url(void)
{
        return AG_TEST_SUITE_GENERATE("ag_http_url interface");
}

