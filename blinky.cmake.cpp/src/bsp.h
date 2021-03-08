/**
 *  \file       bsp.h
 *  \brief      Blinky BSP for Linux x86
 */

/* -------------------------- Development history -------------------------- */
/* -------------------------------- Authors -------------------------------- */
/* --------------------------------- Module -------------------------------- */
#ifndef __BSP_H__
#define __BSP_H__

/* ----------------------------- Include files ----------------------------- */
#include "bsp_common.h"

/* ---------------------- External C language linkage ---------------------- */
/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
class Bsp
{
    static Bsp* pInstance;
    static const RKH_EVT_T evTerm;
    friend void bsp_keyParser(int c);
    friend void bsp_timeTick(void);
    protected:
        Bsp(int argc, char* argv[]);
        ~Bsp() {};
    public:
        Bsp(Bsp&) = delete;
        void operator=(const Bsp&) = delete;
        static Bsp* getInstance(int argc, char* argv[]);
        void ledOn() const;
        void ledOff() const;
};

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
/* -------------------- External C language linkage end -------------------- */
/* ------------------------------ Module end ------------------------------- */
#endif

/* ------------------------------ File footer ------------------------------ */
