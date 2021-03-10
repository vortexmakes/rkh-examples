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
#include "priority.h"
#include "rkhfwk_adapter.h"

/* ---------------------- External C language linkage ---------------------- */
/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ........................ Declares active object ......................... */
class Blinky: public RKH_SMA_T
{
    /* ------------------------------ Constants ---------------------------- */
    static const unsigned int QSTO_SIZE = 4;

    /* ----------------------------- Attributes ---------------------------- */
    /* ............................ Event queue ............................ */
    RKH_EVT_T* qsto[QSTO_SIZE];

    /* ............................. Properties ............................ */
    rui8_t cnt = 0;
    RKHTmEvt timer;     /* it is responsible for toggling the LED */
                        /* posting the evTout signal event to active object */
                        /* 'blinky' */

    /* .......................... Effect actions ........................... */
    friend void initCb(RKH_SMA_T* const me, RKH_EVT_T* pe);

    /* ........................... Entry actions ........................... */
    friend void nLedOnCb(RKH_SMA_T* const me);
    friend void nLedOffCb(RKH_SMA_T* const me);

    /* ........................... Exit actions ............................ */
    /* .............................. Guards ............................... */

    public:
    /* .............................. Signals .............................. */
    /* ----------------------------- Functions ----------------------------- */
    Blinky() = delete;
    Blinky(ActObjPriority prio, ActObjName name);
    void print() const;
    RKH_EVT_T* const* getEvtQue() const;
    unsigned int getSizeOfEvtQue() const;
};

/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
/* -------------------- External C language linkage end -------------------- */
/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
