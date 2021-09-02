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
 120;
 -13.51868;-9.79709;9.66027;,
 13.14921;-9.79709;9.66027;,
 8.32712;-0.03989;6.13157;,
 -8.69648;-0.03989;6.13157;,
 8.32712;-0.03989;-6.32533;,
 -8.69648;-0.03989;-6.32533;,
 13.14921;-9.79709;-9.85393;,
 -13.51868;-9.79709;-9.85393;,
 -8.69648;-19.55419;-6.32533;,
 8.32712;-19.55419;-6.32533;,
 8.32712;-19.55419;6.13157;,
 -8.69648;-19.55419;6.13157;,
 -6.18938;-11.02639;19.03607;,
 -10.72588;-11.02639;13.52667;,
 -10.72588;-27.06609;13.52667;,
 -6.18938;-27.06609;19.03607;,
 -8.71958;-11.02639;-20.26893;,
 -10.72588;-11.02639;-16.77533;,
 -10.72588;-27.06609;-16.77533;,
 -7.28758;-27.06609;-22.28473;,
 -2.72329;-69.62968;9.22417;,
 -5.10518;-69.62968;6.33117;,
 -5.10528;-69.62968;-9.57973;,
 -2.72329;-69.62968;-12.47263;,
 -23.35369;-13.37499;5.49217;,
 -25.48648;7.67961;5.49217;,
 -25.48648;7.67961;-8.74063;,
 -23.35369;-13.37499;-8.74063;,
 19.16821;-27.06559;-8.55183;,
 20.27312;-11.02639;-8.74063;,
 20.27321;-11.02639;5.49217;,
 20.27321;-27.06609;5.49217;,
 4.22422;-69.62968;9.22417;,
 4.22422;-69.62968;-12.47263;,
 -3.87419;8.92921;-29.49373;,
 -3.87419;24.96901;-25.67133;,
 4.72662;24.96901;-25.67133;,
 4.72662;8.92921;-29.49373;,
 9.57201;-11.02639;-20.26893;,
 7.04192;-11.02639;19.03607;,
 7.04192;-27.06609;19.03607;,
 7.69802;-69.62968;6.33117;,
 7.69802;-69.62968;-9.57973;,
 8.14002;-27.06609;-22.28473;,
 13.65752;-11.02639;-16.77533;,
 13.69062;-27.06559;-16.67553;,
 13.65752;-11.02639;13.52667;,
 13.65752;-27.06609;13.52667;,
 11.17171;-69.62968;-5.36083;,
 11.17171;-69.62968;2.11237;,
 13.81962;-27.06609;13.32987;,
 -11.00608;-26.76239;13.34847;,
 -8.67818;-69.62968;2.11237;,
 -8.67818;-69.62968;-5.36083;,
 12.85342;-37.73680;16.01247;,
 12.83032;-38.04280;12.55427;,
 11.30562;-58.27410;13.60187;,
 11.29191;-58.45690;10.74577;,
 12.08972;-47.87000;11.68377;,
 12.12312;-47.42730;14.47567;,
 10.99131;-62.44570;10.39247;,
 11.13272;-62.44570;10.22067;,
 6.65052;-37.73680;21.17817;,
 -5.75528;-37.73680;21.17817;,
 -10.00869;-37.73680;16.01247;,
 -13.41858;-54.00370;5.29367;,
 -9.92978;-55.59050;10.60617;,
 -12.16218;-29.86119;12.42817;,
 -19.75278;-20.19499;5.07677;,
 -14.70018;-60.19960;4.01997;,
 -14.70018;-60.19960;-7.26843;,
 -8.34789;-62.44570;-13.64083;,
 -11.15749;-38.42920;-14.75943;,
 -10.03298;-54.49680;-13.21663;,
 -16.92838;-33.77730;-10.45673;,
 -14.77568;-50.53070;-10.47643;,
 -5.00368;-58.90750;-18.43793;,
 -4.74988;-62.44570;-18.01063;,
 6.50572;-27.06609;-22.28473;,
 5.98382;-58.90750;-18.43793;,
 2.13821;-58.90750;-18.43793;,
 11.12072;-62.17440;-13.74883;,
 5.74422;-62.44570;-18.01063;,
 16.13931;-62.17440;-7.76393;,
 16.23832;-62.44570;4.01997;,
 5.74422;-62.44570;14.76217;,
 -4.74988;-62.44570;14.76217;,
 -8.34789;-62.44570;10.39247;,
 -8.48568;-62.39690;10.25427;,
 -4.89818;-1.86949;-27.33103;,
 -2.76839;-1.86949;-27.33103;,
 3.62091;-1.86949;-27.33103;,
 5.75062;-1.86949;-27.33103;,
 -1.70749;-58.90750;-18.43793;,
 -5.74478;-27.06609;-22.28473;,
 5.13562;-27.44539;-25.39543;,
 -4.37478;-27.44539;-25.39543;,
 -2.06409;-5.12569;-29.86553;,
 2.89611;-5.12569;-29.86553;,
 1.74501;-55.65140;-21.98803;,
 -1.24039;-55.65140;-21.98803;,
 -16.54498;-33.77730;-9.91103;,
 -10.77409;-38.42920;-14.21363;,
 -9.64958;-54.49680;-12.67093;,
 -14.39228;-50.53070;-9.93073;,
 6.63932;-38.04280;17.71017;,
 -5.74278;-38.04280;17.71017;,
 -9.98808;-38.04280;12.55427;,
 6.29502;-47.42730;19.32917;,
 6.27882;-47.87000;16.52297;,
 -5.36098;-47.42730;19.32917;,
 -5.34298;-47.87000;16.52297;,
 -9.35728;-47.42730;14.47567;,
 -9.32759;-47.87000;11.68377;,
 5.89722;-58.27410;18.10607;,
 -4.91958;-58.27410;18.10607;,
 -8.62828;-58.27410;13.60187;,
 -8.61598;-58.45690;10.74577;,
 5.89052;-58.45690;15.24397;,
 -4.91218;-58.45690;15.24397;;
 
 143;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;8,9,10,11;,
 4;1,6,4,2;,
 4;7,0,3,5;,
 4;11,10,1,0;,
 4;10,9,6,1;,
 4;7,6,9,8;,
 4;8,11,0,7;,
 4;12,13,14,15;,
 4;16,17,13,12;,
 4;18,17,16,19;,
 4;20,21,22,23;,
 4;24,25,26,27;,
 4;28,29,30,31;,
 4;32,20,23,33;,
 4;34,35,36,37;,
 4;38,16,12,39;,
 4;39,12,15,40;,
 4;41,32,33,42;,
 4;43,38,44,45;,
 4;44,38,39,46;,
 4;46,39,40,47;,
 4;48,49,41,42;,
 4;29,28,45,44;,
 4;30,29,44,46;,
 3;46,47,30;,
 3;47,50,30;,
 3;30,50,31;,
 3;25,24,13;,
 3;24,51,13;,
 3;13,51,14;,
 4;17,26,25,13;,
 4;27,26,17,18;,
 4;21,52,53,22;,
 3;54,55,47;,
 3;56,57,58;,
 3;59,58,55;,
 3;60,61,57;,
 3;57,61,58;,
 3;58,61,55;,
 3;61,50,55;,
 3;55,50,47;,
 4;40,62,54,47;,
 4;15,63,62,40;,
 4;14,64,63,15;,
 4;65,66,67,68;,
 4;69,24,27,70;,
 3;18,71,72;,
 3;72,71,73;,
 3;27,74,70;,
 3;74,75,70;,
 3;73,71,75;,
 3;75,71,70;,
 3;19,76,18;,
 3;18,76,71;,
 3;71,76,77;,
 4;78,43,79,80;,
 3;81,82,45;,
 3;82,79,45;,
 3;45,79,43;,
 4;28,83,81,45;,
 4;83,28,31,84;,
 3;41,49,60;,
 3;60,49,61;,
 3;61,49,84;,
 4;85,32,41,60;,
 4;86,20,32,85;,
 4;87,21,20,86;,
 3;69,52,88;,
 3;52,21,88;,
 3;88,21,87;,
 4;52,69,70,53;,
 4;22,53,70,71;,
 4;23,22,71,77;,
 4;33,23,77,82;,
 4;42,33,82,81;,
 4;83,48,42,81;,
 4;48,83,84,49;,
 3;89,34,90;,
 3;34,37,90;,
 3;90,37,91;,
 3;91,37,92;,
 3;38,43,36;,
 3;43,92,36;,
 3;36,92,37;,
 4;38,36,35,16;,
 3;34,89,35;,
 3;89,19,35;,
 3;35,19,16;,
 4;43,78,91,92;,
 3;76,93,77;,
 3;77,93,82;,
 3;93,80,82;,
 3;82,80,79;,
 4;94,19,89,90;,
 4;95,96,97,98;,
 4;96,95,99,100;,
 4;76,19,94,93;,
 4;78,95,98,91;,
 4;91,98,97,90;,
 4;90,97,96,94;,
 4;94,96,100,93;,
 4;93,100,99,80;,
 4;80,99,95,78;,
 4;101,102,103,104;,
 4;74,101,104,75;,
 4;75,104,103,73;,
 4;73,103,102,72;,
 4;72,102,101,74;,
 4;62,105,55,54;,
 4;63,106,105,62;,
 4;64,107,106,63;,
 4;108,109,58,59;,
 4;110,111,109,108;,
 4;112,113,111,110;,
 4;105,108,59,55;,
 4;106,110,108,105;,
 4;107,112,110,106;,
 4;109,114,56,58;,
 4;111,115,114,109;,
 4;113,116,115,111;,
 4;84,31,50,61;,
 3;51,88,14;,
 3;14,107,64;,
 3;14,88,107;,
 3;107,113,112;,
 3;107,88,113;,
 3;88,117,113;,
 3;113,117,116;,
 3;117,88,87;,
 4;114,118,57,56;,
 4;115,119,118,114;,
 4;116,117,119,115;,
 4;118,85,60,57;,
 4;119,86,85,118;,
 4;117,87,86,119;,
 4;69,65,68,24;,
 4;24,68,67,51;,
 4;51,67,66,88;,
 4;88,66,65,69;,
 4;27,18,72,74;;
 
 MeshMaterialList {
  10;
  143;
  9,
  9,
  9,
  0,
  9,
  9,
  0,
  0,
  0,
  0,
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
  5,
  5,
  0,
  5,
  5,
  5,
  5,
  5,
  5,
  0,
  0,
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
  5,
  5,
  5,
  5,
  5,
  5,
  0,
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
  7,
  7,
  5,
  7,
  7,
  7,
  7,
  7,
  7,
  9,
  0,
  0,
  5,
  0,
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
  5,
  5,
  5,
  0,
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
  144;
  0.000000;-1.000000;-0.000000;,
  0.000000;0.340093;0.940392;,
  0.000000;1.000000;0.000000;,
  0.000000;-0.000002;1.000000;,
  0.000000;-0.000002;-1.000000;,
  0.595818;-0.727939;0.339273;,
  -0.603364;-0.744256;0.286419;,
  0.871156;-0.009017;0.490923;,
  -0.870654;-0.088196;0.483925;,
  0.942141;-0.034350;-0.333451;,
  -0.872617;-0.072861;-0.482940;,
  0.547849;-0.812224;-0.200385;,
  -0.569962;-0.798345;-0.194395;,
  -0.179815;-0.909806;0.374058;,
  -0.179813;-0.909805;-0.374061;,
  -0.426818;0.000000;0.904338;,
  0.146357;-0.904858;0.399765;,
  0.147008;-0.904733;-0.399809;,
  0.322146;-0.890914;0.320146;,
  0.322089;-0.890683;-0.320848;,
  0.744205;-0.010257;-0.667872;,
  -0.432036;-0.835261;0.340125;,
  -0.599420;-0.013525;0.800321;,
  0.436202;0.899849;0.000000;,
  -0.362570;-0.885478;-0.290639;,
  0.946099;-0.065950;0.317092;,
  0.800237;-0.010322;0.599595;,
  0.339930;0.087085;0.936410;,
  -0.427014;0.081104;0.900601;,
  -0.776727;-0.033721;0.628935;,
  -0.766371;-0.114428;0.632125;,
  -0.844708;-0.127708;-0.519768;,
  -0.726975;-0.083802;-0.681532;,
  -0.878031;-0.017359;-0.478289;,
  0.592998;-0.071296;-0.802042;,
  0.721887;-0.082289;-0.687100;,
  0.939709;-0.068122;-0.335121;,
  0.878634;-0.371946;0.299429;,
  0.657966;-0.409799;0.631779;,
  0.314620;-0.391380;0.864775;,
  -0.394126;-0.399230;0.827816;,
  -0.722058;-0.352559;0.595260;,
  -0.885796;-0.355775;0.297974;,
  -0.859932;-0.315960;-0.400857;,
  -0.701687;-0.243206;-0.669691;,
  -0.313639;-0.350406;-0.882523;,
  0.287992;-0.280860;-0.915521;,
  0.666246;-0.357522;-0.654442;,
  0.875977;-0.363307;-0.317289;,
  0.000000;-0.054772;-0.998499;,
  0.000000;-0.090904;-0.995860;,
  0.798958;0.100230;-0.592976;,
  -0.431893;0.134479;-0.891843;,
  0.000000;-0.196376;-0.980529;,
  0.000000;-0.196376;-0.980529;,
  0.000000;-0.036330;-0.999340;,
  0.000000;0.006728;-0.999977;,
  -0.530477;-0.133931;-0.837052;,
  0.381199;-0.136263;-0.914396;,
  0.000000;-0.158277;-0.987395;,
  0.000000;-0.158277;-0.987395;,
  0.000000;-0.253262;-0.967398;,
  0.000000;-0.287446;-0.957797;,
  0.000000;0.229801;-0.973238;,
  0.000000;-0.158273;-0.987395;,
  0.000000;-0.456800;-0.889569;,
  -0.765281;-0.132434;-0.629926;,
  -0.762663;-0.142086;-0.630997;,
  -0.668074;-0.116260;-0.734957;,
  -0.635718;-0.093878;-0.766192;,
  -0.603505;-0.099549;-0.791120;,
  0.905669;0.030270;0.422903;,
  0.337007;0.173408;0.925395;,
  -0.424399;0.161586;0.890941;,
  -0.941304;0.015291;0.337214;,
  -0.184601;-0.970989;0.151999;,
  0.905676;0.018468;0.423568;,
  0.064183;-0.985804;0.155147;,
  -0.092700;-0.983622;0.154580;,
  0.878216;-0.068216;0.473374;,
  0.036428;-0.995479;0.087718;,
  -0.052918;-0.994751;0.087581;,
  -0.105689;-0.990584;0.087023;,
  0.897437;-0.061342;0.436857;,
  0.770127;-0.068334;0.634220;,
  0.746795;-0.253697;0.614764;,
  -0.566057;-0.085913;0.819877;,
  -0.688148;-0.249662;0.681265;,
  -0.941263;-0.001012;0.337672;,
  0.905623;0.009915;0.423968;,
  0.338613;0.129389;0.931987;,
  -0.425755;0.119155;0.896959;,
  -0.844628;-0.103303;0.525293;,
  0.837177;-0.110537;0.535646;,
  0.026463;-0.997621;0.063656;,
  -0.038499;-0.997233;0.063593;,
  -0.636989;-0.086522;0.766002;,
  -0.623997;-0.136770;0.769364;,
  -0.658508;-0.151151;0.737238;,
  -0.671373;-0.102048;0.734060;,
  0.000000;0.340085;-0.940395;,
  1.000000;-0.000002;-0.000000;,
  0.896494;0.443056;0.000000;,
  -1.000000;-0.000002;0.000000;,
  -0.896490;0.443063;0.000000;,
  0.000000;-0.340096;0.940391;,
  0.896492;-0.443059;-0.000000;,
  0.000000;-0.340088;-0.940394;,
  -0.896489;-0.443067;0.000000;,
  -0.715072;-0.016143;-0.698864;,
  -0.819197;0.002658;-0.573507;,
  0.340274;0.000000;0.940326;,
  0.730387;0.010368;-0.682955;,
  0.709071;0.000000;0.705137;,
  0.785032;0.619456;0.000000;,
  0.976177;0.050298;-0.211067;,
  0.976178;0.050297;-0.211061;,
  0.000000;0.148423;0.988924;,
  -0.976178;0.050297;-0.211061;,
  -0.976177;0.050298;-0.211067;,
  0.936810;-0.066304;-0.343498;,
  0.930565;0.026935;-0.365135;,
  -0.930863;0.030355;-0.364106;,
  -0.936266;-0.060328;-0.346073;,
  -0.933638;-0.150494;-0.325072;,
  0.934436;-0.158932;-0.318700;,
  0.813695;0.105227;-0.571689;,
  0.708523;0.500194;-0.497797;,
  0.481117;0.808867;-0.338025;,
  -0.818261;-0.002072;0.574844;,
  -0.718298;-0.478967;0.504618;,
  -0.444253;-0.839783;0.312096;,
  0.072808;-0.993507;0.087426;,
  0.128101;-0.979759;0.153824;,
  0.338001;0.147949;0.929444;,
  -0.425251;0.137041;0.894640;,
  -0.901931;-0.058715;0.427871;,
  -0.941301;0.005833;0.337519;,
  -0.901737;-0.061450;0.427895;,
  0.052907;-0.996577;0.063529;,
  -0.076941;-0.995021;0.063353;,
  0.337690;-0.125070;0.932911;,
  -0.423744;-0.125592;0.897033;,
  -0.201713;-0.051315;0.978100;;
  143;
  4;3,3,1,1;,
  4;2,2,2,2;,
  4;100,100,4,4;,
  4;0,0,0,0;,
  4;101,101,102,102;,
  4;103,103,104,104;,
  4;105,105,3,3;,
  4;106,106,101,101;,
  4;4,4,107,107;,
  4;108,108,103,103;,
  4;15,22,29,28;,
  4;2,23,23,2;,
  4;32,109,110,33;,
  4;13,21,24,14;,
  4;30,8,10,31;,
  4;36,9,7,25;,
  4;16,13,14,17;,
  4;49,52,51,50;,
  4;2,2,2,2;,
  4;111,15,28,27;,
  4;18,16,17,19;,
  4;34,112,20,35;,
  4;2,2,2,2;,
  4;113,111,27,26;,
  4;11,5,18,19;,
  4;9,36,35,20;,
  4;2,2,2,2;,
  3;113,26,7;,
  3;26,84,7;,
  3;7,84,25;,
  3;8,30,22;,
  3;30,86,22;,
  3;22,86,29;,
  4;23,114,114,23;,
  4;31,10,109,32;,
  4;21,6,12,24;,
  3;71,79,26;,
  3;89,93,83;,
  3;76,83,79;,
  3;38,85,93;,
  3;93,85,83;,
  3;83,85,79;,
  3;85,84,79;,
  3;79,84,26;,
  4;27,72,71,26;,
  4;28,73,72,27;,
  4;29,74,73,28;,
  4;96,99,98,97;,
  4;42,30,31,43;,
  3;32,44,68;,
  3;68,44,69;,
  3;31,66,43;,
  3;66,67,43;,
  3;69,44,67;,
  3;67,44,43;,
  3;33,57,32;,
  3;32,57,44;,
  3;44,57,45;,
  4;60,34,58,62;,
  3;47,46,35;,
  3;46,58,35;,
  3;35,58,34;,
  4;36,48,47,35;,
  4;48,36,25,37;,
  3;18,5,38;,
  3;38,5,85;,
  3;85,5,37;,
  4;39,16,18,38;,
  4;40,13,16,39;,
  4;41,21,13,40;,
  3;42,6,87;,
  3;6,21,87;,
  3;87,21,41;,
  4;6,42,43,12;,
  4;24,12,43,44;,
  4;14,24,44,45;,
  4;17,14,45,46;,
  4;19,17,46,47;,
  4;48,11,19,47;,
  4;11,48,37,5;,
  3;53,49,55;,
  3;49,50,55;,
  3;55,50,56;,
  3;56,50,54;,
  3;112,34,51;,
  3;34,115,51;,
  3;51,115,116;,
  4;117,117,117,117;,
  3;118,119,119;,
  3;119,33,119;,
  3;52,33,110;,
  4;34,60,56,54;,
  3;57,61,45;,
  3;45,61,46;,
  3;61,62,46;,
  3;46,62,58;,
  4;59,59,53,55;,
  4;64,64,63,63;,
  4;64,64,65,65;,
  4;57,59,59,61;,
  4;120,120,121,121;,
  4;56,63,63,55;,
  4;122,122,123,123;,
  4;123,123,124,124;,
  4;61,65,65,62;,
  4;125,125,120,120;,
  4;70,70,70,70;,
  4;126,126,127,127;,
  4;127,127,128,128;,
  4;129,129,130,130;,
  4;130,130,131,131;,
  4;80,80,132,132;,
  4;81,81,80,80;,
  4;82,82,81,81;,
  4;77,77,133,133;,
  4;78,78,77,77;,
  4;75,75,78,78;,
  4;134,134,76,79;,
  4;135,135,134,134;,
  4;136,137,135,135;,
  4;90,90,89,83;,
  4;91,91,90,90;,
  4;138,88,91,91;,
  4;37,25,84,85;,
  3;86,87,29;,
  3;29,136,74;,
  3;29,87,136;,
  3;136,138,137;,
  3;136,87,138;,
  3;87,92,138;,
  3;138,92,88;,
  3;92,87,41;,
  4;94,94,139,139;,
  4;95,95,94,94;,
  4;140,140,95,95;,
  4;141,39,38,93;,
  4;142,40,39,141;,
  4;92,41,40,142;,
  4;143,96,97,30;,
  4;30,97,98,86;,
  4;86,98,99,87;,
  4;87,99,96,42;,
  4;31,32,68,66;;
 }
}
