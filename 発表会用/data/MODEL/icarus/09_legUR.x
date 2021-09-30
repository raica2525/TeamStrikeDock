xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 31;
 0.00541;-0.02943;24.59041;,
 -12.68389;-0.02943;16.62711;,
 -12.68389;-30.26323;16.62711;,
 0.00541;-30.26323;20.28461;,
 0.00541;-0.02943;-24.54059;,
 -12.68389;-0.02943;-16.57709;,
 -14.20049;-0.02943;0.02491;,
 0.00541;-0.02943;0.02491;,
 0.00541;-30.26323;-20.23469;,
 -12.68389;-30.26323;-16.57709;,
 0.00541;-43.80682;0.02491;,
 -14.20049;-38.76292;0.02491;,
 -14.20049;-21.19323;-11.59658;,
 -14.20049;-6.37863;-11.59658;,
 -14.20049;-6.37863;0.02491;,
 -14.20049;-31.01623;0.02491;,
 12.69471;-30.26323;-16.57709;,
 12.69471;-0.02943;-16.57709;,
 14.21141;-0.02943;0.02491;,
 14.21141;-38.76292;0.02491;,
 -14.20049;-21.19323;11.64642;,
 -14.20049;-6.37863;11.64642;,
 12.69471;-0.02943;16.62711;,
 12.69471;-30.26323;16.62711;,
 -20.22340;-21.19323;0.02491;,
 -21.71740;-6.37863;0.02491;,
 -15.82229;-6.37863;-11.59658;,
 -15.82229;-21.19323;-11.59658;,
 -15.82229;-21.19323;11.64642;,
 -15.82229;-6.37863;11.64642;,
 -15.82229;-31.01623;0.02491;;
 
 36;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,5,4;,
 4;10,11,9,8;,
 3;9,12,5;,
 3;5,13,6;,
 3;6,13,14;,
 3;13,5,12;,
 3;9,11,12;,
 3;12,11,15;,
 4;16,17,18,19;,
 4;7,6,1,0;,
 3;11,2,15;,
 3;15,2,20;,
 3;14,21,6;,
 3;6,21,1;,
 3;20,2,21;,
 3;21,2,1;,
 4;3,2,11,10;,
 4;18,22,23,19;,
 4;24,25,26,27;,
 4;28,29,25,24;,
 3;28,24,30;,
 3;30,24,27;,
 4;12,27,26,13;,
 4;13,26,25,14;,
 4;14,25,29,21;,
 4;21,29,28,20;,
 4;20,28,30,15;,
 4;15,30,27,12;,
 4;19,23,3,10;,
 4;16,19,10,8;,
 4;17,16,8,4;,
 4;18,17,4,7;,
 4;22,18,7,0;,
 4;23,22,0,3;;
 
 MeshMaterialList {
  10;
  36;
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  0,
  5,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  5,
  5,
  5,
  5,
  5,
  5;;
  Material {
   0.401600;0.401600;0.401600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\icarus\\gray.jpg";
   }
  }
  Material {
   0.800000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\icarus\\red2.jpg";
   }
  }
  Material {
   0.009600;0.009600;0.009600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\icarus\\black.jpg";
   }
  }
  Material {
   0.000000;0.367200;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\icarus\\blue2.jpg";
   }
  }
  Material {
   0.031200;0.800000;0.455200;0.700000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\icarus\\green.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\icarus\\white.jpg";
   }
  }
  Material {
   0.150400;0.156800;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\icarus\\blue.jpg";
   }
  }
  Material {
   0.800000;0.536800;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\icarus\\yellow.jpg";
   }
  }
  Material {
   0.800000;0.166400;0.068800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\icarus\\red.jpg";
   }
  }
  Material {
   0.094400;0.094400;0.094400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\icarus\\black.jpg";
   }
  }
 }
 MeshNormals {
  45;
  0.995853;0.000000;0.090977;,
  -0.840587;-0.021946;0.541232;,
  -0.749570;-0.033298;0.661087;,
  0.995853;0.000000;-0.090978;,
  0.000000;1.000000;0.000000;,
  -0.777025;-0.296270;-0.555389;,
  1.000000;0.000000;-0.000001;,
  -0.228749;-0.832621;-0.504397;,
  -0.997344;-0.037739;0.062295;,
  -0.983178;-0.022724;-0.181233;,
  -0.979224;-0.056845;0.194653;,
  -1.000000;0.000000;0.000000;,
  -0.990180;0.055339;-0.128377;,
  -0.984583;0.041513;0.169923;,
  -0.971764;-0.235954;-0.000000;,
  -0.902472;-0.219130;-0.370845;,
  -0.913945;-0.044897;-0.403348;,
  -0.998796;-0.049066;-0.000000;,
  -0.902472;-0.219130;0.370845;,
  -0.913945;-0.044898;0.403347;,
  -0.912589;-0.408877;-0.000000;,
  -0.000000;-0.533572;0.845755;,
  -0.000001;-0.855298;-0.518136;,
  -0.000000;-0.533572;-0.845755;,
  0.000000;-0.066321;0.997798;,
  -0.901325;0.028293;-0.432219;,
  0.000000;-0.066323;-0.997798;,
  -0.996585;0.082575;-0.000000;,
  -0.999429;-0.030205;-0.015180;,
  -0.994024;-0.056564;0.093369;,
  -0.982329;0.164250;0.089735;,
  -0.228747;-0.832623;0.504395;,
  -0.000001;-0.855300;0.518133;,
  0.000000;-0.420989;-0.907066;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;-0.420989;0.907066;,
  0.000000;-0.763729;0.645537;,
  0.000000;-0.763729;-0.645537;,
  0.228746;-0.832623;0.504395;,
  0.355051;-0.498808;0.790651;,
  0.355057;-0.498807;-0.790650;,
  0.228748;-0.832621;-0.504398;,
  0.407043;-0.060580;-0.911398;,
  0.407034;-0.060579;0.911402;;
  36;
  4;24,2,1,21;,
  4;4,4,4,4;,
  4;23,5,25,26;,
  4;22,7,5,23;,
  3;5,9,25;,
  3;25,12,27;,
  3;27,12,11;,
  3;12,25,9;,
  3;5,28,9;,
  3;9,28,8;,
  4;3,3,6,6;,
  4;4,4,4,4;,
  3;28,29,8;,
  3;8,29,10;,
  3;11,13,27;,
  3;27,13,30;,
  3;10,1,13;,
  3;13,1,2;,
  4;21,31,31,32;,
  4;6,0,0,6;,
  4;14,17,16,15;,
  4;18,19,17,14;,
  3;18,14,20;,
  3;20,14,15;,
  4;33,33,34,34;,
  4;4,4,4,4;,
  4;4,4,4,4;,
  4;35,35,36,36;,
  4;36,36,37,37;,
  4;38,38,33,33;,
  4;39,40,21,32;,
  4;41,42,22,23;,
  4;43,41,23,26;,
  4;4,4,4,4;,
  4;4,4,4,4;,
  4;40,44,24,21;;
 }
}
