#pragma once

#include "AppBase.h"

namespace NS
{
    class RenderApp : public AppBase
    {
    public:
        RenderApp(int screenWidth, int screenHeight);
        RenderApp(const RenderApp&) = delete;
        RenderApp& operator=(const RenderApp&) = delete;

        virtual bool Initialize() override;
        virtual void Update(float dt) override;
        virtual void Render() override;

    };
}


