#version 330 core

uniform sampler2D texture1;

in vec3 pos;
out vec4 FragColor;

void main() {
	float potential = 0;
	
	FragColor = texture(texture1, vec2((pos.x + 1.0f) / 2.0f, (1.0f - pos.y) / 2.0f));
}
