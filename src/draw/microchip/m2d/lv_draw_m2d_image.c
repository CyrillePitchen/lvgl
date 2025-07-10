/**
 * @file lv_draw_m2d_img.c
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

#include "../../../lv_conf_internal.h"

#if LV_USE_DRAW_M2D
#include "lv_draw_m2d_private.h"

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

bool lv_draw_m2d_image_is_supported(const lv_draw_image_dsc_t * draw_dsc)
{
    if (draw_dsc->clip_radius == 0
	&& draw_dsc->rotation == 0
	&& draw_dsc->scale_x == LV_SCALE_NONE && draw_dsc->scale_y == LV_SCALE_NONE
	&& draw_dsc->skew_x == 0 && draw_dsc->skew_y == 0
	&& draw_dsc->recolor_opa <= LV_OPA_MIN
	&& (draw_dsc->blend_mode == LV_BLEND_MODE_NORMAL
            || draw_dsc->blend_mode == LV_BLEND_MODE_ADDITIVE
            || draw_dsc->blend_mode == LV_BLEND_MODE_MULTIPLY)
        && draw_dsc->tile == 0
        && draw_dsc->sup == NULL
        && draw_dsc->bitmap_mask_src == NULL) {
        return true;
    }

    return false;
}

void lv_draw_m2d_image(lv_draw_task_t * t, const lv_draw_image_dsc_t * draw_dsc, const lv_area_t * coords)
{
    if(draw_dsc->opa <= (lv_opa_t)LV_OPA_MIN)
        return;

    lv_layer_t * layer = t->target_layer;
    lv_draw_m2d_buf_t * target = (lv_draw_m2d_buf_t *)layer->draw_buf;
    lv_draw_m2d_buf_t * src = lv_draw_m2d_search_buf_map(draw_dsc->src);
    if(!src) {
        LV_LOG_ERROR("failed to get some lv_draw_m2d_buf_t from the lv_image_dsc_t source");
        return;
    }

    lv_draw_m2d_buf_sync_for_gpu(target);
    lv_draw_m2d_buf_sync_for_gpu(src);

    struct m2d_rectangle rect;
    rect.x = coords->x1;
    rect.y = coords->y1;
    rect.w = lv_area_get_width(coords);
    rect.h = lv_area_get_height(coords);

    m2d_source_color(255, 255, 255, 255);

    dim_t x = t->area.x1 - layer->buf_area.x1;
    dim_t y = t->area.y1 - layer->buf_area.y1;
    m2d_set_source(M2D_SRC, src->m2d_buf, x, y);
    m2d_source_enable(M2D_SRC, true);

    bool blend_enable = true;
    switch(draw_dsc->blend_mode) {
        case LV_BLEND_MODE_ADDITIVE:
            m2d_blend_factors(M2D_BLEND_SRC_ALPHA, M2D_BLEND_ONE,
                              M2D_BLEND_ONE, M2D_BLEND_ONE_MINUS_SRC_ALPHA);
            break;
        case LV_BLEND_MODE_MULTIPLY:
            m2d_blend_factors(M2D_BLEND_DST_COLOR, M2D_BLEND_ZERO,
                              M2D_BLEND_DST_COLOR, M2D_BLEND_ZERO);
            break;
        case LV_BLEND_MODE_NORMAL:
        default:
            m2d_blend_factors(M2D_BLEND_SRC_ALPHA, M2D_BLEND_ONE_MINUS_SRC_ALPHA,
                              M2D_BLEND_ONE, M2D_BLEND_ONE_MINUS_SRC_ALPHA);

            uint8_t alpha = (draw_dsc->opa >= (lv_opa_t)LV_OPA_MAX) ? 255 : draw_dsc->opa;
            if(lv_color_format_has_alpha((lv_color_format_t)src->base_buf.header.cf) ||
               alpha != 255) {
                m2d_source_color(255, 255, 255, alpha);
            }
            else {
                blend_enable = false;
            }
            break;
    }
    if(blend_enable)
        m2d_set_source(M2D_DST, target->m2d_buf, 0, 0);
    m2d_source_enable(M2D_DST, blend_enable);
    m2d_blend_enable(blend_enable);
    m2d_set_target(target->m2d_buf);
    m2d_draw_rectangles(&rect, 1);
}

#endif /*LV_USE_DRAW_M2D*/
