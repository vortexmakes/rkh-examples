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
#include "LightMgr.h"

/* ----------------------------- Local macros ------------------------------ */
RKH_MODULE_NAME(LightMgr)
#define WaitTime0	RKH_TIME_SEC(5)
#define WaitTime1	RKH_TIME_SEC(5)
#define WaitTime2	RKH_TIME_SEC(5)

/* ......................... Declares active object ........................ */
typedef struct LightMgr LightMgr;
typedef struct Mode Mode;
typedef struct Rate Rate;

/* ................... Declares states and pseudostates .................... */
RKH_DCLR_BASIC_STATE LightMgr_Red, LightMgr_Yelow, LightMgr_Green, 
                     LightMgr_WaitToStart;
RKH_DCLR_CHOICE_STATE LightMgr_C0;
RKH_DCLR_BASIC_STATE Mode_OneCycle, Mode_Cycled;
RKH_DCLR_BASIC_STATE Rate_Steady, Rate_FlashSlowly, Rate_FlashQuickly;

/* ........................ Declares effect actions ........................ */
static void LightMgr_init(LightMgr *const me, RKH_EVT_T *pe);
static void Mode_init(Mode *const me, RKH_EVT_T *pe);
static void Rate_init(Rate *const me, RKH_EVT_T *pe);

/* ......................... Declares entry actions ........................ */
static void LightMgr_enRed(LightMgr *const me);
static void LightMgr_enYelow(LightMgr *const me);
static void LightMgr_enGreen(LightMgr *const me);
static void Mode_enCycle(Mode *const me);

/* ......................... Declares exit actions ......................... */
static void LightMgr_exRed(LightMgr *const me);
static void LightMgr_exYelow(LightMgr *const me);
static void LightMgr_exGreen(LightMgr *const me);

/* ............................ Declares guards ............................ */
static rbool_t LightMgr_isInOneCycle(LightMgr *const me, RKH_EVT_T *pe);

