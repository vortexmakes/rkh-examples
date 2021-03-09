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
/* Despite RKH framework is written in C language, it could be used in a C++ 
 * application without much effort. This is mainly due to RKH framework was 
 * developed from ground up using OOP concepts. However, you have to keep in 
 * mind some simple things if you want to use it in your C++ application:
 *
 * - An active class must be derived from the RKH's RKH_SMA_T class.
 * - Every state machine's action is implemented as a C function callback
 * - Every C callback only calls a specific C++ method of the active class.
 * - So, every C callback has its own C++ method.
 * - It implements the dynamic action's behavior.
 */

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
static void initCb(RKH_SMA_T* const me, RKH_EVT_T* pe);

/* ......................... Declares entry actions ........................ */
static void nLedOnCb(RKH_SMA_T* const me);
static void nLedOffCb(RKH_SMA_T* const me);

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
Blinky::Blinky(ActObjPriority prio, ActObjName name)
{
    RKHActObjInit(this, name, prio, &ledOn, initCb);
}

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ............................ Effect actions ............................. */
void 
Blinky::init(RKH_EVT_T* pe)
{
    RKH_SMA_T* ao = static_cast<RKH_SMA_T*>(this);  /* performs an upcast */

    RKH_TR_FWK_AO(ao);
    RKH_TR_FWK_QUEUE(&equeue);
    RKH_TR_FWK_STATE(ao, &ledOn);
    RKH_TR_FWK_STATE(ao, &ledOff);
    RKH_TR_FWK_OBJ_NAME(&timer.tmr, "timer");
    RKH_TR_FWK_SIG(evTout);
    RKH_TR_FWK_SIG(evTerminate);

    RKH_SET_STATIC_EVENT(&timer, evTout);
    RKH_TMR_INIT(&timer.tmr, RKH_UPCAST(RKH_EVT_T, &timer), NULL);
    cnt = 0;
}

static void
initCb(RKH_SMA_T* const me, RKH_EVT_T* pe)
{
    Blinky* realMe = static_cast<Blinky*>(me);      /* performs a downcast */
    realMe->init(pe);
}

/* ............................. Entry actions ............................. */
void 
Blinky::nLedOn()
{
    Bsp* bsp = Bsp::getInstance(0, nullptr);
    RKH_SMA_T* ao = static_cast<RKH_SMA_T*>(this);  /* performs an upcast */

    RKH_TMR_ONESHOT(&timer.tmr, ao, LedOnTime(2));
    bsp->ledOn();
    ++cnt;
}

static void
nLedOnCb(RKH_SMA_T* const me)
{
    Blinky* realMe = static_cast<Blinky*>(me);      /* performs a downcast */
    realMe->nLedOn();
}

void 
Blinky::nLedOff()
{
    Bsp* bsp = Bsp::getInstance(0, nullptr);
    RKH_SMA_T* ao = static_cast<RKH_SMA_T*>(this);  /* performs an upcast */

    RKH_TMR_ONESHOT(&timer.tmr, ao, LedOffTime(2));
    bsp->ledOff();
}

static void
nLedOffCb(RKH_SMA_T* const me)
{
    Blinky* realMe = static_cast<Blinky*>(me);      /* performs a downcast */
    realMe->nLedOff();
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

/* ------------------------------ End of file ------------------------------ */
