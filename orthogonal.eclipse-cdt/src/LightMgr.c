/**
 *  \file       LightMgr.c
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2020.17.06  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <LightMgr.h>
#include <stdio.h>
#include "rkhfwk_cast.h"
#include "rkhfwk_dynevt.h"
#include "rkhtrc_record.h"
#include "rkhsm.h"
#include "rkhsma.h"
#include "rkhassert.h"
#include "rkhtmr.h"
#include "signals.h"
#include "events.h"
#include "priorities.h"
#include "bsp.h"

/* ----------------------------- Local macros ------------------------------ */
RKH_MODULE_NAME(LightMgr)
#define ACT_MIN_TIME    RKH_TIME_SEC(3)
#define ACT_MAX_TIME    RKH_TIME_SEC(5)
#define INACT_MAX_TIME  RKH_TIME_SEC(5)

/* ......................... Declares active object ........................ */
typedef struct PulseCounter PulseCounter;
typedef struct LightMgr LightMgr;

/* ................... Declares states and pseudostates .................... */
RKH_DCLR_BASIC_STATE Processing;
RKH_DCLR_BASIC_STATE PulseCounter_Idle, PulseCounter_Setup, 
                     PulseCounter_Active, PulseCounter_Inactive, 
                     PulseCounter_WaitInactive;

/* ........................ Declares effect actions ........................ */
static void LightMgr_init(LightMgr *const me, RKH_EVT_T *pe);
static void LightMgr_dispatchStatus(LightMgr *const me, 
                                           RKH_EVT_T *pe);
static void LightMgr_dispatchTout(LightMgr *const me, 
                                         RKH_EVT_T *pe);
static void PulseCounter_init(PulseCounter *const me, RKH_EVT_T *pe);
static void PulseCounter_reset(PulseCounter *const me, RKH_EVT_T *pe);
static void PulseCounter_pulse(PulseCounter *const me, RKH_EVT_T *pe);
static void PulseCounter_register(PulseCounter *const me, RKH_EVT_T *pe);

/* ......................... Declares entry actions ........................ */
static void PulseCounter_enSetup(PulseCounter *const me);
static void PulseCounter_enActive(PulseCounter *const me);
static void PulseCounter_enInactive(PulseCounter *const me);

/* ......................... Declares exit actions ......................... */
static void PulseCounter_exSetup(PulseCounter *const me);
static void PulseCounter_exActive(PulseCounter *const me);
static void PulseCounter_exInactive(PulseCounter *const me);

