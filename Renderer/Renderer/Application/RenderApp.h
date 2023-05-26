#pragma once

#include "AppBase.h"

namespace NS
{
    class SceneBase;

    class RenderApp : public AppBase
    {
    public:
        RenderApp(int screenWidth, int screenHeight);
        RenderApp(const RenderApp&) = delete;
        RenderApp& operator=(const RenderApp&) = delete;

        virtual bool Initialize() override;
        virtual void UpdateGUI() override;
        virtual void Update(float dt) override;
        virtual void Render() override;

    private:
        SceneBase* m_pScene;
    };
}


