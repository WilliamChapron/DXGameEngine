// Shader combiné (Vertex + Pixel)


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

    result.position = float4(input.position, 1.0f);
    result.color = input.color;

    return result;
}

float4 PSMain(VSOutput input) : SV_TARGET
{
    return input.color;
}
