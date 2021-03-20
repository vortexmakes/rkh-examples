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
/* ------------------------------- Constants ------------------------------- */
static const unsigned int EVTQUE_STORAGE_SIZE = 4;

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* .............................. Event queues ............................. */
static RKH_EVT_T* evtQueStorage[EVTQUE_STORAGE_SIZE];

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
int
main(int argc, char* argv[])
{
    Bsp::getInstance(argc, argv);

    RKH_SMA_ACTIVATE(blinky, &evtQueStorage, EVTQUE_STORAGE_SIZE, 0, 0);
    rkh_fwk_enter();
    RKH_TRC_CLOSE();
    return 0;
}

/* ------------------------------ End of file ------------------------------ */
