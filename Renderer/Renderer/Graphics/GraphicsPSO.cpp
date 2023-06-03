#include "GraphicsPSO.h"

namespace NS
{
    void GraphicsPSO::operator=(const GraphicsPSO& pso) 
    {
        m_vertexShader = pso.m_vertexShader;
        m_pixelShader = pso.m_pixelShader;
        m_hullShader = pso.m_hullShader;
        m_domainShader = pso.m_domainShader;
        m_geometryShader = pso.m_geometryShader;
        m_inputLayout = pso.m_inputLayout;
        m_blendState = pso.m_blendState;
        m_depthStencilState = pso.m_depthStencilState;
        m_rasterizerState = pso.m_rasterizerState;
        m_primitiveTopology = pso.m_primitiveTopology;
    }
}