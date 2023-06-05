#pragma once

#include "AppBase.h"

#include "Object/Camera.h"

/*
*	<RenderApp>
*	�⺻ Application Ŭ������ AppBase Ŭ������ ��� ����.
*	���� �޴� UI�� �����ϸ�, ���� ������ ��� �ش� ���� �����ϰ� Render() �Լ��� ȣ��, ���� ���� ������ �۾��� �����.
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


