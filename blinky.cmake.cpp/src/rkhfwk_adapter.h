/**
 *  \file       rkhfwk_adapter.h
 *  \brief      It files contains useful resources for using RKH in 
 *              C++ applications.
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
#include <string>

/* --------------------------------- Macros -------------------------------- */
using ActObjName = std::string;

#define sm_cast(actObj) \
    reinterpret_cast<RKH_SM_T*>(static_cast<RKH_SMA_T*>(actObj))

#define st_cast(state) \
    reinterpret_cast<const RKH_ST_T*>(state)

#define act_cast(action) \
    reinterpret_cast<RKH_TRN_ACT_T>(action)

inline void 
makeSmName(RKH_SM_T* sm, ActObjName& name)
{
#if ( RKH_CFG_TRC_EN == RKH_ENABLED && \
     (RKH_CFG_TRC_ALL_EN == RKH_ENABLED || \
      RKH_CFG_TRC_SMA_EN == RKH_ENABLED || \
      RKH_CFG_TRC_SM_EN == RKH_ENABLED || \
      RKH_CFG_TRC_FWK_EN == RKH_ENABLED))
    sm->name = name.c_str();
#endif
}

#define RKHActObjInit(actObj, name_, prio_, initialState_, initialAction_) \
    RKH_SM_T* sm = sm_cast(actObj); \
    sm->prio = prio_; \
    sm->ppty = HCAL; \
    sm->istate = st_cast(initialState_); \
    sm->iaction = act_cast(initialAction_); \
    sm->state = st_cast(initialState_); \
    makeSmName(sm, name_)

/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ End of file ------------------------------ */
