



float PI = 3.1415f;
float shininess = 25.0f;

float gLightIntensity = 27.0f;
float3 gLigthDirection = float3(.577f, -.577f, .577f);


float3 gCameraPosition : CAMERA;

float4x4 gWorldMatrix : WORLD;
float4x4 gWorldViewProjection : WorldViewProjection;

Texture2D gDiffuseMap : DiffuseMap;
Texture2D gNormalMap : NormalMap;
Texture2D gGlossMap : GlossMap;
Texture2D gSpecularMap : SpecularMap;
Texture2D gTransparentDiffuseMap : TransparentDiffuseMap;


///check [VERTEX SHADER SEMANTICS]     <===================
struct VS_INPUT{
    float3 Position : POSITION;
    float3 Color : COLOR;
    float2 UV : TEXCOORD;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
};

///check [PIXEL SHADER SEMANTICS]      <==============
struct VS_OUTPUT{
    float4 Position : SV_POSITION;
    float4 WorldPosition : WORLD;
    float3 Color : COLOR;
    float2 UV : TEXCOORD;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 ViewDirection : VIEW;
};


// VERTEX SHADER ---------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input){
    VS_OUTPUT output = (VS_OUTPUT)0;
    
    
    //To know which order to use mul see Row Major vs columnn major matrices
    output.WorldPosition = mul(float4(input.Position, 1.0f), gWorldMatrix);
    ///output.Position = mul(float4(input, 1.0f), gWorldViewProjection);
    output.Position = mul(output.WorldPosition, gWorldViewProjection);
    output.Color = input.Color;
    output.UV = input.UV;
    output.Normal = mul(normalize(input.Normal), (float3x3) (gWorldMatrix)); //float4(-input.Normal.b, input.Normal.g, -input.Normal.r, 1.0f
    output.Tangent = mul(normalize(input.Tangent), (float3x3) (gWorldMatrix));
    output.ViewDirection = normalize(gCameraPosition - (float3)output.WorldPosition);
    
    return output;
}


float4 PixelShading(VS_OUTPUT input, SamplerState samplingState)
{
    
    ///NORMAL CALCULATION----------------------------------------
    float4 sampledNormal = gNormalMap.Sample(samplingState, input.UV);
    sampledNormal = float4(2.0f * sampledNormal.r - 1.0f, 2.0f * sampledNormal.g - 1.0f, 2.0f * sampledNormal.b - 1.0f, sampledNormal.a);
    float3 biTangent = normalize(cross(input.Normal,input.Tangent));
    
    float4x4 ONB_matrix = float4x4( input.Tangent.x, biTangent.x, input.Normal.x, 0,
                                    input.Tangent.y, biTangent.y, input.Normal.y, 0,
                                    input.Tangent.z, biTangent.z, input.Normal.z, 0,
                                    0, 0, 0, 1);
    float3 normal = normalize((float3)mul(ONB_matrix, sampledNormal));
    // ----------------------------------------------------------
    
    
    
    //OBSERVED AREA--------------------------------------
    ///SampledNormal Gets treated as a column vector

    float observedArea = dot(normal, -gLigthDirection);
    //---------------------------------------------------
    
    
    //DIFFUSE REFLECTION --------------------------------
    const float diffuseReflectionCoef = 7.0f;
    const float4 lambertDiffuse = diffuseReflectionCoef * gDiffuseMap.Sample(samplingState, input.UV) / PI;
    //---------------------------------------------------
    
    
    ////VIEWDIRECTION-------------------------------------
    //const float3 viewDirection = normalize(gCameraPosition - (float3) input.WorldPosition);
    ////--------------------------------------------------
    
    
    
    float4 glossSample = gGlossMap.Sample(samplingState, input.UV);
    float4 specularSample = gSpecularMap.Sample(samplingState, input.UV);
    
    float3 reflectedLightDir = normalize(reflect(gLigthDirection, normal));
    float cosAplha = abs(dot(reflectedLightDir,input.ViewDirection));
    float phong = specularSample.r * pow(cosAplha, glossSample.x * shininess);
    if (isnan(phong))
    {
        phong = 0;
    }
    //return float4(observedArea, observedArea, observedArea,1.0f);
    //return float4(cosAplha, cosAplha, cosAplha, 1.0f);
    //return float4(phong, phong, phong, 1.0f);
    //return float4(normal*0.5f+0.5f, 1.0f);
    //return float4(reflectedLightDir * 0.5 + 0.5, 1);
    //return float4(input.ViewDirection * 0.5 + 0.5, 1);
    return lambertDiffuse * observedArea + float4(phong, phong, phong,0.0f);

}

float4 TransparencyShading(VS_OUTPUT input, SamplerState samplingState)
{
    const float diffuseReflectionCoef = 7.0f;
    const float4 lambertDiffuse = diffuseReflectionCoef * gDiffuseMap.Sample(samplingState, input.UV) / PI;
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

