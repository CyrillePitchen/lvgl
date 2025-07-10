/**
 * @file lv_draw_m2d.c
 *
 */

/**
 * Copyright 2025 Microchip Technology
 *
 * SPDX-License-Identifier: MIT
 */


/*********************
*      INCLUDES
*********************/

#include "lv_draw_m2d_private.h"
#include "lv_draw_m2d.h"

#if LV_USE_DRAW_M2D

#include "../../../misc/lv_area_private.h"
#include <m2d/m2d.h>

/*********************
 *      DEFINES
 *********************/

#define DRAW_UNIT_ID_M2D         9
#define DRAW_UNIT_M2D_PREF_SCORE 70

/**********************
 *  STATIC PROTOTYPES
 **********************/

static int32_t m2d_evaluate(lv_draw_unit_t * draw_unit, lv_draw_task_t * task);
static int32_t m2d_dispatch(lv_draw_unit_t * draw_unit, lv_layer_t * layer);
static void m2d_execute_drawing(lv_draw_m2d_unit_t * u);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_draw_m2d_init(void)
{
    lv_draw_buf_m2d_init_handlers();

    lv_draw_m2d_unit_t * draw_m2d_unit = lv_draw_create_unit(sizeof(lv_draw_m2d_unit_t));
    draw_m2d_unit->base_unit.evaluate_cb = m2d_evaluate;
    draw_m2d_unit->base_unit.dispatch_cb = m2d_dispatch;
    draw_m2d_unit->base_unit.name = "M2D";
    lv_draw_m2d_init_buf_map();

    if(m2d_init()) {
        LV_LOG_ERROR("m2d_init() failed");
	return;
    }

    m2d_blend_functions(M2D_FUNC_ADD, M2D_FUNC_ADD);
}

void lv_draw_m2d_deinit(void)
{
    lv_draw_m2d_cleanup_buf_map();
    m2d_cleanup();
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static int32_t m2d_evaluate(lv_draw_unit_t * u, lv_draw_task_t * t)
{
    LV_UNUSED(u);

    bool supported = false;
    switch(t->type) {
        case LV_DRAW_TASK_TYPE_FILL:
            if(lv_draw_m2d_fill_is_supported((lv_draw_fill_dsc_t *)t->draw_dsc))
                supported = true;
            break;
        case LV_DRAW_TASK_TYPE_IMAGE:
            if(lv_draw_m2d_image_is_supported((lv_draw_image_dsc_t *)t->draw_dsc))
                supported = true;
            break;

        case LV_DRAW_TASK_TYPE_LAYER:
            if(lv_draw_m2d_layer_is_supported((lv_draw_image_dsc_t *)t->draw_dsc))
                supported = true;
            break;


        default:
            break;
    }

    if(!supported) {
        lv_draw_m2d_buf_t * target = (lv_draw_m2d_buf_t *)t->target_layer->draw_buf;

        lv_draw_m2d_buf_sync_for_cpu(target);
        return 0;
    }

    if(t->preference_score > DRAW_UNIT_M2D_PREF_SCORE) {
        t->preference_score = DRAW_UNIT_M2D_PREF_SCORE;
        t->preferred_draw_unit_id = DRAW_UNIT_ID_M2D;
    }
    return 1;
}

static int32_t m2d_dispatch(lv_draw_unit_t * draw_unit, lv_layer_t * layer)
{
    lv_draw_m2d_unit_t * u = (lv_draw_m2d_unit_t *)draw_unit;

    /* Return immediately if it's busy with draw task. */
    if(u->task_act)
        return 0;

    /* Try to get one ready to draw. */
    lv_draw_task_t * t = lv_draw_get_available_task(layer, NULL, DRAW_UNIT_ID_M2D);

    if(t == NULL || t->preferred_draw_unit_id != DRAW_UNIT_ID_M2D)
        return LV_DRAW_UNIT_IDLE;

    if(lv_draw_layer_alloc_buf(layer) == NULL)
        return LV_DRAW_UNIT_IDLE;

    u->task_act = t;
    t->state = LV_DRAW_TASK_STATE_IN_PROGRESS;
    t->draw_unit = draw_unit;

    m2d_execute_drawing(u);

    t->state = LV_DRAW_TASK_STATE_READY;
    u->task_act = NULL;

    /* The draw unit is free now. Request a new dispatching as it can get a new task. */
    lv_draw_dispatch_request();

    return 1;
}

static void m2d_execute_drawing(lv_draw_m2d_unit_t * u)
{
    lv_draw_task_t * t = u->task_act;
    lv_layer_t * layer = t->target_layer;
    lv_area_t area;

    if (!lv_area_intersect(&area, &t->area, &t->clip_area)) return;
    lv_area_move(&area, -layer->buf_area.x1, -layer->buf_area.y1);

    switch(t->type) {
        case LV_DRAW_TASK_TYPE_FILL:
            lv_draw_m2d_fill(t, t->draw_dsc, &area);
            break;
        case LV_DRAW_TASK_TYPE_IMAGE:
            lv_draw_m2d_image(t, t->draw_dsc, &area);
            break;
        case LV_DRAW_TASK_TYPE_LAYER:
            lv_draw_m2d_layer(t, t->draw_dsc, &area);
            break;
        default:
            break;
    }
}
           
#endif /*LV_USE_DRAW_M2D*/
