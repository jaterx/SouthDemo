#version 400

// Passed from the vertex shader
in vec2 uv;

// Passed from outside
uniform float time;
uniform sampler2D diffuse_map;


void main()
{
	 gl_FragColor = texture(diffuse_map, uv);

}