/**
 * @file lv_draw_m2d_private.h
 *
 */

#ifndef LV_DRAW_M2D_PRIVATE_H
#define LV_DRAW_M2D_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
*      INCLUDES
*********************/
#include "../../../lv_conf_internal.h"

#if LV_USE_DRAW_M2D

#include "../../../display/lv_display_private.h"
#include "../../lv_draw_private.h"
#include "../../lv_image_dsc.h"
#include "../../lv_draw_rect.h"
#include <m2d/m2d.h>

/*********************
*      DEFINES
*********************/

/**********************
*      TYPEDEFS
**********************/
typedef struct _lv_draw_m2d_unit_t {
    lv_draw_unit_t base_unit;
    lv_draw_task_t * task_act;
} lv_draw_m2d_unit_t;

typedef struct _lv_draw_m2d_buf_t {
    lv_draw_buf_t base_buf;
    struct m2d_buffer * m2d_buf;
    void * alias_key;
    bool owned_by_gpu;
} lv_draw_m2d_buf_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void lv_draw_buf_m2d_init_handlers(void);

void lv_draw_m2d_init_buf_map(void);

void lv_draw_m2d_cleanup_buf_map(void);

lv_draw_m2d_buf_t * lv_draw_m2d_search_buf_map(const lv_image_dsc_t * img_dsc);

int32_t lv_draw_m2d_buf_sync_for_cpu(lv_draw_m2d_buf_t * draw_m2d_buf);

int32_t lv_draw_m2d_buf_sync_for_gpu(lv_draw_m2d_buf_t * draw_m2d_buf);

bool lv_draw_m2d_fill_is_supported(const lv_draw_fill_dsc_t * draw_dsc);

void lv_draw_m2d_fill(lv_draw_task_t * t, const lv_draw_fill_dsc_t * draw_dsc, const lv_area_t * coords);

bool lv_draw_m2d_image_is_supported(const lv_draw_image_dsc_t * draw_dsc);

void lv_draw_m2d_image(lv_draw_task_t * t, const lv_draw_image_dsc_t * draw_dsc, const lv_area_t * coords);

bool lv_draw_m2d_layer_is_supported(const lv_draw_image_dsc_t * draw_dsc);

void lv_draw_m2d_layer(lv_draw_task_t *t, const lv_draw_image_dsc_t * draw_dsc, const lv_area_t * coords);

#endif /*LV_USE_DRAW_M2D*/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_DRAW_M2D_PRIVATE_H*/
