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

#version 410

layout(isolines) in;
uniform mat4 MVP;

void main(){
  
  float u = gl_TessCoord.x;

  vec3 p0 = gl_in[0].gl_Position.xyz;
  vec3 p1 = gl_in[1].gl_Position.xyz;
  vec3 p2 = gl_in[2].gl_Position.xyz;
  vec3 p3 = gl_in[3].gl_Position.xyz;
  
  float u1 = (1.0 - u);
  float u2 = u * u;

  float b3 = u2 * u;
  float b2 = 3.0 * u2 * u1;
  float b1 = 3.0 * u * u1 * u1;
  float b0 = u1 * u1 * u1;

  vec3 p = p0 * b0 + p1 * b1 + p2 * b2 + p3 * b3;
  
  p.y = 2 * gl_TessCoord.y + p.y;

  gl_Position = MVP * vec4(p, 1.0);
  

  //float r = (gl_TessCoord.y + gl_TessCoord.x / gl_TessLevelOuter[0]);
  //float t = gl_TessCoord.x * 2.0 * 3.14159;

    //gl_Position = vec4(sin(t)*r, cos(t)*r,0.5, 1.0);

  //gl_Position = vec4(gl_TessCoord.x, gl_TessCoord.y, 0.0, 1.0);

}
