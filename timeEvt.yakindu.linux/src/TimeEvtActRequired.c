/**
 *  \file       Example2ActRequired.c
 *  \brief      This file implements the required operations for Example2 
 *              active object.
 */

/* -------------------------- Development history -------------------------- */
/*
 */

/* -------------------------------- Authors -------------------------------- */
/*
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include "rkhtrc_record.h"
#include "signals.h"
#include "TimeEvt.h"
#include "bsp.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void 
init(void)
{
    RKH_TR_FWK_SIG(evTout0);
    RKH_TR_FWK_SIG(evTout1);
    RKH_TR_FWK_SIG(evTout2);
    RKH_TR_FWK_SIG(evTout3);
    RKH_TR_FWK_SIG(evStart);
    RKH_TR_FWK_SIG(evPause);
    RKH_TR_FWK_SIG(evResume);
    RKH_TR_FWK_SIG(evReset);
}

void 
ledOn(void)
{
    bsp_ledOn();
}

void 
ledOff(void)
{
    bsp_ledOff();
}

void
ledToggle(void)
{
    bsp_ledToggle();
}

/* ------------------------------ End of file ------------------------------ */
