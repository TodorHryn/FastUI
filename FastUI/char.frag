#version 330 core

uniform vec4 color;
uniform sampler2D text;

in vec3 pos;
out vec4 FragColor;

void main() {
	vec2 texPos = vec2((pos.x + 1.0) / 2.0, (1.0 - pos.y) / 2.0);
	FragColor = color * vec4(1.0, 1.0, 1.0, texture(text, texPos).r);
}
