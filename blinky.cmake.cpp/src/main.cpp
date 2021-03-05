/**
 *  \file       main.cpp
 *  \brief      Example application.
 */

/* -------------------------- Development history -------------------------- */
/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "blinky.h"
#include "bsp.h"
#include <string>
#include <iostream>

/* ----------------------------- Local macros ------------------------------ */
#define QSTO_SIZE           4

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
RKH_SMA_T *blinky;

/* ---------------------------- Local variables ---------------------------- */
static RKH_EVT_T *qsto[QSTO_SIZE];

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
int
main(int argc, char *argv[])
{
    Blinky aoBlinky(BlinkyPrio);
    blinky = &aoBlinky;
    Bsp::getInstance(argc, argv);

    RKH_SMA_ACTIVATE(blinky, qsto, QSTO_SIZE, 0, 0);
    rkh_fwk_enter();
    RKH_TRC_CLOSE();

    return 0;
}

/* ------------------------------ End of file ------------------------------ */