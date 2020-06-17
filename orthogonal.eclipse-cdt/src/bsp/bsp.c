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
#include "trace_io_cfg.h"
#include "rkhfwk_dynevt.h"
#include "rkhfwk_evtpool.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
#define ESC                         0x1B
#define kbmap(c)                    ((c) - '0')
#define SIZEOF_EP0STO               64
#define SIZEOF_EP0_BLOCK            sizeof(StatusEvt)

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static uint8_t ep0sto[SIZEOF_EP0STO];

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void
printBanner(void)
{
    printf("Parameterized state machines\n\n");
    printf("RKH version      = %s\n", RKH_RELEASE);
    printf("Port version     = %s\n", rkhport_get_version());
    printf("Port description = %s\n\n", rkhport_get_desc());
    printf("Description: \n\n"
           "The goal of this demo application is to show how to \n"
           "represent a parameterized state machine using the RKH framework.\n"
           "This kind of state machine could be thought as an array of \n"
           "state machines.\n\n");
    printf("1.- Press 'a' to active pulse counter 0\n");
    printf("2.- Press 'b' to inactive pulse counter 0\n");
    printf("3.- Press 'c' to active pulse counter 3\n");
    printf("4.- Press 'd' to inactive pulse counter 3\n");
    printf("5.- Press ESC to quit\n\n");

}

/* ---------------------------- Global functions --------------------------- */
void
bsp_init(int argc, char *argv[])
{
    printBanner();
    trace_io_setConfig(argc, argv);
    rkh_dynEvt_init();
    rkh_fwk_registerEvtPool(ep0sto, SIZEOF_EP0STO, SIZEOF_EP0_BLOCK);
}

void
bsp_keyParser(int c)
{
    StatusEvt *statusEvt;

    switch(c)
    {
        case ESC:
            rkhport_fwk_stop();
            break;
        case 'a':
            statusEvt = RKH_ALLOC_EVT(StatusEvt, evActive, pulseCounterMgr);
            statusEvt->id = 0; /* to SM component 0 */
            RKH_SMA_POST_FIFO(pulseCounterMgr, 
                              RKH_UPCAST(RKH_EVT_T, statusEvt), NULL);
            break;
        case 'b':
            statusEvt = RKH_ALLOC_EVT(StatusEvt, evInactive, pulseCounterMgr);
            statusEvt->id = 0; /* to SM component 0 */
            RKH_SMA_POST_FIFO(pulseCounterMgr, 
                              RKH_UPCAST(RKH_EVT_T, statusEvt), NULL);
            break;
        case 'c':
            statusEvt = RKH_ALLOC_EVT(StatusEvt, evActive, pulseCounterMgr);
            statusEvt->id = NUM_PULSE_COUNTERS - 1;
            RKH_SMA_POST_FIFO(pulseCounterMgr, 
                              RKH_UPCAST(RKH_EVT_T, statusEvt), NULL);
            break;
        case 'd':
            statusEvt = RKH_ALLOC_EVT(StatusEvt, evInactive, pulseCounterMgr);
            statusEvt->id = NUM_PULSE_COUNTERS - 1;
            RKH_SMA_POST_FIFO(pulseCounterMgr, 
                              RKH_UPCAST(RKH_EVT_T, statusEvt), NULL);
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
bsp_register(int pulseCounterId, uint32_t nPulses)
{
    printf("PulseCounter %d detected %d pulses\n", pulseCounterId,
                                                   nPulses);
}

/* ------------------------------ File footer ------------------------------ */
