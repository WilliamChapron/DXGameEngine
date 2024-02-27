cbuffer Data : register(b0)
{
    float4x4 model;
    float4x4 view;
    float4x4 projection;
};

struct VSInput
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VSOutput VSMain(VSInput input)
{
    VSOutput result;

    result.position = mul(float4(input.position, 1.0f), model);
    result.position = mul(result.position, view);
    result.position = mul(result.position, projection);

    result.color = input.color;

    return result;
}

float4 PSMain(VSOutput input) : SV_TARGET
{
    return input.color;
}
