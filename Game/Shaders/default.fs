#version 330 core

out vec4 Fragcolor;

uniform vec4 vColor;

void main() 
{
	Fragcolor= vColor;
}