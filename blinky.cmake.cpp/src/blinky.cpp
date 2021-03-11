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
 * - An active class must be derived from the class RKH_SMA_T of RKH.
 * - Every state machine's action must be implemented as a callback function, 
 *   whose signature must comply with RKH requirements, but the body of these 
 *   callbacks are written in C++ language.
 *
 * Additional notes about this example:
 * - Every C callback just calls a specific C++ method of the active class.
 * - It means that every C callback has its own C++ method, which implements 
 *   the dynamic action's behavior.
 * - C callbacks are private and non-member functions of the active class.
 * - Having defined C++ methods as protected, C callbacks were declared as 
 *   friends of the active class.
 * - Before accessing to active class members a callback must perform a 
 *   downcast to the active class.
 * - Using inheritance the behavior of state machine's actions could 
 *   be dynamically changed.
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
void initCb(RKH_SMA_T* const me, RKH_EVT_T* pe);

/* ......................... Declares entry actions ........................ */
void nLedOnCb(RKH_SMA_T* const me);
void nLedOffCb(RKH_SMA_T* const me);

/* ......................... Declares exit actions ......................... */
/* ............................ Declares guards ............................ */
/* ........................ States and pseudostates ........................ */
RKH_CREATE_BASIC_STATE(ledOn, nLedOnCb, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(ledOn)
    RKH_TRREG(evTout, NULL, NULL, &ledOff),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(ledOff, nLedOffCb, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(ledOff)
    RKH_TRREG(evTout, NULL, NULL, &ledOn),
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

void
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

void
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

void
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
