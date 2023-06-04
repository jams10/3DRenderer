
#include "SCDrawTriangle.h"

#include <imgui.h>

#include "Graphics/GraphicsProcessor.h"
#include "Graphics/D3D11Graphics.h"
#include "Graphics/GraphicsCommon.h"
#include "Utility/StringEncode.h"

namespace NS
{
	void SCDrawTriangle::Initialize(GraphicsProcessor* pGraphics)
	{
		SceneBase::Initialize(pGraphics);

		m_triangleModel.Initialize(pGraphics, std::vector<MeshForCPU>{MakeTriangle()});

		// ���� ��� ���� ����.
		pGraphics->GetD3D11()->CreateGlobalConstantBuffer(m_globalConstantBufferData);
	}

	SCDrawTriangle::~SCDrawTriangle()
	{
		m_triangleModel.m_meshes[0]->Shutdown();
	}

	void SCDrawTriangle::UpdateGUI()
	{
		SceneBase::UpdateGUI();

		ImGui::Begin("Position");

		ImGui::SliderFloat3("Translation", &m_translation.x, -5.0f, 5.0f);
		ImGui::SliderFloat3("Rotation(Rad)", &m_rotation.x, -3.14f, 3.14f);
		ImGui::SliderFloat3("Scaling", &m_scale.x, 0.1f, 2.0f);

		ImGui::End();
	}

	void SCDrawTriangle::Update(float dt)
	{
		SceneBase::Update(dt);

		/* �۷ι� ��� ���۶� �� �� ��� ���� ������ ���̴��� �����ϰ� cpu�ʿ��� ����, ���ε� ���־�� ��.*/

		// ���� ��ȯ ���. SRT ����.
		m_triangleModel.m_meshWorldTransformData.world = Matrix::CreateScale(m_scale) *
			Matrix::CreateRotationY(m_rotation.y) *
			Matrix::CreateRotationX(m_rotation.x) *
			Matrix::CreateRotationZ(m_rotation.z) *
			Matrix::CreateTranslation(m_translation);
		// �� Ʈ������ ��� ���� ������Ʈ.
		m_triangleModel.UpdateModelTransformConstantBuffer(m_pGraphics);

		// �۷ι� ��� ���� ������ ������Ʈ.(CPU)
		UpdateGlobalConstantData(
			m_camera.GetCameraPosition(), 
			m_camera.GetViewMatrixRow(), 
			m_camera.GetProjectionMatrixRow()
		);

		// �۷ι� ��� ���� ������Ʈ.(GPU)
		m_pGraphics->GetD3D11()->UpdateGlobalConstantBuffer(m_globalConstantBufferData);
	}

	void SCDrawTriangle::Render()
	{
		// ���÷� ���ε�.
		m_pGraphics->GetD3D11()->GetContext()->VSSetSamplers(0, UINT(Graphics::samplerStates.size()),
																Graphics::samplerStates.data());
		m_pGraphics->GetD3D11()->GetContext()->PSSetSamplers(0, UINT(Graphics::samplerStates.size()),
																Graphics::samplerStates.data());

		m_pGraphics->SetPipelineState(Graphics::vertexColorPSO);
		// ���� ��� ���� ���������ο� ����.
		m_pGraphics->GetD3D11()->SetGlobalConstantBufferData();

		m_triangleModel.Render(m_pGraphics); // �� ����, �ε��� ����, ��� ���� ���ε�.
	}

	MeshForCPU SCDrawTriangle::MakeTriangle()
	{
		MeshForCPU meshForCPU;
		vector<Vector3> positions;
		vector<Vector3> normalModel;
		vector<Vector2> texcoord;
		vector<Vector3> tangentModel;
		vector<Vector3> colors;

		positions.push_back(Vector3(-1.f, -1.f, 0.f));
		positions.push_back(Vector3(0.f, 1.f, 0.f));
		positions.push_back(Vector3(1.f, -1.f, 0.f));
		normalModel.push_back(Vector3(0.0f, 0.0f, -1.0f));
		normalModel.push_back(Vector3(0.0f, 0.0f, -1.0f));
		normalModel.push_back(Vector3(0.0f, 0.0f, -1.0f));
		texcoord.push_back(Vector2(0.0f, 1.0f));
		texcoord.push_back(Vector2(0.5f, 0.0f));
		texcoord.push_back(Vector2(0.1f, 1.0f));
		tangentModel.push_back(Vector3(0.0f, 0.0f, -1.0f));
		tangentModel.push_back(Vector3(0.0f, 0.0f, -1.0f));
		tangentModel.push_back(Vector3(0.0f, 0.0f, -1.0f));
		colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
		colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
		colors.push_back(Vector3(0.0f, 0.0f, 1.0f));

		for (int i = 0; i < 3; ++i)
		{
			Vertex v;
			v.position = positions[i];
			v.normalModel = normalModel[i];
			v.texcoord = texcoord[i];
			v.tangentModel = tangentModel[i];
			v.color = colors[i];
			meshForCPU.vertices.push_back(v);
		}

		meshForCPU.indices = { 0, 1, 2, };
		return meshForCPU;
	}
}