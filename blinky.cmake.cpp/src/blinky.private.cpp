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
#include "rkhfwk_adapter.h"

/* ----------------------------- Local macros ------------------------------ */
inline int
LedOffTime(int value)
{
    return RKH_TIME_SEC(value);
}

inline int
LedOnTime(int value)
{
    return RKH_TIME_SEC(value);
}

/* ......................... Declares active object ........................ */
/* ................... Declares states and pseudostates .................... */
RKH_DCLR_BASIC_STATE ledOff, ledOn;

/* ........................ Declares effect actions ........................ */
void initCb(RKH_SMA_T* const me, RKH_EVT_T* pe);

/* ......................... Declares entry actions ........................ */
void nLedOnCb(RKH_SMA_T* const me);
void nLedOffCb(RKH_SMA_T* const me);

/* ......................... Declares exit actions ......................... */
/* ............................ Declares guards ............................ */
/* ........................ States and pseudostates ........................ */
RKH_CREATE_BASIC_STATE(ledOn, nLedOnCb, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(ledOn)
    RKH_TRREG(Blinky::evTout, NULL, NULL, &ledOff),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(ledOff, nLedOffCb, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(ledOff)
    RKH_TRREG(Blinky::evTout, NULL, NULL, &ledOn),
RKH_END_TRANS_TABLE

/* ............................. Active object ............................. */
Blinky::Blinky(ActObjPriority prio, const ActObjName name) 
    : RKH_SMA_T{prio, 
                HCAL, 
                name.c_str(), 
                st_cast(&ledOn), 
                act_cast(initCb), 
                st_cast(&ledOn)}, 
      cnt(0)
{
}

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ............................ Effect actions ............................. */
void
initCb(RKH_SMA_T* const me, RKH_EVT_T* pe)
{
    Blinky* realMe = static_cast<Blinky*>(me);      /* performs a downcast */

    RKH_TR_FWK_AO(me);
    RKH_TR_FWK_QUEUE(&me->equeue);
    RKH_TR_FWK_STATE(me, &ledOn);
    RKH_TR_FWK_STATE(me, &ledOff);
    RKH_TR_FWK_OBJ_NAME(&realMe->timer.tmr, "timer");
    RKH_TR_FWK_SIG(Blinky::evTout);
    RKH_TR_FWK_SIG(Blinky::evTerminate);

    RKH_SET_STATIC_EVENT(&realMe->timer, Blinky::evTout);
    RKH_TMR_INIT(&realMe->timer.tmr, 
                 RKH_UPCAST(RKH_EVT_T, &realMe->timer), 
                 NULL);
    realMe->cnt = 0;
}

/* ............................. Entry actions ............................. */
void
nLedOnCb(RKH_SMA_T* const me)
{
    Blinky* realMe = static_cast<Blinky*>(me);      /* performs a downcast */
    Bsp* bsp = Bsp::getInstance(0, nullptr);

    RKH_TMR_ONESHOT(&realMe->timer.tmr, me, LedOnTime(2));
    bsp->ledOn();
    ++realMe->cnt;
}

void
nLedOffCb(RKH_SMA_T* const me)
{
    Blinky* realMe = static_cast<Blinky*>(me);      /* performs a downcast */
    Bsp* bsp = Bsp::getInstance(0, nullptr);

    RKH_TMR_ONESHOT(&realMe->timer.tmr, me, LedOffTime(2));
    bsp->ledOff();
}

/* ............................. Exit actions .............................. */
/* ................................ Guards ................................. */
/* ---------------------------- Global functions --------------------------- */
void 
Blinky::print() const
{
    std::cout << 
        "RKH_SMA_T::sm.prio = " << 
        static_cast<int>(sm.prio) << std::endl;
    std::cout << 
        "RKH_SMA_T::sm.name = " << 
        sm.name << std::endl;
    std::cout << std::endl;
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