/* ............................ Declares guards ............................ */
/* ........................ States and pseudostates ........................ */
RKH_CREATE_BASIC_STATE(Processing, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(Processing)
	RKH_TRINT(evInactive, NULL, LightMgr_dispatchStatus),
	RKH_TRINT(evActive, NULL, LightMgr_dispatchStatus),
	RKH_TRINT(evTactMinTout, NULL, LightMgr_dispatchTout),
	RKH_TRINT(evTactMaxTout, NULL, LightMgr_dispatchTout),
	RKH_TRINT(evTinactMaxTout, NULL, LightMgr_dispatchTout),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(PulseCounter_Idle, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(PulseCounter_Idle)
	RKH_TRREG(evActive, NULL, PulseCounter_reset, &PulseCounter_Setup),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(PulseCounter_Setup, PulseCounter_enSetup, 
                       PulseCounter_exSetup, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(PulseCounter_Setup)
	RKH_TRREG(evTactMinTout, NULL, NULL, &PulseCounter_Active),
	RKH_TRREG(evInactive, NULL, NULL, &PulseCounter_Idle),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(PulseCounter_Active, PulseCounter_enActive, 
                       PulseCounter_exActive, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(PulseCounter_Active)
	RKH_TRREG(evInactive, NULL, PulseCounter_pulse, 
              &PulseCounter_Inactive),
	RKH_TRREG(evTactMaxTout, NULL, NULL, &PulseCounter_WaitInactive),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(PulseCounter_Inactive, PulseCounter_enInactive, 
                       PulseCounter_exInactive, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(PulseCounter_Inactive)
	RKH_TRREG(evActive, NULL, NULL, &PulseCounter_Setup),
	RKH_TRREG(evTinactMaxTout, NULL, PulseCounter_register, 
              &PulseCounter_Idle),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(PulseCounter_WaitInactive, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(PulseCounter_WaitInactive)
	RKH_TRREG(evInactive, NULL, NULL, &PulseCounter_Idle),
RKH_END_TRANS_TABLE

/* ............................. Active object ............................. */
struct PulseCounter
{
    RKH_SM_T sm;        /* base class */
    TimeEvt tactMin;    /* timer tactMin */
    TimeEvt tactMax;    /* timer tactMax */
    TimeEvt tinactMax;  /* timer tinactMax */
    uint32_t nPulses;   /* amount of detected pulses */
    int id;             /* identification */
    LightMgr *theLightMgr; /* reference to its own container */
};

struct LightMgr
{
    RKH_SMA_T sma;  /* base class  */
    OnOff onOff;    /* orthogonal region */ 
    Mode mode;      /* orthogonal region */ 
};

RKH_SMA_CREATE(LightMgr, pulseCounterMgr, LightMgrPrio, HCAL, 
               &Processing, LightMgr_init, NULL);
RKH_SMA_DEF_PTR(pulseCounterMgr);
RKH_SM_CONST_CREATE(pulseCounter,       /* name of parameterized SM */
                    1,
                    HCAL,               /* it is a hierarchical SM */
                    &PulseCounter_Idle, /* default state */
                    PulseCounter_init,  /* topmost initial action */
                    NULL);

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ............................ Effect actions ............................. */
void 
LightMgr_init(LightMgr *const me, RKH_EVT_T *pe)
{
    int i;
    PulseCounter *pulseCtr;

	RKH_TR_FWK_AO(me);
	RKH_TR_FWK_QUEUE(&RKH_UPCAST(RKH_SMA_T, me)->equeue);
	RKH_TR_FWK_STATE(me, &Processing);
	RKH_TR_FWK_SIG(evInactive);
	RKH_TR_FWK_SIG(evActive);
	RKH_TR_FWK_SIG(evTactMinTout);
	RKH_TR_FWK_SIG(evTactMaxTout);
	RKH_TR_FWK_SIG(evTinactMaxTout);

	RKH_TR_FWK_STATE(me, &PulseCounter_Idle);
	RKH_TR_FWK_STATE(me, &PulseCounter_Setup);
	RKH_TR_FWK_STATE(me, &PulseCounter_Active);
	RKH_TR_FWK_STATE(me, &PulseCounter_Inactive);
	RKH_TR_FWK_STATE(me, &PulseCounter_WaitInactive);
	RKH_TR_FWK_SIG(evInactive);
	RKH_TR_FWK_SIG(evActive);

    for (pulseCtr = &me->pulseCounters[0], i = 0; 
         i < NUM_PULSE_COUNTERS; 
         ++i, ++pulseCtr)
    {
        rkh_sm_init(RKH_UPCAST(RKH_SM_T, pulseCtr));
    }
}

void 
LightMgr_dispatchStatus(LightMgr *const me, RKH_EVT_T *pe)
{
    int ix;

    ix = RKH_DOWNCAST(StatusEvt, pe)->id;
    RKH_REQUIRE(ix <= NUM_PULSE_COUNTERS);

    rkh_sm_dispatch(RKH_DOWNCAST(RKH_SM_T, &me->pulseCounters[ix]), pe);
}

void 
LightMgr_dispatchTout(LightMgr *const me, RKH_EVT_T *pe)
{
    int ix;

    ix = RKH_DOWNCAST(TimeEvt, pe)->id;
    RKH_REQUIRE(ix <= NUM_PULSE_COUNTERS);

    rkh_sm_dispatch(RKH_DOWNCAST(RKH_SM_T, &me->pulseCounters[ix]), pe);
}

void 
PulseCounter_init(PulseCounter *const me, RKH_EVT_T *pe)
{
    char name[16];

    sprintf(name, "pc%d", me->id);
	RKH_TR_FWK_ACTOR(me, name);
    sprintf(name, "tactMin%d", me->id);
	RKH_TR_FWK_OBJ_NAME(&me->tactMin.evt.tmr, name);
    sprintf(name, "tactMax%d", me->id);
	RKH_TR_FWK_OBJ_NAME(&me->tactMax.evt.tmr, name);
    sprintf(name, "tinactMax%d", me->id);
	RKH_TR_FWK_OBJ_NAME(&me->tinactMax.evt.tmr, name);
}

void 
PulseCounter_reset(PulseCounter *const me, RKH_EVT_T *pe)
{
    me->nPulses = 0;
}

void 
PulseCounter_pulse(PulseCounter *const me, RKH_EVT_T *pe)
{
    ++me->nPulses;
}

void 
PulseCounter_register(PulseCounter *const me, RKH_EVT_T *pe)
{
    bsp_register(me->id, me->nPulses);
}

/* ............................. Entry actions ............................. */
void 
PulseCounter_enSetup(PulseCounter *const me)
{
    RKH_TMR_T *tmr = &me->tactMin.evt.tmr;

	RKH_SET_STATIC_EVENT(&me->tactMin, evTactMinTout);
	RKH_TMR_INIT(tmr, RKH_UPCAST(RKH_EVT_T, &me->tactMin), NULL);
	RKH_TMR_ONESHOT(tmr, 
                    RKH_UPCAST(RKH_SMA_T, me->theLightMgr), 
                    ACT_MIN_TIME);
}

void 
PulseCounter_enActive(PulseCounter *const me)
{
    RKH_TMR_T *tmr = &me->tactMax.evt.tmr;

	RKH_SET_STATIC_EVENT(&me->tactMax, evTactMaxTout);
	RKH_TMR_INIT(tmr, RKH_UPCAST(RKH_EVT_T, &me->tactMax), NULL);
	RKH_TMR_ONESHOT(tmr, 
                    RKH_UPCAST(RKH_SMA_T, me->theLightMgr), 
                    ACT_MAX_TIME);
}

void 
PulseCounter_enInactive(PulseCounter *const me)
{
    RKH_TMR_T *tmr = &me->tinactMax.evt.tmr;

	RKH_SET_STATIC_EVENT(&me->tinactMax, evTinactMaxTout);
	RKH_TMR_INIT(tmr, RKH_UPCAST(RKH_EVT_T, &me->tinactMax), NULL);
	RKH_TMR_ONESHOT(tmr, 
                    RKH_UPCAST(RKH_SMA_T, me->theLightMgr), 
                    INACT_MAX_TIME);
}

/* ............................. Exit actions .............................. */
void 
PulseCounter_exSetup(PulseCounter *const me)
{
	rkh_tmr_stop(&me->tactMin.evt.tmr);
}

void 
PulseCounter_exActive(PulseCounter *const me)
{
	rkh_tmr_stop(&me->tactMax.evt.tmr);
}

void 
PulseCounter_exInactive(PulseCounter *const me)
{
	rkh_tmr_stop(&me->tinactMax.evt.tmr);
}

/* ................................ Guards ................................. */
/* ---------------------------- Global functions --------------------------- */
void
LightMgr_ctor(void)
{
    int i;
    LightMgr *me = RKH_DOWNCAST(LightMgr, pulseCounterMgr);
    PulseCounter *pulseCtr;

    for (i = 0; i < NUM_PULSE_COUNTERS; ++i)
    {
        pulseCtr = &me->pulseCounters[i];
        pulseCtr->id = i;
        pulseCtr->nPulses = 0;
        pulseCtr->tactMin.id = i;
        pulseCtr->tactMax.id = i;
        pulseCtr->tinactMax.id = i;
        pulseCtr->theLightMgr = me;
        RKH_SM_INIT(pulseCtr,     /* Instance of SM component */
                    pulseCounter, /* Complete next parameters with the */
                    1,            /* same values used in the macro */
                    HCAL,         /* RKH_SM_CONST_CREATE() */
                    &PulseCounter_Idle,
                    PulseCounter_init,
                    NULL);
    }
}

/* ------------------------------ End of file ------------------------------ */
