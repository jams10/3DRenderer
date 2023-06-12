
#include "Common.hlsli"

/*
*   Blinn Phong 반사 모델을 사용해 라이팅을 계산하는 픽셀 쉐이더.
*/

Texture2D g_texture0 : register(t0);

cbuffer PixelConstantBuffer : register(b0)
{
    Material material;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
    float3 vertexToEye = normalize(eyeWorld - input.posWorld);
    float3 color = float3(0.0, 0.0, 0.0);
    
    // 라이팅 계산.
    int i = 0;
    [unroll]
    for (i = 0; i < MAX_LIGHTS; ++i)
    {   
        if (lights[i].turnOn == 0)
            continue;
        if (lights[i].type == 0)
            color += ComputeDirectionalLight(lights[i], material, input.normalWorld, vertexToEye) * lights[i].color;
        if (lights[i].type == 1)
            color += ComputePointLight(lights[i], material, input.posWorld, input.normalWorld, vertexToEye) * lights[i].color;
        if (lights[i].type == 2)
            color += ComputeSpotLight(lights[i], material, input.posWorld, input.normalWorld, vertexToEye) * lights[i].color;
    }
     
    return material.bUseTexture ? float4(color, 1.0) * g_texture0.Sample(linearClampSampler, input.texcoord) :
                                  float4(color, 1.0) * float4(material.color, 1.0f);
}