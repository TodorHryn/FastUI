#version 330 core

uniform sampler2D img;

in vec3 pos;
out vec4 FragColor;

void main() {
	vec2 texPos = vec2((pos.x + 1.0) / 2.0, (1.0 - pos.y) / 2.0);
	FragColor = vec4(texture(img, texPos).rgb, 1.0);
}
