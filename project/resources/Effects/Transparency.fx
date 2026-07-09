



float PI = 3.1415f;

float3 gCameraPosition : CAMERA;

float4x4 gWorldMatrix : WORLD;
float4x4 gWorldViewProjection : WorldViewProjection;


Texture2D gDiffuseMap : DiffuseMap;
Texture2D gNormalMap : NormalMap;
Texture2D gGlossMap : GlossMap;
Texture2D gSpecularMap : SpecularMap;
Texture2D gTransparentDiffuseMap : TransparentDiffuseMap;


///check [VERTEX SHADER SEMANTICS]     <===================
struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Color : COLOR;
    float2 UV : TEXCOORD;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
};

///check [PIXEL SHADER SEMANTICS]      <==============
struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float4 WorldPosition : WORLD;
    float3 Color : COLOR;
    float2 UV : TEXCOORD;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 ViewDirection : VIEW;
};


// VERTEX SHADER ---------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    
    
    //To know which order to use mul see Row Major vs columnn major matrices
    output.WorldPosition = mul(float4(input.Position, 1.0f), gWorldMatrix);
    ///output.Position = mul(float4(input, 1.0f), gWorldViewProjection);
    output.Position = mul(output.WorldPosition, gWorldViewProjection);
    output.Color = input.Color;
    output.UV = input.UV;
    output.Normal = mul(normalize(input.Normal), (float3x3) (gWorldMatrix)); //float4(-input.Normal.b, input.Normal.g, -input.Normal.r, 1.0f
    output.Tangent = mul(normalize(input.Tangent), (float3x3) (gWorldMatrix));
    output.ViewDirection = normalize(gCameraPosition - (float3) output.WorldPosition);
    
    return output;
}


float4 PixelShading(VS_OUTPUT input, SamplerState samplingState)
{
    const float diffuseReflectionCoef = 7.0f;
    const float4 lambertDiffuse = diffuseReflectionCoef * gTransparentDiffuseMap.Sample(samplingState, input.UV) / PI;
    return lambertDiffuse;
}

SamplerState TextureSamplerPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Wrap;
    AddressV = Wrap;
};
SamplerState TextureSamplerLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};
SamplerState TextureSamplerAnisotropic
{
    Filter = ANISOTROPIC;
    AddressU = Wrap;
    AddressV = Wrap;
};


// PIXEL SHADER ---------------------------------------------------------
float4 PS_PointSampling(VS_OUTPUT input) : SV_TARGET
{
    return PixelShading(input, TextureSamplerPoint);
}

float4 PS_LinearSampling(VS_OUTPUT input) : SV_TARGET
{
    return PixelShading(input, TextureSamplerLinear);
}

float4 PS_AnisotropicSampling(VS_OUTPUT input) : SV_TARGET
{
    return PixelShading(input, TextureSamplerAnisotropic);
}

technique11 TechniquePointSampling
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS_PointSampling()));
    }
}
technique11 TechniqueLinearSampling
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS_LinearSampling()));
    }
}
technique11 TechniqueAnisotropicSampling
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS_AnisotropicSampling()));
    }
}

