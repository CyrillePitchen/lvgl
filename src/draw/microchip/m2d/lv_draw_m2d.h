/**
 * @file lv_draw_m2d.h
 *
 */

/**
 * Copyright 2025 Microchip Technology
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef LV_DRAW_M2D_H
#define LV_DRAW_M2D_H

#ifdef __cplusplus
extern "C" {
#endif
	
/*********************
 *      INCLUDES
 *********************/

#include "../../../lv_conf_internal.h"

#if LV_USE_DRAW_M2D

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

void lv_draw_m2d_init(void);
void lv_draw_m2d_deinit(void);

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_DRAW_M2D*/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_DRAW_M2D_H*/
