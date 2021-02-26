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

/* ---------------------- External C language linkage ---------------------- */
/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ................................ Signals ................................ */
/* ........................ Declares active object ......................... */
RKH_SMA_DCLR(blinky);

class Blinky
{
    RKH_SMA_T ao;       /* base structure */
    rui8_t cnt;         /* private member */
    RKHTmEvt timer;     /* it is responsible for toggling the LED */
                        /* posting the evTout signal event to active object */
                        /* 'blinky' */
    public:
    Blinky(int prio);
    enum Signals
    {
	    evTout,         /* timeout */
        evTerminate
    };
};

/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
/* -------------------- External C language linkage end -------------------- */
/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
