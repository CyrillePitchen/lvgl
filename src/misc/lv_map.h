/**
 * @file lv_map.h
 * Map. The map entries are dynamically allocated by the 'lv_mem' module.
 */

#ifndef LV_MAP_H
#define LV_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lv_types.h"

/*********************
 *      DEFINES
 *********************/
#define map_for_each(_map, _it)       \
    for(_it = lv_map_begin(_map);     \
        _it != lv_map_end(_map);      \
        _it = lv_map_entry_next(_it))

#define map_for_each_safe(_map, _it, _next)			  \
    for(_it = lv_map_begin(_map), _next = lv_map_entry_next(_it); \
        _it != lv_map_end(_map);                                  \
        _it = _next)

#define map_for_each_reverse(_map, _it) \
    for(_it = lv_map_rbegin(_map);      \
        _it != lv_map_rend(_map);	\
        _it = lv_map_entry_prev(_it))

#define map_for_each_reverse_safe(_map, _it, _prev)                     \
    for(_it = lv_map_rbegin(_map), _prev = lv_map_entry_prev(_it);      \
        _it != lv_map_rend(_map);                                       \
        _it = _prev)

/**********************
 *      TYPEDEFS
 **********************/
typedef struct _lv_link_t {
    struct _lv_link_t * prev;
    struct _lv_link_t * next;
} lv_link_t;

typedef void lv_map_key_t;
typedef void lv_map_value_t;

typedef struct _lv_map_class_t {
    uint32_t key_size;
    uint32_t value_size;
    void * (*malloc_cb)(size_t size);
    void (*free_cb)(void * data);
    int (*key_comp_cb)(const lv_map_key_t * key1, const lv_map_key_t * key2);
    void (*key_copy_cb)(lv_map_key_t * dst_key, const lv_map_key_t * src_key);
    void (*key_destructor_cb)(lv_map_key_t * key);
    void (*value_copy_cb)(lv_map_value_t * dst_value, const lv_map_value_t * src_value);
    void (*value_destructor_cb)(lv_map_value_t * value);
} lv_map_class_t;

struct _lv_map_entry_t;
typedef struct _lv_map_entry_t lv_map_entry_t;

typedef struct _lv_map_t {
    const lv_map_class_t * class_p;
    lv_map_entry_t * root;
    lv_link_t end;
} lv_map_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/
uint32_t lv_map_get_entry_size(const lv_map_class_t * class_p);
lv_map_t * lv_map_create(const lv_map_class_t * class_p);
void lv_map_delete(lv_map_t * map);
void lv_map_init(lv_map_t * map, const lv_map_class_t * class_p);
void lv_map_clear(lv_map_t * map);
bool lv_map_insert(lv_map_t * map, const lv_map_key_t * key, const lv_map_value_t * value, lv_map_entry_t ** out);
bool lv_map_erase(lv_map_t * map, const lv_map_key_t * key);
void lv_map_remove(lv_map_entry_t * entry);
bool lv_map_set(lv_map_t * map, const lv_map_key_t * key, const lv_map_value_t * value, lv_map_entry_t ** out);
lv_map_entry_t * lv_map_find(const lv_map_t * map, const lv_map_key_t * key);
lv_map_entry_t * lv_map_begin(const lv_map_t * map);
lv_map_entry_t * lv_map_end(const lv_map_t * map);
lv_map_entry_t * lv_map_rbegin(const lv_map_t * map);
lv_map_entry_t * lv_map_rend(const lv_map_t * map);

const lv_map_key_t * lv_map_entry_get_key(const lv_map_entry_t * entry);
lv_map_value_t * lv_map_entry_get_value(const lv_map_entry_t * entry);
void lv_map_entry_set_value(lv_map_entry_t * entry, const lv_map_value_t * value);
lv_map_entry_t * lv_map_entry_prev(lv_map_entry_t * entry);
lv_map_entry_t * lv_map_entry_next(lv_map_entry_t * entry);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_MAP_H*/
