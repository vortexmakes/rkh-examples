/**
 *  \file       events.h
 *  \brief      Event specification.
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
/* --------------------------------- Module -------------------------------- */
#ifndef __EVENTS_H__
#define __EVENTS_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkhevt.h"
#include "rkhtmr.h"

/* ---------------------- External C language linkage ---------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ................................ Events ................................ */
typedef struct StatusEvt StatusEvt;
struct StatusEvt
{
    RKH_EVT_T evt;  /* signal event */
    int id;         /* SM component identifier */
};

typedef struct TimeEvt TimeEvt;
struct TimeEvt
{
    RKHTmEvt evt;   /* time event */
    int id;         /* SM component identifier */
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
