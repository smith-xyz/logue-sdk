#pragma once
/*
 *  File: wizz.hpp
 *
 *  Updating harmonics parameter
 *
 */

#include "userosc.h"
#include "biquad.hpp"
typedef __uint32_t uint32_t;

struct Harm
{

    enum
    {
        k_flag_none = 0,
        k_flag_harm = 1 << 15,
        k_flag_reset = 1 << 6
    };

    struct Params
    {
        uint8_t harm;

        Params(void) : harm(0)
        { }
    };

    struct State
    {
        const float *harm;
        float imperfection;
        uint32_t flags : 8;

        State(void) : harm(wavesA[0]),
                      flags(k_flags_none)
        {
            imperfection = osc_white() * 1.0417e-006f; // +/- 0.05Hz@48KHz
        }
    };

    Harm(void)
    {
        init();
    }

    void init(void)
    {
        state = State();
        params = Params();
        prelpf.mCoeffs.setPoleLP(0.8f);
        postlpf.mCoeffs.setFOLP(osc_tanpif(0.45f));
    }

    inline void updatePitch(float w0)
    {
        w0 += state.imperfection;
    }

    inline void updateHarm(const uint16_t flags)
    {
        if (flags & k_flag_harm)
        {
            static const uint8_t k_a_thr = k_waves_a_cnt;
            static const uint8_t k_b_thr = k_a_thr + k_waves_b_cnt;
            static const uint8_t k_c_thr = k_b_thr + k_waves_c_cnt;

            uint8_t idx = params.harm;
            const float *const *table;

            if (idx < k_a_thr)
            {
                table = wavesA;
            }
            else if (idx < k_b_thr)
            {
                table = wavesB;
                idx -= k_a_thr;
            }
            else
            {
                table = wavesC;
                idx -= k_b_thr;
            }
            state.harm = table[idx];
        }
    }

    State state;
    Params params;
    dsp::BiQuad prelpf, postlpf;
};
