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
 * - This alternative is a kind of singleton class in which its specification 
 *   is entirely hidden. It provides a constant, opaque and global pointer to 
 *   access the only class instance.
 * - In order to test this alternative you have to rename files 
 *   _blinky.closed-singleton.h_/_blinky.closed-singleton.cpp_/
 *   _bsp_blinky.closed-singleton.cpp_/main.closed-singleton.cpp_ to 
 *   blinky.h/blinky.cpp/bsp_blinky.cpp/main.cpp respectively.
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
class Blinky: public RKH_SMA_T
{
    private:
        /* --------------------------- Functions --------------------------- */
        Blinky(ActObjPriority prio, const char* name);

        /* ........................ Effect actions ......................... */
        friend void initCb(RKH_SMA_T* const me, RKH_EVT_T* pe);

        /* ......................... Entry actions ......................... */
        friend void nLedOnCb(RKH_SMA_T* const me);
        friend void nLedOffCb(RKH_SMA_T* const me);

        /* ......................... Exit actions .......................... */
        /* ............................ Guards ............................. */

    public:
        /* --------------------------- Attributes -------------------------- */
        static Blinky instance;

        /* ............................ Signals ............................ */
        /* --------------------------- Functions --------------------------- */
        void print() const;

    protected:
        /* ........................... Properties .......................... */
        rui8_t cnt;
        RKHTmEvt timer;     /* it is responsible for toggling the LED */
                            /* posting the evTout signal event to active */
                            /* object 'blinky' */

        /* ........................ Effect actions ......................... */
        void init(RKH_EVT_T* pe);

        /* ......................... Entry actions ......................... */
        void nLedOn();
        void nLedOff();

        /* ......................... Exit actions .......................... */
        /* ............................ Guards ............................. */
};

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
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
RKH_SMA_T* const blinky = &Blinky::instance;

/* ---------------------------- Local variables ---------------------------- */
Blinky Blinky::instance(BlinkyPrio, "blinky");

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
Blinky::Blinky(ActObjPriority prio, const char* name) 
    : RKH_SMA_T{prio, 
                HCAL, 
                name, 
                st_cast(&ledOn), 
                act_cast(initCb), 
                st_cast(&ledOn)}, 
      cnt(0)
{
}

void 
Blinky::print() const
{
    std::cout << 
        "RKH_SMA_T::sm.prio = " << 
        static_cast<int>(sm.prio) << std::endl;
    std::cout << 
        "RKH_SMA_T::sm.name = " << 
        sm.name << std::endl;
    std::cout << 
        "cnt = " << 
        (int)cnt << std::endl;
    std::cout << std::endl;
}

/* ------------------------------ End of file ------------------------------ */
