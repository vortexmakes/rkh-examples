/**
 *  \file       PulseCounterMgr.c
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2020.06.06  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "rkhsm.h"
#include "rkhsma.h"
#include "rkhassert.h"
#include "signals.h"
#include "events.h"
#include "priorities.h"
#include "PulseCounterMgr.h"

/* ----------------------------- Local macros ------------------------------ */
RKH_MODULE_NAME(PulseCounterMgr)

/* ......................... Declares active object ........................ */
typedef struct PulseCounter PulseCounter;
typedef struct PulseCounterMgr PulseCounterMgr;

/* ................... Declares states and pseudostates .................... */
RKH_DCLR_BASIC_STATE Processing;
RKH_DCLR_BASIC_STATE PulseCounter_Idle, PulseCounter_Setup, 
                     PulseCounter_Active, PulseCounter_Inactive, 
                     PulseCounter_WaitInactive;

/* ........................ Declares effect actions ........................ */
static void PulseCounterMgr_init(PulseCounterMgr *const me, RKH_EVT_T *pe);
static void PulseCounterMgr_dispatchStatus(PulseCounterMgr *const me, 
                                           RKH_EVT_T *pe);
static void PulseCounterMgr_dispatchTout(PulseCounterMgr *const me, 
                                         RKH_EVT_T *pe);
void PulseCounter_init(PulseCounter *const me, RKH_EVT_T *pe);
void PulseCounter_reset(PulseCounter *const me, RKH_EVT_T *pe);
void PulseCounter_pulse(PulseCounter *const me, RKH_EVT_T *pe);
void PulseCounter_register(PulseCounter *const me, RKH_EVT_T *pe);

/* ......................... Declares entry actions ........................ */
void PulseCounter_enSetup(PulseCounter *const me);
void PulseCounter_enActive(PulseCounter *const me);
void PulseCounter_enInactive(PulseCounter *const me);

/* ......................... Declares exit actions ......................... */
void PulseCounter_exSetup(PulseCounter *const me);
void PulseCounter_exActive(PulseCounter *const me);
void PulseCounter_exInactive(PulseCounter *const me);

/* ............................ Declares guards ............................ */
/* ........................ States and pseudostates ........................ */
RKH_CREATE_BASIC_STATE(Processing, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(Processing)
	RKH_TRINT(evInactive, NULL, PulseCounterMgr_dispatchStatus),
	RKH_TRINT(evActive, NULL, PulseCounterMgr_dispatchStatus),
	RKH_TRINT(evTactMinTout, NULL, PulseCounterMgr_dispatchTout),
	RKH_TRINT(evTactMaxTout, NULL, PulseCounterMgr_dispatchTout),
	RKH_TRINT(evTinactMaxTout, NULL, PulseCounterMgr_dispatchTout),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(PulseCounter_Idle, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(PulseCounter_Idle)
	RKH_TRREG(evInactive, NULL, NULL, &PulseCounter_Setup),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(PulseCounter_Setup, PulseCounter_enSetup, 
                       PulseCounter_exSetup, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(PulseCounter_Setup)
	RKH_TRREG(evTactMinTout, NULL, NULL, &PulseCounter_Active),
	RKH_TRREG(evActive, NULL, PulseCounter_reset, &PulseCounter_Idle),
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
    RKH_SM_T sm;        /* base structure */
    RKHTmEvt tmEvtObj0;
    RKHTmEvt tmEvtObj1;
    RKHTmEvt tmEvtObj2;
    uint32_t nPulses;
};

struct PulseCounterMgr
{
    RKH_SMA_T sma;      /* base structure */
    PulseCounter pulseCounters[NUM_PULSE_COUNTERS];
};

RKH_SMA_CREATE(PulseCounterMgr, pulseCounterMgr, PulseCounterMgrPrio, HCAL, 
               &Processing, PulseCounterMgr_init, NULL);
RKH_SMA_DEF_PTR(pulseCounterMgr);
RKH_SM_CONST_CREATE(pulseCounter, 1, HCAL, &PulseCounter_Idle, 
                    PulseCounter_init, NULL);

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ............................ Initial action ............................. */
/* ............................ Effect actions ............................. */
void 
PulseCounterMgr_init(PulseCounterMgr *const me, RKH_EVT_T *pe)
{
	RKH_TR_FWK_AO(me);
	RKH_TR_FWK_QUEUE(&RKH_UPCAST(RKH_SMA_T, me)->equeue);
	RKH_TR_FWK_STATE(me, &Processing);
	RKH_TR_FWK_SIG(evInactive);
	RKH_TR_FWK_SIG(evActive);
	RKH_TR_FWK_SIG(evTactMinTout);
	RKH_TR_FWK_SIG(evTactMaxTout);
	RKH_TR_FWK_SIG(evTinactMaxTout);
}

void 
PulseCounterMgr_dispatchStatus(PulseCounterMgr *const me, RKH_EVT_T *pe)
{
    int ix;

    ix = RKH_DOWNCAST(StatusEvt, pe)->index;
    RKH_REQUIRE(ix <= NUM_PULSE_COUNTERS);

    rkh_sm_dispatch(RKH_DOWNCAST(RKH_SM_T, &me->pulseCounters[ix]), pe);
}

void 
PulseCounterMgr_dispatchTout(PulseCounterMgr *const me, RKH_EVT_T *pe)
{
    int ix;

    ix = RKH_DOWNCAST(TimeEvt, pe)->index;
    RKH_REQUIRE(ix <= NUM_PULSE_COUNTERS);

    rkh_sm_dispatch(RKH_DOWNCAST(RKH_SM_T, &me->pulseCounters[ix]), pe);
}

void 
PulseCounter_init(PulseCounter *const me, RKH_EVT_T *pe)
{
}

void 
PulseCounter_reset(PulseCounter *const me, RKH_EVT_T *pe)
{
}

void 
PulseCounter_pulse(PulseCounter *const me, RKH_EVT_T *pe)
{
}

void 
PulseCounter_register(PulseCounter *const me, RKH_EVT_T *pe)
{
}

/* ............................. Entry actions ............................. */
void 
PulseCounter_enSetup(PulseCounter *const me)
{
}

void 
PulseCounter_enActive(PulseCounter *const me)
{
}

void 
PulseCounter_enInactive(PulseCounter *const me)
{
}

/* ............................. Exit actions .............................. */
void 
PulseCounter_exSetup(PulseCounter *const me)
{
}

void 
PulseCounter_exActive(PulseCounter *const me)
{
}

void 
PulseCounter_exInactive(PulseCounter *const me)
{
}

/* ................................ Guards ................................. */
/* ---------------------------- Global functions --------------------------- */
void
PulseCounterMgr_ctor(void)
{
    int i;
    PulseCounterMgr *me = RKH_DOWNCAST(PulseCounterMgr, pulseCounterMgr);

    /*rkh_sma_ctor(RKH_UPCAST(RKH_SMA_T, me), &me->vtbl);*/
    for (i = 0; i < NUM_PULSE_COUNTERS; ++i)
    {
        RKH_SM_INIT(&me->pulseCounters[i], pulseCounter, 1, HCAL, 
                    &PulseCounter_Idle, PulseCounter_init, NULL);
    }
}

/* ------------------------------ End of file ------------------------------ */
