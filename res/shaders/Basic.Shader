#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP; // Model View Projection matrix

void main()
{
   gl_Position = u_MVP * position; // will move it to appropriate space based on ortho matrix we provided
   v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	//color = vec4(1.0, 0.0, 0.0, 1.0); // RED
	//color = vec4(0.2, 0.3, 0.8, 1.0); // BLUE
	vec4 texColor = texture(u_Texture, v_TexCoord);
	//color = u_Color;
	color = texColor; // actual texture look up color
	//color = vec4(1.0); // white
};