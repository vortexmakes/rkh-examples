/**
 *  \file       events.h
 *  \brief      Event specification.
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
/* --------------------------------- Module -------------------------------- */
#ifndef __EVENTS_H__
#define __EVENTS_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkhevt.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ................................ Events ................................ */
typedef struct ModeEvt ModeEvt;
struct ModeEvt
{
    RKH_EVT_T evt;      /* signal event */
    uint16_t onTime;    /* On time */
    uint16_t offTime;   /* Off time */
};

/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
/* -------------------- External C language linkage end -------------------- */
#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
