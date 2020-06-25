/*
 *  --------------------------------------------------------------------------
 *
 *                                Framework RKH
 *                                -------------
 *
 *            State-machine framework for reactive embedded systems
 *
 *                      Copyright (C) 2010 Leandro Francucci.
 *          All rights reserved. Protected by international copyright laws.
 *
 *
 *  RKH is free software: you can redistribute it and/or modify it under the
 *  terms of the GNU General Public License as published by the Free Software
 *  Foundation, either version 3 of the License, or (at your option) any
 *  later version.
 *
 *  RKH is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with RKH, see copying.txt file.
 *
 *  Contact information:
 *  RKH site: http://vortexmakes.com/que-es/
 *  RKH GitHub: https://github.com/vortexmakes/RKH
 *  RKH Sourceforge: https://sourceforge.net/projects/rkh-reactivesys/
 *  e-mail: lf@vortexmakes.com
 *  ---------------------------------------------------------------------------
 */

/**
 *  \file       bsp.c
 *  \brief      BSP for LightMgr example on Linux
 *
 *  \ingroup    bsp
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2019.09.17  DaBa  v1.0.0  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Darío Baliña       db@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <LightMgr.h>
#include <stdio.h>
#include <stdlib.h>
#include "signals.h"
#include "events.h"
#include "bsp.h"
#include "rkh.h"
#include "rkhfwk_dynevt.h"
#include "trace_io_cfg.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
#define ESC                         0x1B
#define kbmap(c)                    ((c) - '0')
static RKH_ROM_STATIC_EVENT(evStartObj, evStart);
static RKH_ROM_STATIC_EVENT(evModeObj, evMode);
static RKH_ROM_STATIC_EVENT(evRateObj, evRate);

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void
printBanner(void)
{
    printf("State machine with orthogonal regions\n\n");
    printf("RKH version      = %s\n", RKH_RELEASE);
    printf("Port version     = %s\n", rkhport_get_version());
    printf("Port description = %s\n\n", rkhport_get_desc());
    printf("Description: \n\n"
           "The goal of this demo application is to show how to \n"
           "represent a state machine with multiple orthogonal regions \n"
           "using the RKH framework.\n");
    printf("1.- Press 'a' to change light sequence mode\n");
    printf("2.- Press 'b' to change rate of switch on and off the light\n");
    printf("3.- Press 'c' to start a cycle in OneCycle mode\n");
    printf("4.- Press ESC to quit\n\n");
}

/* ---------------------------- Global functions --------------------------- */
void
bsp_init(int argc, char *argv[])
{
    printBanner();
    trace_io_setConfig(argc, argv);
}

void
bsp_keyParser(int c)
{
    switch (c)
    {
        case ESC:
            rkhport_fwk_stop();
            break;
        case 'a':
            RKH_SMA_POST_FIFO(lightMgr, &evModeObj, NULL);
            break;
        case 'b':
            RKH_SMA_POST_FIFO(lightMgr, &evRateObj, NULL);
            break;
        case 'c':
            RKH_SMA_POST_FIFO(lightMgr, &evStartObj, NULL);
            break;
        default:
            break;
    }
}

void
bsp_timeTick(void)
{
}

void
bsp_turnOnRedLight(void)
{
    printf("Set red light\n");
}

void 
bsp_turnOnYellowLight(void)
{
    printf("Set yellow light\n");
}

void 
bsp_turnOnGreenLight(void)
{
    printf("Set green light\n");
}

/* ------------------------------ File footer ------------------------------ */
