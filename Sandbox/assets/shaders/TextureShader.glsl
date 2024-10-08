// Basic Texture Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in int a_EntityID;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
	float TilingFactor;
};

layout (location = 0) out VertexOutput Output;
layout (location = 3) out flat int v_TexIndex;
layout (location = 4) out flat int v_EntityID;

void main()
{
	Output.Color = a_Color;
	Output.TexCoord = a_TexCoord;
	Output.TilingFactor = a_TilingFactor;
	v_TexIndex = int(a_TexIndex);
	v_EntityID = a_EntityID;

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int color2;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
	float TilingFactor;
};

layout (location = 0) in VertexOutput Input;
layout (location = 3) in flat int v_TexIndex;
layout (location = 4) in flat int v_EntityID;

layout (binding = 0) uniform sampler2D u_Textures[32];

void main()
{
	vec4 texColor = Input.Color;

	switch (int(v_TexIndex))
	{
		case 0: texColor *= texture(u_Textures[0], Input.TexCoord * Input.TilingFactor); break;
		case 1: texColor *= texture(u_Textures[1], Input.TexCoord * Input.TilingFactor); break;
		case 2: texColor *= texture(u_Textures[2], Input.TexCoord * Input.TilingFactor); break;
		case 3: texColor *= texture(u_Textures[3], Input.TexCoord * Input.TilingFactor); break;
		case 4: texColor *= texture(u_Textures[4], Input.TexCoord * Input.TilingFactor); break;
		case 5: texColor *= texture(u_Textures[5], Input.TexCoord * Input.TilingFactor); break;
		case 6: texColor *= texture(u_Textures[6], Input.TexCoord * Input.TilingFactor); break;
		case 7: texColor *= texture(u_Textures[7], Input.TexCoord * Input.TilingFactor); break;
		case 8: texColor *= texture(u_Textures[8], Input.TexCoord * Input.TilingFactor); break;
		case 9: texColor *= texture(u_Textures[9], Input.TexCoord * Input.TilingFactor); break;
		case 10: texColor *= texture(u_Textures[10], Input.TexCoord * Input.TilingFactor); break;
		case 11: texColor *= texture(u_Textures[11], Input.TexCoord * Input.TilingFactor); break;
	}

	color = texColor;

	color2 = v_EntityID;
}