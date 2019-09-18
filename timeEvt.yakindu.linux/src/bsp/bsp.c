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
 *  \brief      BSP for TimeEvt example on Linux
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
#include <stdio.h>
#include <stdlib.h>

#include "TimeEvt.h"
#include "signals.h"
#include "event.h"
#include "bsp.h"
#include "rkh.h"
#include "trace_io_cfg.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
#define ESC                         0x1B
#define kbmap(c)                    ((c) - '0')

#define SIZEOF_EP0STO               64
#define SIZEOF_EP0_BLOCK            4
#define SIZEOF_EP1STO               32
#define SIZEOF_EP1_BLOCK            8

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static RKH_ROM_STATIC_EVENT(evStart, start);
static RKH_ROM_STATIC_EVENT(evStop, stop);
static RKH_ROM_STATIC_EVENT(evResume, resume);
static RKH_ROM_STATIC_EVENT(evReset, reset);

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void
printBanner(void)
{
    printf("Time Event example\n\n");
    printf("RKH version      = %s\n", RKH_RELEASE);
    printf("Port version     = %s\n", rkhport_get_version());
    printf("Port description = %s\n\n", rkhport_get_desc());
    printf("Description: \n\n"
           "The goal of this demo application is to explain how to \n"
           "represent a state machine using the RKH framework. To do \n"
           "that is proposed a simple and abstract example, which is \n"
           "shown in the documentation file Figure 1 section \n"
           "\"Representing a State Machine\". \n\n\n");

    printf("1.- Press s to start. \n");
    printf("2.- Press p to stop. \n");
    printf("3.- Press r to resume. \n");
    printf("4.- Press x to reset. \n");
    printf("5.- Press ESC to quit \n\n\n");

}

/* ---------------------------- Global functions --------------------------- */
void
bsp_init(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    printBanner();

    trace_io_setConfig(argc, argv);
}

void
bsp_keyParser(int c)
{
    switch(c)
    {
        case ESC:
            rkhport_fwk_stop();
            break;

        case 's':
            RKH_SMA_POST_FIFO(timeEvt, &evStart, 0);
            break;

        case 'p':
            RKH_SMA_POST_FIFO(timeEvt, &evStop, 0);
            break;

        case 'r':
            RKH_SMA_POST_FIFO(timeEvt, &evResume, 0);
            break;

        case 'x':
            RKH_SMA_POST_FIFO(timeEvt, &evReset, 0);
            break;
    }
}

void
bsp_timeTick(void)
{
}

void
bsp_ledOn(void)
{
    printf("LED ON\n");
}

void
bsp_ledOff(void)
{
    printf("LED OFF\n");
}

void
bsp_ledToggle(void)
{
    printf("LED TOGGLE\n");
}

/* ------------------------------ File footer ------------------------------ */
