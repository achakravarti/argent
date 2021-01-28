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


#include "./http-client.h"
#include "./object.h"
#include "./test.h"


/*
 * Define the __AG_TEST_SUITE_ID__ symbolic constant. This constant represents
 * the unique ID of the HTTP client interface test suite, and is required by the
 * testing interface to metaprogrammatically generate the relevant testing
 * functions.
 */
#define __AG_TEST_SUITE_ID__ 9


/*
 * Define the SAMPLE() helper macro. This macro metaprogrammatically generates
 * the support functions required for testing a sample HTTP client object. The
 * first parameter specifies the unique tag to identify a given sample, and the
 * remaining parameters are the corresponding properties of the client.
 *
 * For a given tag <TAG>, the SAMPLE() macro generates the following inline
 * support functions:
 *   - TAG()    : returns the sample HTTP client object.
 *   - TAG_LEN(): returns the length of the sample client.
 *   - TAG_SZ() : returns the size of the sample client.
 *   - TAG_HASH : returns the hash of the sample client.
 */
#define SAMPLE(tag, ip, port, host, agent, referer)                            \
        static inline ag_http_client *tag(void)                                \
        {                                                                      \
                return ag_http_client_new(ip, port, host, agent, referer);     \
        }                                                                      \
        static inline size_t tag ## _LEN(void)                                 \
        {                                                                      \
                AG_AUTO(ag_http_client) *c = tag();                            \
                AG_AUTO(ag_string) *s = ag_http_client_str(c);                 \
                return ag_string_len(s);                                       \
        }                                                                      \
        static inline size_t tag ## _SZ(void)                                  \
        {                                                                      \
                AG_AUTO(ag_http_client) *c = tag();                            \
                AG_AUTO(ag_string) *s = ag_http_client_str(c);                 \
                return ag_string_sz(s);                                        \
        }                                                                      \
        static inline size_t tag ## _HASH(void)                                \
        {                                                                      \
                AG_AUTO(ag_http_client) *c = tag();                            \
                AG_AUTO(ag_string) *s = ag_http_client_str(c);                 \
                return ag_hash_new_str(s);                                     \
        }


/*
 * Define the sample client objects to be used for testing. We use the SAMPLE()
 * helper macro defined above to generate the test client objects with different
 * properties.
 */
SAMPLE(CLIENT0, "", 0, "", "", "");
SAMPLE(CLIENT1, "192.168.0.1", 0, "host.com", "mozilla", "google.com");
SAMPLE(CLIENT2, "192.168.1.1", 40, "domain.com", "webkit", "");


/*
 * Run the ag_object_copy() metatest for ag_http_client_copy() with the above
 * sample HTTP client objects.
 */
AG_METATEST_OBJECT_COPY(ag_http_client, CLIENT0());
AG_METATEST_OBJECT_COPY(ag_http_client, CLIENT1());
AG_METATEST_OBJECT_COPY(ag_http_client, CLIENT2());


/*
 * Run the ag_object_clone() metatest for ag_http_client_clone() with the above
 * sample HTTP client objects.
 */
AG_METATEST_OBJECT_CLONE(ag_http_client, CLIENT0());
AG_METATEST_OBJECT_CLONE(ag_http_client, CLIENT1());
AG_METATEST_OBJECT_CLONE(ag_http_client, CLIENT2());


/*
 * Run the ag_object_release() metatest for ag_http_client_release() with the
 * above sample HTTP client objects.
 */
AG_METATEST_OBJECT_RELEASE(ag_http_client, CLIENT0());
AG_METATEST_OBJECT_RELEASE(ag_http_client, CLIENT1());
AG_METATEST_OBJECT_RELEASE(ag_http_client, CLIENT2());


/*
 * Run the ag_object_cmp() metatest for ag_http_client_cmp() with the above
 * sample HTTP client objects. The client object passed through the first
 * parameter is lexicographically smaller than the second.
 */
