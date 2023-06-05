#pragma once

#include "AppBase.h"

#include "Object/Camera.h"

/*
*	<RenderApp>
*	기본 Application 클래스인 AppBase 클래스를 상속 받음.
*	메인 메뉴 UI를 생성하며, 씬을 선택할 경우 해당 씬을 생성하고 Render() 함수를 호출, 씬에 대한 렌더링 작업을 담당함.
*/
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

        void MoveCamera(float dt);

    private:
        SceneBase* m_pScene;
        Camera m_Camera;
        int m_selectedSceneIndex;
        int m_currentSceneIndex;
        bool m_openSystemInfoWindow;
        bool m_openSceneSelectWindow;

    private:
        void ShowTitleMenuWindow();
        void ShowSystemInfoWindow();
        void ShowSceneSelectWindow();
    };
}


