#version 330 core

uniform vec4 color;

in vec3 pos;
out vec4 FragColor;

void main() {
	FragColor = color;
}