AG_METATEST_OBJECT_CMP(ag_http_client, CLIENT1(), CLIENT0());
AG_METATEST_OBJECT_CMP(ag_http_client, CLIENT2(), CLIENT0());
AG_METATEST_OBJECT_CMP(ag_http_client, CLIENT1(), CLIENT2());


/*
 * Run the ag_object_lt() metatest for ag_http_client_lt() with the above sample
 * HTTP client objects. The client object passed through the first parameter is
 * lexicographically smaller than the second.
 */
AG_METATEST_OBJECT_LT(ag_http_client, CLIENT1(), CLIENT0());
AG_METATEST_OBJECT_LT(ag_http_client, CLIENT2(), CLIENT0());
AG_METATEST_OBJECT_LT(ag_http_client, CLIENT1(), CLIENT2());


/*
 * Run the ag_object_eq() metatest for ag_http_client_eq() with the above sample
 * HTTP client objects. The client object passed through the first parameter is
 * lexicographically smaller than the second.
 */
AG_METATEST_OBJECT_EQ(ag_http_client, CLIENT1(), CLIENT0());
AG_METATEST_OBJECT_EQ(ag_http_client, CLIENT2(), CLIENT0());
AG_METATEST_OBJECT_EQ(ag_http_client, CLIENT1(), CLIENT2());


/*
 * Run the ag_object_gt() metatest for ag_http_client_gt() with the above sample
 * HTTP client objects. The client object passed through the first parameter is
 * lexicographically smaller than the second.
 */
AG_METATEST_OBJECT_GT(ag_http_client, CLIENT1(), CLIENT0());
AG_METATEST_OBJECT_GT(ag_http_client, CLIENT2(), CLIENT0());
AG_METATEST_OBJECT_GT(ag_http_client, CLIENT1(), CLIENT2());


/*
 * Run the ag_object_empty() metatest for ag_http_client_empty() with the sample
 * client objects. Since client objects can never be empty, we only run the
 * AG_METATEST_OBJECT_EMPTY_NOT() metatest.
 */
AG_METATEST_OBJECT_EMPTY_NOT(ag_http_client, CLIENT0());
AG_METATEST_OBJECT_EMPTY_NOT(ag_http_client, CLIENT1());
AG_METATEST_OBJECT_EMPTY_NOT(ag_http_client, CLIENT2());


/*
 * Run the ag_object_valid() metatest for ag_http_client_valid() with the above
 * sample client objects. Since client objects are always valid, we only run the
 * AG_METATEST_OBJECT_VALID() metatest.
 */
AG_METATEST_OBJECT_VALID(ag_http_client, CLIENT0());
AG_METATEST_OBJECT_VALID(ag_http_client, CLIENT1());
AG_METATEST_OBJECT_VALID(ag_http_client, CLIENT2());


/*
 * Run the ag_object_typeid() metatest for ag_http_client_typeid() with the
 * sample client objects.
 */
AG_METATEST_OBJECT_TYPEID(ag_http_client, CLIENT0(), AG_TYPEID_HTTP_CLIENT);
AG_METATEST_OBJECT_TYPEID(ag_http_client, CLIENT1(), AG_TYPEID_HTTP_CLIENT);
AG_METATEST_OBJECT_TYPEID(ag_http_client, CLIENT2(), AG_TYPEID_HTTP_CLIENT);


/*
 * Run the ag_object_uuid() metatest for ag_http_client_uuid() with the sample
 * client objects.
 */
AG_METATEST_OBJECT_UUID(ag_http_client, CLIENT0());
AG_METATEST_OBJECT_UUID(ag_http_client, CLIENT1());
AG_METATEST_OBJECT_UUID(ag_http_client, CLIENT2());


/*
 * Run the ag_object_refc() metatest for ag_http_client_refc() with the sample
 * client objects.
 */
AG_METATEST_OBJECT_REFC(ag_http_client, CLIENT0());
AG_METATEST_OBJECT_REFC(ag_http_client, CLIENT1());
AG_METATEST_OBJECT_REFC(ag_http_client, CLIENT2());


/*
 * Run the ag_object_len() metatest for ag_http_client_len() with the sample
 * client objects.
 */