/* ........................ States and pseudostates ........................ */
RKH_CREATE_BASIC_STATE(LightMgr_Red, LightMgr_enRed, LightMgr_exRed, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(LightMgr_Red)
	RKH_TRREG(evTout0, NULL, NULL, &LightMgr_Yelow),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(LightMgr_Yelow, LightMgr_enYelow, LightMgr_exYelow, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(LightMgr_Yelow)
	RKH_TRREG(evTout1, NULL, NULL, &LightMgr_Green),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(LightMgr_Green, LightMgr_enGreen, LightMgr_exGreen, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(LightMgr_Green)
	RKH_TRREG(evTout2, NULL, NULL, &LightMgr_C0),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(LightMgr_WaitToStart, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(LightMgr_WaitToStart)
	RKH_TRREG(evStart, NULL, NULL, &LightMgr_Red),
RKH_END_TRANS_TABLE

RKH_CREATE_CHOICE_STATE(LightMgr_C0);
RKH_CREATE_BRANCH_TABLE(LightMgr_C0)
	RKH_BRANCH(LightMgr_isInOneCycle, NULL, &LightMgr_Red),
	RKH_BRANCH(ELSE, NULL, &LightMgr_WaitToStart),
RKH_END_BRANCH_TABLE

RKH_CREATE_BASIC_STATE(Mode_OneCycle, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(Mode_OneCycle)
	RKH_TRREG(evMode, NULL, NULL, &Mode_Cycled),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(Mode_Cycled, Mode_enCycle, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(Mode_Cycled)
	RKH_TRREG(evMode, NULL, NULL, &Mode_OneCycle),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(Rate_Steady, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(Rate_Steady)
	RKH_TRREG(evRate, NULL, NULL, &Rate_FlashSlowly),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(Rate_FlashSlowly, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(Rate_FlashSlowly)
	RKH_TRREG(evRate, NULL, NULL, &Rate_FlashQuickly),
RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(Rate_FlashQuickly, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(Rate_FlashQuickly)
	RKH_TRREG(evRate, NULL, NULL, &Rate_Steady),
RKH_END_TRANS_TABLE

/* ............................. Active object ............................. */
struct Mode
{
    RKH_SM_T sm;        /* base class */
};

struct Rate
{
    RKH_SM_T sm;        /* base class */
};

struct LightMgr
{
    RKH_SMA_T sma;      /* base class  */
    RKHSmaVtbl vtbl;    /* virtual table */
    RKHTmEvt tmEvtObj0;
    RKHTmEvt tmEvtObj1;
    RKHTmEvt tmEvtObj2;
    Mode mode;          /* orthogonal region */ 
    Rate rate;          /* orthogonal region */ 
};

RKH_SMA_CREATE(LightMgr, 
               lightMgr, 
               LightMgrPrio, 
               HCAL, 
               &LightMgr_Red, 
               LightMgr_init, 
               NULL);
RKH_SMA_DEF_PTR(lightMgr);

RKH_SM_CONST_CREATE(rate,           /* name of parameterized SM */
                    1,
                    HCAL,           /* it is a hierarchical SM */
                    &Rate_Steady,   /* default state */
                    Rate_init,      /* topmost initial action */
                    NULL);
RKH_SM_CONST_CREATE(mode,           /* name of parameterized SM */
                    2,
                    HCAL,           /* it is a hierarchical SM */
                    &Mode_OneCycle, /* default state */
                    Mode_init,      /* topmost initial action */
                    NULL);

/* ------------------------------- Constants ------------------------------- */
static RKH_ROM_STATIC_EVENT(evStartObj, evStart);

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void
dispatch(RKH_SMA_T *me, void *arg)
{
    LightMgr *realMe;

    realMe = RKH_DOWNCAST(LightMgr, me);
    rkh_sm_dispatch(RKH_UPCAST(RKH_SM_T, realMe), (RKH_EVT_T *)arg);
    rkh_sm_dispatch(RKH_UPCAST(RKH_SM_T, &realMe->mode), (RKH_EVT_T *)arg);
    rkh_sm_dispatch(RKH_UPCAST(RKH_SM_T, &realMe->rate), (RKH_EVT_T *)arg);
}

/* ............................ Effect actions ............................. */
static void 
LightMgr_init(LightMgr *const me, RKH_EVT_T *pe)
{
	/* init(); */
	RKH_TR_FWK_AO(me);
	RKH_TR_FWK_QUEUE(&RKH_UPCAST(RKH_SMA_T, me)->equeue);
	RKH_TR_FWK_STATE(me, &LightMgr_Red);
	RKH_TR_FWK_STATE(me, &LightMgr_Yelow);
	RKH_TR_FWK_STATE(me, &LightMgr_Green);
	RKH_TR_FWK_STATE(me, &LightMgr_WaitToStart);
	RKH_TR_FWK_SIG(evStart);
	RKH_TR_FWK_SIG(evTout0);
	RKH_TR_FWK_SIG(evTout1);
	RKH_TR_FWK_SIG(evTout2);
	RKH_TR_FWK_TIMER(&me->tmEvtObj0.tmr);
	RKH_TR_FWK_TIMER(&me->tmEvtObj1.tmr);
	RKH_TR_FWK_TIMER(&me->tmEvtObj2.tmr);

    rkh_sm_init(RKH_UPCAST(RKH_SM_T, &me->mode));
    rkh_sm_init(RKH_UPCAST(RKH_SM_T, &me->rate));
}

static void 
Mode_init(Mode *const me, RKH_EVT_T *pe)
{
    RKH_TR_FWK_OBJ_NAME(me, "Mode");
	RKH_TR_FWK_STATE(me, &Mode_OneCycle);
	RKH_TR_FWK_STATE(me, &Mode_Cycled);
	RKH_TR_FWK_SIG(evMode);
}

static void 
Rate_init(Rate *const me, RKH_EVT_T *pe)
{
    RKH_TR_FWK_OBJ_NAME(me, "Rate");
	RKH_TR_FWK_STATE(me, &Rate_Steady);
	RKH_TR_FWK_STATE(me, &Rate_FlashSlowly);
	RKH_TR_FWK_STATE(me, &Rate_FlashQuickly);
	RKH_TR_FWK_SIG(evRate);
}

/* ............................. Entry actions ............................. */
static void 
LightMgr_enRed(LightMgr *const me)
{
	RKH_SET_STATIC_EVENT(&me->tmEvtObj0, evTout0);
	RKH_TMR_INIT(&me->tmEvtObj0.tmr, 
                 RKH_UPCAST(RKH_EVT_T, 
                 &me->tmEvtObj0), 
                 NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj0.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime0);
    bsp_turnOnRedLight();
}

static void 
LightMgr_enYelow(LightMgr *const me)
{
	RKH_SET_STATIC_EVENT(&me->tmEvtObj1, evTout1);
	RKH_TMR_INIT(&me->tmEvtObj1.tmr, 
                 RKH_UPCAST(RKH_EVT_T, 
                 &me->tmEvtObj1), 
                 NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj1.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime1);
    bsp_turnOnYellowLight();
}

static void 
LightMgr_enGreen(LightMgr *const me)
{
	RKH_SET_STATIC_EVENT(&me->tmEvtObj2, evTout2);
	RKH_TMR_INIT(&me->tmEvtObj2.tmr, 
                 RKH_UPCAST(RKH_EVT_T, 
                 &me->tmEvtObj2), 
                 NULL);
	RKH_TMR_ONESHOT(&me->tmEvtObj2.tmr, RKH_UPCAST(RKH_SMA_T, me), WaitTime2);
    bsp_turnOnGreenLight();
}

void 
Mode_enCycle(Mode *const me)
{
	/*startCycle();*/
    RKH_SMA_POST_LIFO(lightMgr, &evStartObj, NULL);
}

/* ............................. Exit actions .............................. */
static void 
LightMgr_exRed(LightMgr *const me)
{
	rkh_tmr_stop(&me->tmEvtObj0.tmr);
}

static void 
LightMgr_exYelow(LightMgr *const me)
{
	rkh_tmr_stop(&me->tmEvtObj1.tmr);
}

static void 
LightMgr_exGreen(LightMgr *const me)
{
	rkh_tmr_stop(&me->tmEvtObj2.tmr);
}

/* ................................ Guards ................................. */
static rbool_t 
LightMgr_isInOneCycle(LightMgr *const me, RKH_EVT_T *pe)
{
    /*isInOneCycle()*/
	return (RKH_UPCAST(RKH_SM_T, &me->mode)->state == &Mode_OneCycle) ? 
            false : true;
}

/* ---------------------------- Global functions --------------------------- */
void
LightMgr_ctor(void)
{
    LightMgr *me = RKH_DOWNCAST(LightMgr, lightMgr);

    me->vtbl = rkhSmaVtbl;      /* Initialize AO's virtual table and */
    me->vtbl.task = dispatch;   /* override its dispatch operation */
    rkh_sma_ctor(RKH_UPCAST(RKH_SMA_T, me), &me->vtbl);
    RKH_SM_INIT(&me->rate,      /* Instance of SM component */
                rate,           /* Complete next parameters with the */
                1,              /* same values used in the macro */
                HCAL,           /* RKH_SM_CONST_CREATE() */
                &Rate_Steady,
                Rate_init,
                NULL);
    RKH_SM_INIT(&me->mode,      /* Instance of SM component */
                mode,           /* Complete next parameters with the */
                2,              /* same values used in the macro */
                HCAL,           /* RKH_SM_CONST_CREATE() */
                &Mode_OneCycle,
                Mode_init,
                NULL);
}

/* ------------------------------ End of file ------------------------------ */
