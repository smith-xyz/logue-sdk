/*
 * File: wizz.cpp
 *
 * Naive wizz oscillator
 *
 */

#include "userosc.h"
#include "wizz.hpp"
typedef __uint32_t uint32_t;

static Harm s_harm;

void OSC_INIT(uint32_t platform, uint32_t api)
    {
        (void)platform;
        (void)api;
    }

void OSC_CYCLE(const user_osc_param_t *const params,
               int32_t *yn,
               const uint32_t frames)
    {
        Harm::State &s = s_harm.state;
        const Harm::Params &p = s_harm.params;

        q31_t *__restrict y = (q31_t *)yn;
        const q31_t *y_e = y + frames;

        while(s_harm.state.harm > 0) {
            const int &h = Harm::k_flag_harm;
            const int calc = 2 * (sin(*y_e) / h);
            s_harm.updateHarm(s_harm.k_flag_harm);
            s_harm.state.harm--;
        }
    }

void OSC_NOTEON(const user_osc_param_t *const params)
    {
        s_harm.state.flags |= Harm::k_flag_reset;
    }

void OSC_NOTEOFF(const user_osc_param_t *const params)
    {
        (void)params;
    }

void OSC_PARAM(uint16_t index, uint16_t value)
    {
        Harm::Params &p = s_harm.params;
        Harm::State &s = s_harm.state;

        switch (index)
        {
        case k_user_osc_param_id1:
            // harmonics
            // select parameter
            {
                static const uint8_t cnt = k_waves_a_cnt + k_waves_b_cnt + k_waves_c_cnt;
                p.harm = value % cnt;
                s.flags |= Harm::k_flag_harm;
            }
            break;
            
            default:
            break;
    }