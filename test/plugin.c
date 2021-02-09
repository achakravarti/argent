#include "./object.h"
#include "./test.h"


#define __AG_TEST_SUITE_ID__ 13


static int sample_plugin_symbol_foo(void)
{
        return 123;
}


static inline ag_plugin *sample_local(void)
{
        return ag_plugin_new_local("sample_plugin_symbol_foo");
}


static inline ag_hash sample_hash(ag_plugin *hnd)
{
        AG_AUTO(ag_plugin) *p = hnd;
        AG_AUTO(ag_string) *s = ag_plugin_str(p);

        return ag_hash_new_str(s);
}


AG_METATEST_OBJECT_COPY(ag_plugin, sample_local());
AG_METATEST_OBJECT_CLONE(ag_plugin, sample_local());
AG_METATEST_OBJECT_RELEASE(ag_plugin, sample_local());

AG_METATEST_OBJECT_EMPTY_NOT(ag_plugin, sample_local());
AG_METATEST_OBJECT_VALID(ag_plugin, sample_local());

AG_METATEST_OBJECT_TYPEID(ag_plugin, sample_local(), AG_TYPEID_PLUGIN);
AG_METATEST_OBJECT_UUID(ag_plugin, sample_local());
AG_METATEST_OBJECT_REFC(ag_plugin, sample_local());
AG_METATEST_OBJECT_LEN(ag_plugin, sample_local(), 1);
AG_METATEST_OBJECT_HASH(ag_plugin, sample_local(), sample_hash(sample_local()));
AG_METATEST_OBJECT_STR(ag_plugin, sample_local(),
    "::sample_plugin_symbol_foo()");


extern ag_test_suite *
test_suite_plugin(void)
{

        return AG_TEST_SUITE_GENERATE("ag_plugin interface");
}

