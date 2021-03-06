//==============================================================================
// Copyright (c) 2018-2020 Advanced Micro Devices, Inc. All rights reserved.
/// \author AMD Developer Tools Team
/// \file
/// \brief GL GPA Session Implementation
//==============================================================================

#include "gl_gpa_session.h"
#include "gl_gpa_pass.h"
#include "gpa_context_counter_mediator.h"

GLGPASession::GLGPASession(IGPAContext* pParentContext, GPA_Session_Sample_Type sampleType)
    : GPASession(pParentContext, sampleType)
{
}

GPA_API_Type GLGPASession::GetAPIType() const
{
    return GPA_API_OPENGL;
}

GPAPass* GLGPASession::CreateAPIPass(PassIndex passIndex)
{
    GPAPass* pRetPass = nullptr;

    CounterList*     pPassCounters = GetCountersForPass(passIndex);
    GPACounterSource counterSource = GetParentContext()->GetCounterSource((*pPassCounters)[0]);

    GLGPAPass* pGlPass = new (std::nothrow) GLGPAPass(this, passIndex, counterSource, pPassCounters);

    if (nullptr == pGlPass)
    {
        GPA_LogError("Unable to allocate memory for the pass.");
    }
    else
    {
        pRetPass = pGlPass;
    }

    return pRetPass;
}
