#include "../include/argent.h"

#include <stdint.h>


extern ag_hash ag_hash_new(size_t key)
{
        key = (key ^ (key >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
        key = (key ^ (key >> 27)) * UINT64_C(0x94d049bb133111eb);
        key = key ^ (key >> 31);
    
        return key;
}


extern ag_hash ag_hash_new_str(const char *key)
{
        AG_ASSERT_PTR (key);

        register ag_hash hash = 5381;
        register int c;

        while ((c = *key++))
                hash = ((hash << 5) + hash) + c;

        return hash;
}

