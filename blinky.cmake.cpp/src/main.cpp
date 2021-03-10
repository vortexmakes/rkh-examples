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
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
RKH_SMA_T* blinky;

/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
int
main(int argc, char* argv[])
{
    Blinky aoBlinky(BlinkyPrio, "blinky");
    blinky = &aoBlinky;
    Bsp::getInstance(argc, argv);
     
    RKH_SMA_ACTIVATE(blinky, 
                     aoBlinky.getEvtQue(), 
                     aoBlinky.getSizeOfEvtQue(), 0, 0);
    rkh_fwk_enter();
    RKH_TRC_CLOSE();
    return 0;
}

RKH_EVT_T* const* 
Blinky::getEvtQue() const
{
    return qsto;
}

unsigned int
Blinky::getSizeOfEvtQue() const
{
    return Blinky::QSTO_SIZE;
}

/* ------------------------------ End of file ------------------------------ */
