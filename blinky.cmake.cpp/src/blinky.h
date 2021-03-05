/**
 *  \file       blinky.h
 *  \brief      Example application.
 */

/* -------------------------- Development history -------------------------- */
/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __BLINKY_H__
#define __BLINKY_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include <string>
#include "priority.h"

/* ---------------------- External C language linkage ---------------------- */
/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ................................ Signals ................................ */
/* ........................ Declares active object ......................... */
class Blinky: public RKH_SMA_T
{
    public:
    enum Signals
    {
        evTout,         /* timeout */
        evTerminate     /* finishes the program */
    };

    rui8_t cnt = 0;
    RKHTmEvt timer;     /* it is responsible for toggling the LED */
                        /* posting the evTout signal event to active object */
                        /* 'blinky' */
    Blinky() = delete;
    Blinky(ActObjPriority prio);
    void init(RKH_EVT_T *pe);
    void nLedOn();
    void nLedOff();
    void print() const;
};

/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
/* -------------------- External C language linkage end -------------------- */
/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
