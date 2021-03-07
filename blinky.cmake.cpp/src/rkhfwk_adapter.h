/**
 *  \file       rkhfwk_adapter.h
 *  \brief      It files contains useful templates and classes for using RKH 
 *              in C++ applications.
 */

/* -------------------------- Development history -------------------------- */
/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  lf@vortexmakes.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __RKHFWK_ADAPTER_H__
#define __RKHFWK_ADAPTER_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"

/* --------------------------------- Macros -------------------------------- */
#define sm_cast(actObj) \
   reinterpret_cast<RKH_SM_T*>(static_cast<RKH_SMA_T*>(actObj))

#define st_cast(state) \
    reinterpret_cast<const RKH_ST_T*>(state)

#define act_cast(action) \
    reinterpret_cast<RKH_TRN_ACT_T>(action)

inline const char *
str_cast(std::string s)
{
    return s.c_str();
}

#if ( RKH_CFG_TRC_EN == RKH_ENABLED && \
     (RKH_CFG_TRC_ALL_EN == RKH_ENABLED || \
      RKH_CFG_TRC_SMA_EN == RKH_ENABLED || \
      RKH_CFG_TRC_SM_EN == RKH_ENABLED || \
      RKH_CFG_TRC_FWK_EN == RKH_ENABLED))
    #define makeSmName(actObj_, name_) \
        sm_cast(actObj_)->name = name_
#else
    #define makeSmName(actObj_, name_)
#endif

#define RKHActObjInit(actObj, name_, prio_, initialState_, initialAction_) \
    sm_cast(actObj)->prio = prio_; \
    sm_cast(actObj)->ppty = HCAL; \
    makeSmName(actObj, name_); \
    sm_cast(actObj)->istate = st_cast(initialState_); \
    sm_cast(actObj)->iaction = act_cast(initialAction_); \
    sm_cast(actObj)->state = st_cast(initialState_)

/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
