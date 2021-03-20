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
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
int
main(int argc, char* argv[])
{
    Blinky& aoblinky = Blinky::getInstance(BlinkyPrio, "blinky");
    aoblinky.print();
    Bsp::getInstance(argc, argv);

    RKH_SMA_ACTIVATE(&aoblinky, 
                     static_cast<Blinky&>(aoblinky).getEvtQue(), 
                     static_cast<Blinky&>(aoblinky).getSizeOfEvtQue(), 0, 0);
    rkh_fwk_enter();
    RKH_TRC_CLOSE();
    return 0;
}

/* ------------------------------ End of file ------------------------------ */
