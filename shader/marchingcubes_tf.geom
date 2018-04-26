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

// A inGPU MarchingCubes by Geometry shader
// vert : simple.vert
// frag : phong.frag

#version 440 core

uniform mat4 uModelView;
uniform mat4 uProjection;

uniform vec3 uLightPosition = vec3(100, 100, 100);

uniform ivec3 uSlice;
uniform float uThreshold;
uniform sampler3D uData;

uniform isamplerBuffer uEdgeConnection;
uniform ivec2 uEdgeToVerts[12] = ivec2[12](ivec2(0,1), ivec2(1,2), ivec2(2,3), ivec2(0,3),
					   ivec2(4,5), ivec2(5,6), ivec2(6,7), ivec2(4,7),
					   ivec2(0,4), ivec2(1,5), ivec2(2,6), ivec2(3,7) );

in vertex_info{
  vec3 vNormal;
  vec3 vTextureCoord;
} In[];

/*
out lighting{
  vec3 lN;
  vec3 lL;
  vec3 lV;
  vec3 lC;
} Out;
*/



layout(points) in;
layout(triangle_strip, max_vertices=50) out;
//layout(points, max_vertices=16) out;

//layout(xfb_buffer=0, xfb_offset=0) out vec3 iVertexPosition;
//layout(xfb_buffer=1, xfb_offset=0) out vec3 iNormal;
out vec3 iVertexPosition;
out vec3 iNormal;
//layout(xfb_buffer = 2, xfb_offset = 0) out vec3 iTextureCoord;

void main(){
  
  vec4 center = gl_in[0].gl_Position;
  ivec3 f = ivec3(floor(center.x), floor(center.y), floor(center.z));
  ivec3 c = ivec3(ceil(center.x), ceil(center.y), ceil(center.z));

  ivec4 voxelVerts[8];
  voxelVerts[0] = ivec4(f.x, f.y, f.z, 1.0);
  voxelVerts[1] = ivec4(c.x, f.y, f.z, 1.0);
  voxelVerts[2] = ivec4(c.x, c.y, f.z, 1.0);
  voxelVerts[3] = ivec4(f.x, c.y, f.z, 1.0);
  voxelVerts[4] = ivec4(f.x, f.y, c.z, 1.0);
  voxelVerts[5] = ivec4(c.x, f.y, c.z, 1.0);
  voxelVerts[6] = ivec4(c.x, c.y, c.z, 1.0);
  voxelVerts[7] = ivec4(f.x, c.y, c.z, 1.0);
  
  float voxelValues[8];
  bool isLargerThanThreshold[8];

  // get values of each voxel ,and if larger than threshold
  for(int i=0; i<8; i++){
    voxelValues[i] = texelFetch(uData, voxelVerts[i].xyz, 0).x;
    if(voxelValues[i] >= uThreshold){
      isLargerThanThreshold[i] = true;
    }
    else{
      isLargerThanThreshold[i] = false;
    }
  }
  
  // calculate pattern 
  int pattern = 0;
  for(int i=0; i<8; i++){
    if(isLargerThanThreshold[i]){
      pattern += int(pow(2.0,float(i)));
    }
  }
  
  // get edge connection and the vertices and emit these
  int offset = 16*pattern;
  for(int i=0; i<16; i+=3){
    int edgeNum[3];
    for(int j=0; j<3; j++){
      edgeNum[j] = int(texelFetch(uEdgeConnection, int(offset+i+j)).r);
    }
    if(edgeNum[0] < 0){
      break;
    }
    
    vec4 triangles[3];
    for(int j=0; j<3; j++){
      int vertA = int(uEdgeToVerts[edgeNum[j]].x);
      int vertB = int(uEdgeToVerts[edgeNum[j]].y);
      if(voxelValues[vertB] > voxelValues[vertA]){
	    int tmp=vertB;
		vertB=vertA;
		vertA=tmp;
      }
      float percentage = float(uThreshold - voxelValues[vertB]) / float(voxelValues[vertA] - voxelValues[vertB]);
      triangles[j] = mix(voxelVerts[vertB], voxelVerts[vertA], percentage);
    }
    
    /*
    vec3 N = mat3(uModelView) * cross((triangles[2] - triangles[0]).xyz, (triangles[1]- triangles[0]).xyz);
    vec4 gTri = uModelView * vec4( (triangles[0].x + triangles[1].x + triangles[2].x) /3.0,
				   (triangles[0].y + triangles[1].y + triangles[2].y) /3.0,
				   (triangles[0].z + triangles[1].z + triangles[2].z) /3.0, 
				   1.0 );

    vec3 L = uLightPosition - gTri.xyz;
    vec3 V = -gTri.xyz;
    */

    vec3 N = cross((triangles[2] - triangles[0]).xyz, (triangles[1]- triangles[0]).xyz);

    for(int j=0; j<3; j++){
      //gl_Position = triangles[j];
      iVertexPosition = triangles[j].xyz;
      iNormal = N;
      /*
      gl_Position = uProjection * uModelView * triangles[j];
      Out.lN = N;
      Out.lL = L;
      Out.lV = V;
	    Out.lC = vec3(1.0,1.0,1.0);
      gl_PointSize = 5;
      */
      EmitVertex();
    }
    
    EndPrimitive();
  }

}






