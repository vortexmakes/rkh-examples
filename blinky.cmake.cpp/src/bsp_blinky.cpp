/**
 *  \file       bsp_blinky.cpp
 *  \brief      Blinky BSP for Linux x86
 */

/* -------------------------- Development history -------------------------- */
/* -------------------------------- Authors -------------------------------- */
/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <iostream>

#include "blinky.h"
#include "bsp.h"
#include "rkh.h"
#include "trace_io_cfg.h"

RKH_THIS_MODULE

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
#define ESC         0x1B

#if 0
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static RKH_ROM_STATIC_EVENT(evTerm, TERMINATE);

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void
printBanner(void)
{
    printf("Blinky: a very simple state machine example.\n\n");
    printf("RKH version      = %s\n", RKH_RELEASE);
    printf("Port version     = %s\n", rkhport_get_version());
    printf("Port description = %s\n\n", rkhport_get_desc());
    printf("Description: \n\n"
           "The goal of this demo application is to explain how to \n"
           "represent a \"flat\" state machine and how to use the timer \n"
           "services using the RKH framework. To do that is proposed a \n"
           "very simple demo that use one state machine and one timer, \n"
           "which is shown and explained in the reference manual section \n"
           "\"Examples\". "
           "This is the 'hello world' of RKH programming!.\n\n\n");

    printf("1.- Press ESC to quit \n\n\n");
}

/* ---------------------------- Global functions --------------------------- */
void
bsp_init(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    printBanner();

    trace_io_setConfig(argc, argv);

    rkh_fwk_init();

    RKH_FILTER_ON_GROUP(RKH_TRC_ALL_GROUPS);
    RKH_FILTER_ON_EVENT(RKH_TRC_ALL_EVENTS);
    RKH_FILTER_OFF_EVENT(RKH_TE_TMR_TOUT);
    RKH_FILTER_OFF_EVENT(RKH_TE_SM_STATE);
    RKH_FILTER_OFF_SMA(blinky);
    RKH_FILTER_OFF_ALL_SIGNALS();

    RKH_TRC_OPEN();
}

void
bsp_keyParser(int c)
{
    if (c == ESC)
    {
        RKH_SMA_POST_FIFO(blinky, &evTerm, 0);
        rkhport_fwk_stop();
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
#endif
/* ------------------------------ File footer ------------------------------ */
