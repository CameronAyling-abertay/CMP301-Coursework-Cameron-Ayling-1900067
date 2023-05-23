// Tessellation Hull Shader
// Prepares control points for tessellation
struct InputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    int distance : TEXCOORD1;
};

struct ConstantOutputType
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

cbuffer TessBuffer : register(b0)
{
    int4 edgeTess;
    int2 inTess;
}

struct OutputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

ConstantOutputType PatchConstantFunction(InputPatch<InputType, 4> inputPatch, uint patchId : SV_PrimitiveID)
{
    ConstantOutputType output;

    output.edges[0] = (edgeTess[0] - inputPatch[0].distance / int(6) < 1) ? 1 : edgeTess[0] - inputPatch[0].distance / int(6);
    output.edges[1] = (edgeTess[1] - inputPatch[1].distance / int(6) < 1) ? 1 : edgeTess[1] - inputPatch[1].distance / int(6);
    output.edges[2] = (edgeTess[2] - inputPatch[2].distance / int(6) < 1) ? 1 : edgeTess[2] - inputPatch[2].distance / int(6);
    output.edges[3] = (edgeTess[3] - inputPatch[3].distance / int(6) < 1) ? 1 : edgeTess[3] - inputPatch[3].distance / int(6);

    output.inside[0] = (inTess[0] - inputPatch[0].distance / int(6) < 1) ? 1 : inTess[0] - inputPatch[0].distance / int(6);
    output.inside[1] = (inTess[1] - inputPatch[0].distance / int(6) < 1) ? 1 : inTess[1] - inputPatch[0].distance / int(6);

    return output;
}


[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(4)]
[patchconstantfunc("PatchConstantFunction")]
OutputType main(InputPatch<InputType, 4> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    OutputType output;

    // Set the position for this control point as the output position.
    output.position = patch[pointId].position;
    output.tex = patch[pointId].tex;
    output.normal = patch[pointId].normal;

    return output;
}