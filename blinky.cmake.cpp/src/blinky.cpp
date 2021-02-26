/**
 *  \file       blinky.cpp
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
#include "priority.h"
#include "signal.h"
#include "event.h"
#include "bsp.h"
#include <iostream>

/* ----------------------------- Local macros ------------------------------ */
#define LedOffTime      RKH_TIME_SEC(2)
#define LedOnTime       RKH_TIME_SEC(2)

/* ......................... Declares active object ........................ */
/* ................... Declares states and pseudostates .................... */
/* ........................ Declares effect actions ........................ */
/* ......................... Declares entry actions ........................ */
/* ......................... Declares exit actions ......................... */
/* ............................ Declares guards ............................ */
/* ........................ States and pseudostates ........................ */
/* ............................. Active object ............................. */
Blinky::Blinky(int prio)
{
    RKH_SM_INIT(&ao, "blinky", prio, HCAL, NULL, NULL, NULL);
    cnt = 0;
    std::cout << "ao.sm.prio = " << (int)ao.sm.prio << std::endl;
    std::cout << "ao.sm.name = " << ao.sm.name << std::endl;
}

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ............................ Effect actions ............................. */
/* ............................. Entry actions ............................. */
/* ............................. Exit actions .............................. */
/* ................................ Guards ................................. */
/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
