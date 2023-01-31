#include "OBJShaderHeader.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{

//	float3 lightdir = float3(1, -1, 1);
//	lightdir = normalize(lightdir);

//	float3 lightcolor = float3(1, 1, 1);
	float4 wnormal = normalize(mul(world, float4(normal, 0)));
	float4 wpos = mul(world, pos);

	VSOutput output; // ピクセルシェーダーに渡す値
	output.svpos = mul(mul(viewproj, world), pos);

	float3 ambient = m_ambient;

	float3 diffuse = dot(lightv, wnormal.xyz) * m_diffuse;

	//const float3 eye = float3(0, 0, -20);
	const float shininess = 4.0f;
	float3 eyedir = normalize(cameraPos - wpos.xyz);
	float3 reflect = normalize(lightv + 2 * dot(lightv, wnormal.xyz) * wnormal.xyz);
//	float3 reflect = normalize(-lightv + 2 * dot(lightv, wnormal.xyz) * wnormal.xyz);
	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;


	//output.color.rgb = dot(-lightdir, normal) * m_diffuse * lightcolor;
	output.color.rgb = (ambient + diffuse + specular) * lightcolor;
	output.color.a = m_alpha;
	//	output.normal = normal;
	output.uv = uv;
	return output;
}