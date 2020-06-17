/**
 *  \file       main.c
 *  \brief      Example application.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2020.06.06  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <LightMgr.h>
#include "rkh.h"
#include "bsp.h"

/* ----------------------------- Local macros ------------------------------ */
#define QSTO_SIZE           4

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
static RKH_EVT_T *qsto[QSTO_SIZE];

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void
setupTraceFilters(void)
{
    RKH_FILTER_ON_GROUP(RKH_TRC_ALL_GROUPS);
    RKH_FILTER_ON_EVENT(RKH_TRC_ALL_EVENTS);
    RKH_FILTER_OFF_GROUP_ALL_EVENTS(RKH_TG_USR);
    RKH_FILTER_OFF_EVENT(RKH_TE_TMR_TOUT);
    RKH_FILTER_OFF_EVENT(RKH_TE_SM_STATE);
    /*RKH_FILTER_OFF_EVENT(RKH_TE_SMA_FIFO);*/
    /*RKH_FILTER_OFF_EVENT(RKH_TE_SMA_LIFO);*/
    /*RKH_FILTER_OFF_EVENT(RKH_TE_SM_TS_STATE);*/
    RKH_FILTER_OFF_EVENT(RKH_TE_SM_DCH);
    /*RKH_FILTER_OFF_SMA(pulseCounterMgr);*/
    RKH_FILTER_OFF_ALL_SMA();
    RKH_FILTER_OFF_ALL_SIGNALS();
}

/* ---------------------------- Global functions --------------------------- */
int
main(int argc, char *argv[])
{
    bsp_init(argc, argv);
    rkh_fwk_init();
    setupTraceFilters();

    RKH_TRC_OPEN();
    LightMgr_ctor();
    RKH_SMA_ACTIVATE(pulseCounterMgr, qsto, QSTO_SIZE, 0, 0);
    rkh_fwk_enter();
    RKH_TRC_CLOSE();

    return 0;
}

/* ------------------------------ End of file ------------------------------ */
