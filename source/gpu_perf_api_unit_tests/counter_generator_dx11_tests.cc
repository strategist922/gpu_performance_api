//==============================================================================
// Copyright (c) 2012-2020 Advanced Micro Devices, Inc. All rights reserved.
/// \author AMD Developer Tools Team
/// \file
/// \brief  Unit Tests for DX11 Counter Generator
//==============================================================================

#include "counter_generator_tests.h"
#include "gpu_perf_api_types.h"
#include "gpa_counter.h"

#include "counters/public_derived_counters_dx11_gfx8.h"
#include "counters/public_derived_counters_dx11_gfx9.h"
#include "counters/public_derived_counters_dx11_gfx10.h"

#ifdef AMDT_INTERNAL
#include "gpa_hw_counter_dx11_gfx8.h"
#include "gpa_hw_counter_dx11_gfx9.h"
#include "gpa_hw_counter_dx11_gfx10.h"
#endif

static void GetExpectedCountersForGeneration(GPA_Hw_Generation gen, std::vector<const char*>& counterNames)
{
    counterNames.clear();

    const GPACounterDesc* pPublicCounters    = nullptr;
    size_t                publicCounterCount = 0;

    GPA_CounterGroupDesc*     pHardwareGroups    = nullptr;
    GPA_HardwareCounterDesc** ppHardwareCounters = nullptr;
    unsigned int              hwGroupCount       = 0;

    switch (gen)
    {
    case GPA_HW_GENERATION_GFX8:
        pPublicCounters    = DX11GFX8_PUBLIC_COUNTERS;
        publicCounterCount = DX11GFX8_PUBLIC_COUNTER_COUNT;
#ifdef AMDT_INTERNAL
        pHardwareGroups    = countergfx8::hw_dx11_groups_gfx8;
        hwGroupCount       = countergfx8::hw_dx11_group_count_gfx8;
        ppHardwareCounters = countergfx8::dx11_counter_group_array_gfx8;
#endif
        break;

    case GPA_HW_GENERATION_GFX9:
        pPublicCounters    = DX11GFX9_PUBLIC_COUNTERS;
        publicCounterCount = DX11GFX9_PUBLIC_COUNTER_COUNT;
#ifdef AMDT_INTERNAL
        pHardwareGroups    = countergfx9::hw_dx11_groups_gfx9;
        hwGroupCount       = countergfx9::hw_dx11_group_count_gfx9;
        ppHardwareCounters = countergfx9::dx11_counter_group_array_gfx9;
#endif
        break;

    case GPA_HW_GENERATION_GFX10:
        pPublicCounters    = DX11GFX10_PUBLIC_COUNTERS;
        publicCounterCount = DX11GFX10_PUBLIC_COUNTER_COUNT;
#ifdef AMDT_INTERNAL
        pHardwareGroups    = countergfx10::hw_dx11_groups_gfx10;
        hwGroupCount       = countergfx10::hw_dx11_group_count_gfx10;
        ppHardwareCounters = countergfx10::dx11_counter_group_array_gfx10;
#endif
        break;

    default:
        break;
    }

    for (size_t i = 0; i < publicCounterCount; i++)
    {
        counterNames.push_back(pPublicCounters[i].m_pName);
    }

    // Optionally, get internal derived counters
    const GPACounterDesc* pInternalDerivedCounters    = nullptr;
    size_t                internalDerivedCounterCount = 0;

#ifdef AMDT_INTERNAL
    GPA_GetInternalDerivedCounters(GPA_API_DIRECTX_11, gen, &pInternalDerivedCounters, &internalDerivedCounterCount);
#endif

    for (size_t i = 0; i < internalDerivedCounterCount; i++)
    {
        counterNames.push_back(pInternalDerivedCounters[i].m_pName);
    }

    for (unsigned int i = 0; i < hwGroupCount; i++)
    {
        for (unsigned int j = 0; j < pHardwareGroups[i].m_numCounters; j++)
        {
            counterNames.push_back(ppHardwareCounters[i][j].m_pName);
        }
    }
}

// Test the DX11 counter names on all supported hardware
TEST(CounterDLLTests, DX11CounterNames)
{
    VerifyHardwareNotSupported(GPA_API_DIRECTX_11, gDevIdUnknown, FALSE);

    std::vector<const char*> counterNames;
    GetExpectedCountersForGeneration(GPA_HW_GENERATION_GFX8, counterNames);
    VerifyCounterNames(GPA_API_DIRECTX_11, gDevIdVI, FALSE, counterNames);
    GetExpectedCountersForGeneration(GPA_HW_GENERATION_GFX9, counterNames);
    VerifyCounterNames(GPA_API_DIRECTX_11, gDevIdGfx9, FALSE, counterNames);
    GetExpectedCountersForGeneration(GPA_HW_GENERATION_GFX10, counterNames);
    VerifyCounterNames(GPA_API_DIRECTX_11, gDevIdGfx10, FALSE, counterNames);
}

// Test the DX11 counter names on all generations
TEST(CounterDLLTests, DX11CounterNamesByGeneration)
{
    VerifyHardwareNotSupported(GPA_API_DIRECTX_11, GPA_HW_GENERATION_NONE, FALSE);
    VerifyHardwareNotSupported(GPA_API_DIRECTX_11, GPA_HW_GENERATION_NVIDIA, FALSE);
    VerifyHardwareNotSupported(GPA_API_DIRECTX_11, GPA_HW_GENERATION_INTEL, FALSE);
    VerifyHardwareNotSupported(GPA_API_DIRECTX_11, GPA_HW_GENERATION_GFX6, FALSE);
    VerifyHardwareNotSupported(GPA_API_DIRECTX_11, GPA_HW_GENERATION_GFX7, FALSE);

    std::vector<const char*> counterNames;
    GetExpectedCountersForGeneration(GPA_HW_GENERATION_GFX8, counterNames);
    VerifyCounterNames(GPA_API_DIRECTX_11, GPA_HW_GENERATION_GFX8, FALSE, counterNames);
    GetExpectedCountersForGeneration(GPA_HW_GENERATION_GFX9, counterNames);
    VerifyCounterNames(GPA_API_DIRECTX_11, GPA_HW_GENERATION_GFX9, FALSE, counterNames);
    GetExpectedCountersForGeneration(GPA_HW_GENERATION_GFX10, counterNames);
    VerifyCounterNames(GPA_API_DIRECTX_11, GPA_HW_GENERATION_GFX10, FALSE, counterNames);
}

TEST(CounterDLLTests, Dx11CounterLibTest)
{
    VerifyCounterLibInterface(GPA_API_DIRECTX_11, gDevIdVI, REVISION_ID_ANY, false);
    VerifyCounterLibInterface(GPA_API_DIRECTX_11, gDevIdGfx9, REVISION_ID_ANY, false);
    VerifyCounterLibInterface(GPA_API_DIRECTX_11, gDevIdGfx10, REVISION_ID_ANY, false);
}
