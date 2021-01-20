#ifndef __ARGENT_TYPEID_H__
#define __ARGENT_TYPEID_H__

#ifdef __cplusplus
extern "C" {
#endif


typedef int ag_typeid;


#define AG_TYPEID_OBJECT        ((ag_typeid)  0)
#define AG_TYPEID_FIELD         ((ag_typeid) -1)
#define AG_TYPEID_LIST          ((ag_typeid) -2)
#define AG_TYPEID_ALIST         ((ag_typeid) -3)
#define AG_TYPEID_HTTP_URL      ((ag_typeid) -4)
#define AG_TYPEID_HTTP_CLIENT   ((ag_typeid) -5)


#ifdef __cplusplus
}
#endif

#endif /* __ARGENT_TYPEID_H__ */