AG_METATEST_OBJECT_LEN(ag_http_client, CLIENT1(), CLIENT1_LEN());
AG_METATEST_OBJECT_LEN(ag_http_client, CLIENT2(), CLIENT2_LEN());


/*
 * Run the ag_object_sz() metatest for ag_http_client_sz() with the sample
 * client objects.
 */
AG_METATEST_OBJECT_SZ(ag_http_client, CLIENT1(), CLIENT1_SZ());
AG_METATEST_OBJECT_SZ(ag_http_client, CLIENT2(), CLIENT2_SZ());


/*
 * Run the ag_object_hash() metatest for ag_http_client_hash() with the sample
 * client objects.
 */
AG_METATEST_OBJECT_HASH(ag_http_client, CLIENT0(), CLIENT0_HASH());
AG_METATEST_OBJECT_HASH(ag_http_client, CLIENT1(), CLIENT1_HASH());
AG_METATEST_OBJECT_HASH(ag_http_client, CLIENT2(), CLIENT2_HASH());


/*
 * Run the ag_object_str() metatest for ag_http_client_str() with the sample
 * client objects.
 */
AG_METATEST_OBJECT_STR(ag_http_client, CLIENT0(),
    "[] host=, agent=, referer=");
AG_METATEST_OBJECT_STR(ag_http_client, CLIENT1(),
    "[192.168.0.1] host=host.com, agent=mozilla, referer=google.com");
AG_METATEST_OBJECT_STR(ag_http_client, CLIENT2(),
    "[192.168.1.1:40] host=domain.com, agent=webkit, referer=");


/*
 * Run the metatest for ag_http_client_ip() with the above sample client
 * objects. The first parameter is the sample client object, and the second
 * parameter is the expected string result.
 */
AG_METATEST_HTTP_CLIENT_IP(CLIENT0(), "");
AG_METATEST_HTTP_CLIENT_IP(CLIENT1(), "192.168.0.1");
AG_METATEST_HTTP_CLIENT_IP(CLIENT2(), "192.168.1.1");


/*
 * Run the metatest for ag_http_client_port() with the sample client objects.
 * The first parameter is the sample client object, and the second parameter is
 * the expected unsigned integer result.
 */
AG_METATEST_HTTP_CLIENT_PORT(CLIENT0(), 0);
AG_METATEST_HTTP_CLIENT_PORT(CLIENT1(), 0);
AG_METATEST_HTTP_CLIENT_PORT(CLIENT2(), 40);


/*
 * Run the metatest for ag_http_client_host() with the sample client objects.
 * The first parameter is the sample client object, and the second parameter is
 * the expected string result.
 */
AG_METATEST_HTTP_CLIENT_HOST(CLIENT0(), "");
AG_METATEST_HTTP_CLIENT_HOST(CLIENT1(), "host.com");
AG_METATEST_HTTP_CLIENT_HOST(CLIENT2(), "domain.com");


/*
 * Run the metatest for ag_http_client_agent() with the sample client objects.
 * The first parameter is the sample client object, and the second parameter is
 * the expected string result.
 */
AG_METATEST_HTTP_CLIENT_AGENT(CLIENT0(), "");
AG_METATEST_HTTP_CLIENT_AGENT(CLIENT1(), "mozilla");
AG_METATEST_HTTP_CLIENT_AGENT(CLIENT2(), "webkit");


/*
 * Run the metatest for ag_http_client_referer() with the sample client objects.
 * The first parameter is the sample client object, and the second parameter is
 * the expected string result.
 */
AG_METATEST_HTTP_CLIENT_REFERER(CLIENT0(), "");
AG_METATEST_HTTP_CLIENT_REFERER(CLIENT1(), "google.com");
AG_METATEST_HTTP_CLIENT_REFERER(CLIENT2(), "");


/*
 * Define the test_suite_http_client() function. We generate the test cases from
 * the above metatest definitions through a call to AG_TEST_SUITE_GENERATE().
 */
extern ag_test_suite *
test_suite_http_client(void)
{
        return AG_TEST_SUITE_GENERATE("ag_http_client interface");
}

