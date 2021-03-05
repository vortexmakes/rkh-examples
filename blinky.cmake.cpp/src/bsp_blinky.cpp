/**
 *  \file       bsp_blinky.cpp
 *  \brief      Blinky BSP for C++ on Linux x86
 */

/* -------------------------- Development history -------------------------- */
/* -------------------------------- Authors -------------------------------- */
/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <iostream>
#include "blinky.h"
#include "bsp.h"
#include "rkh.h"
#include "trace_io_cfg.h"

RKH_THIS_MODULE

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
#define ESC         0x1B

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
extern RKH_SMA_T *blinky;

/* ---------------------------- Local variables ---------------------------- */
Bsp* Bsp::pInstance = nullptr;
const RKH_EVT_T Bsp::evTerm = {Blinky::evTerminate, 0, 0};

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
void
bsp_keyParser(int c)
{
    if (c == ESC)
    {
        RKH_SMA_POST_FIFO(blinky, &Bsp::evTerm, 0);
        rkhport_fwk_stop();
    }
}

void
bsp_timeTick(void)
{
}

Bsp::Bsp(int argc, char *argv[])
{
    std::cout << "Blinky is the 'hello world' of RKH programming!\n\n";
    std::cout << "RKH version      = " << RKH_RELEASE << std::endl;
    std::cout << "Port version     = " << rkhport_get_version() << std::endl;
    std::cout << "Port description = " << rkhport_get_desc() << std::endl;
    std::cout << "\nDescription:\n" <<
        "The goal of this demo application is to explain how to \n" <<
        "represent a \"flat\" state machine and how to use the timer \n" <<
        "services by using the RKH framework. To do that is proposed a \n" <<
        "very simple demo that uses one state machine and one timer, \n" <<
        "which is shown and explained in the reference manual section \n" <<
        "\"Examples\".\n\n";
    std::cout << "Press ESC to quit \n\n\n";

    trace_io_setConfig(argc, argv);
    rkh_fwk_init();
    RKH_FILTER_ON_GROUP(RKH_TRC_ALL_GROUPS);
    RKH_FILTER_ON_EVENT(RKH_TRC_ALL_EVENTS);
    RKH_FILTER_OFF_EVENT(RKH_TE_TMR_TOUT);
    RKH_FILTER_OFF_EVENT(RKH_TE_SM_STATE);
    RKH_FILTER_OFF_SMA(blinky);
    RKH_FILTER_OFF_ALL_SIGNALS();
    RKH_TRC_OPEN();
}

Bsp *
Bsp::getInstance(int argc, char *argv[])
{
    if (pInstance == nullptr)
    {
        pInstance = new Bsp(argc, argv);
    }
    return pInstance;
}

void
Bsp::ledOn() const
{
    std::cout << "LED ON" << std::endl;
}

void
Bsp::ledOff() const
{
    std::cout << "LED OFF" << std::endl;
}

/* ------------------------------ File footer ------------------------------ */
