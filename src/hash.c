#include "./api.h"




// https://stackoverflow.com/questions/664014
extern ag_hash_t ag_hash_new(size_t key)
{
    key = (key ^ (key >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    key = (key ^ (key >> 27)) * UINT64_C(0x94d049bb133111eb);
    key = key ^ (key >> 31);
    return key;
}


// https://stackoverflow.com/questions/7666509
extern ag_hash_t ag_hash_new_str(const char *key)
{
    register ag_hash_t hash = 5381;
    register int c;

    while ((c = *key++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

