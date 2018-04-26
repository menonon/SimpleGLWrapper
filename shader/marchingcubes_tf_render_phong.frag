/*
The MIT License (MIT)

Copyright (c) 2013-2018 menonon

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#version 330 core

in vertex_info{
  vec3 vPosition;
  vec3 vNormal;
} In;


layout(location = 0) out vec4 color;

uniform vec3 diffuse_albedo /*= vec3(0.5, 0.2, 0.7)*/;
uniform vec3 specular_albedo = vec3(0.7);
uniform float specular_power = 128.0;

uniform vec3 uLightPosition = vec3(100, 100, 100);
uniform vec3 uLightIntensity = vec3(100.0, 100.0, 100.0);


void main(){

  vec3 N = normalize(In.vNormal);
  vec3 L = normalize(uLightPosition - In.vPosition);
  vec3 V = normalize(- In.vPosition);

  vec3 R = reflect(-L, N);

  vec3 diffuse = max( dot(N, L), 0.0) * diffuse_albedo;
  //vec3 diffuse = max( dot(N, L), 0.0) * In.lC;
  vec3 specular = pow(max(dot(R, V), 0.0), specular_power) * specular_albedo;

  color = vec4(diffuse + specular, 1.0);
  //color = vec4(diffuse_albedo, 1.0);
  //color = vec4(In.vNormal,1.0);
}
