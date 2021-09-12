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
 179;
 1.15370;-15.71262;-6.13364;,
 1.97670;-27.29462;-4.60944;,
 -4.22710;-27.18312;-0.84204;,
 -5.22360;-15.39062;-2.22374;,
 2.78770;-39.59802;-3.36534;,
 -2.46420;-39.67152;-0.06144;,
 2.72950;-38.89302;14.98996;,
 1.95070;-25.11932;17.42136;,
 -4.22710;-25.27982;16.64166;,
 -2.28660;-39.05442;14.20626;,
 1.10120;-12.56652;16.48426;,
 -5.23980;-12.64472;15.73356;,
 4.26400;-75.31870;7.20346;,
 2.40430;-73.94580;6.99576;,
 1.85570;-75.44521;4.53256;,
 4.32370;-77.20599;3.37176;,
 -8.40340;-13.03512;12.05376;,
 -7.22520;-25.81632;12.58656;,
 -4.22710;-25.27982;16.64166;,
 -5.23980;-12.64472;15.73356;,
 -4.47300;-39.37982;10.59066;,
 -2.28660;-39.05442;14.20626;,
 7.44780;-38.81952;14.04196;,
 9.59890;-39.11112;9.60576;,
 10.92140;-26.08052;11.66206;,
 7.98640;-25.14452;16.48756;,
 7.45480;-12.68282;15.72946;,
 10.69360;-14.05182;11.42226;,
 3.45460;-53.80322;-2.25984;,
 4.02670;-69.65649;-1.52904;,
 1.26580;-69.71820;0.64476;,
 -0.54640;-53.84892;0.53076;,
 4.24750;-75.31870;-0.76424;,
 1.93300;-73.94580;0.83106;,
 -1.64780;-53.85542;8.05156;,
 0.81670;-69.72690;5.53036;,
 1.84840;-69.71820;7.85686;,
 -0.10440;-53.84892;10.84756;,
 1.85570;-75.44521;4.53256;,
 2.40430;-73.94580;6.99576;,
 4.04010;-69.65649;8.58336;,
 3.42510;-53.80322;11.58016;,
 -0.10440;-53.84892;10.84756;,
 1.84840;-69.71820;7.85686;,
 6.29040;-69.59500;7.93396;,
 6.05620;-73.75560;7.15716;,
 6.66430;-75.19209;5.07736;,
 7.28410;-69.58620;5.99316;,
 6.80330;-53.75752;10.76506;,
 8.32780;-53.75092;7.55736;,
 -7.05830;-3.59822;6.62976;,
 -5.25800;-3.31682;10.09036;,
 0.32930;-5.20812;-7.68054;,
 -5.24060;-5.12382;-4.49154;,
 5.82780;-3.06212;10.17186;,
 7.76170;-3.86682;6.53046;,
 0.27210;-3.12212;10.88086;,
 -5.25800;-3.31682;10.09036;,
 6.66430;-75.19209;5.07736;,
 6.05620;-73.75560;7.15716;,
 6.29040;-69.59500;7.93396;,
 6.80330;-53.75752;10.76506;,
 7.44780;-38.81952;14.04196;,
 7.98640;-25.14452;16.48756;,
 7.45480;-12.68282;15.72946;,
 5.82780;-3.06212;10.17186;,
 7.53060;-15.44282;-2.30804;,
 8.14180;-27.04812;-0.99614;,
 5.90830;-4.88122;-4.48784;,
 7.97440;-39.43642;-0.22554;,
 7.42220;-53.75752;0.44836;,
 6.79340;-69.59500;0.72186;,
 6.45010;-73.75560;0.99246;,
 -7.46820;-14.21692;4.97026;,
 -6.35570;-26.53262;5.88336;,
 -6.63650;-4.37292;1.07916;,
 -3.97220;-39.55602;5.27376;,
 -1.47290;-53.85542;4.32586;,
 0.78210;-69.72690;3.10686;,
 1.78080;-75.44521;2.40256;,
 1.93300;-73.94580;0.83106;,
 4.24750;-75.31870;-0.76424;,
 4.32120;-77.20599;1.08676;,
 1.78080;-75.44521;2.40256;,
 6.71990;-75.19209;2.76576;,
 6.45010;-73.75560;0.99246;,
 7.29420;-69.58620;3.41536;,
 8.24890;-53.75092;3.99636;,
 6.71990;-75.19209;2.76576;,
 9.28680;-39.28742;4.61716;,
 10.15480;-26.62422;5.26716;,
 9.77300;-14.84072;4.55136;,
 7.33390;-4.41792;1.01476;,
 3.99970;1.56088;-5.31194;,
 4.78420;0.48068;-5.60844;,
 -0.06390;0.17578;-7.93184;,
 -0.13190;1.25368;-7.01204;,
 5.52700;0.75688;-1.83024;,
 4.51550;1.73748;-2.16174;,
 4.51730;1.93588;1.98406;,
 5.59390;1.01568;2.56546;,
 -4.90820;-0.06162;-5.60804;,
 -4.23440;1.05618;-5.29994;,
 4.02030;2.03328;4.90846;,
 4.76680;1.24558;6.10626;,
 5.59390;1.01568;2.56546;,
 4.51730;1.93588;1.98406;,
 4.76680;1.24558;6.10626;,
 -0.07490;1.02388;6.84166;,
 4.02030;2.03328;4.90846;,
 -0.12960;1.80678;5.39556;,
 -4.90680;0.69528;6.09846;,
 -4.26410;1.52598;4.89636;,
 -5.64970;0.17738;-1.82364;,
 -4.73750;1.18698;-2.16194;,
 -5.71280;0.45378;2.57526;,
 -4.73460;1.39188;1.97946;,
 -5.71280;0.45378;2.57526;,
 3.99970;1.56088;-5.31194;,
 -0.13190;1.25368;-7.01204;,
 -0.19580;2.17638;-5.79854;,
 3.05380;2.53518;-4.82034;,
 4.51550;1.73748;-2.16174;,
 3.99970;1.56088;-5.31194;,
 3.05380;2.53518;-4.82034;,
 3.29520;2.64968;-2.29944;,
 4.51730;1.93588;1.98406;,
 4.51550;1.73748;-2.16174;,
 3.29520;2.64968;-2.29944;,
 3.22810;2.78208;1.55926;,
 -0.13190;1.25368;-7.01204;,
 -4.23440;1.05618;-5.29994;,
 -3.38620;2.08008;-4.78874;,
 -0.19580;2.17638;-5.79854;,
 4.02030;2.03328;4.90846;,
 4.51730;1.93588;1.98406;,
 3.22810;2.78208;1.55926;,
 3.12530;2.67358;3.67126;,
 -0.12960;1.80678;5.39556;,
 4.02030;2.03328;4.90846;,
 3.12530;2.67358;3.67126;,
 -0.17880;2.43078;3.83916;,
 -4.26410;1.52598;4.89636;,
 -0.12960;1.80678;5.39556;,
 -0.17880;2.43078;3.83916;,
 -3.46310;2.22958;3.65806;,
 -4.73750;1.18698;-2.16194;,
 -4.73460;1.39188;1.97946;,
 -3.53690;2.29428;1.56186;,
 -3.60630;2.15428;-2.29124;,
 -4.23440;1.05618;-5.29994;,
 -4.73750;1.18698;-2.16194;,
 -3.60630;2.15428;-2.29124;,
 -3.38620;2.08008;-4.78874;,
 -4.73460;1.39188;1.97946;,
 -4.26410;1.52598;4.89636;,
 -3.46310;2.22958;3.65806;,
 -3.53690;2.29428;1.56186;,
 3.05380;2.53518;-4.82034;,
 -0.19580;2.17638;-5.79854;,
 -0.16970;3.15098;-0.16664;,
 3.29520;2.64968;-2.29944;,
 3.05380;2.53518;-4.82034;,
 3.22810;2.78208;1.55926;,
 3.29520;2.64968;-2.29944;,
 -0.19580;2.17638;-5.79854;,
 -3.38620;2.08008;-4.78874;,
 3.12530;2.67358;3.67126;,
 3.22810;2.78208;1.55926;,
 -0.17880;2.43078;3.83916;,
 3.12530;2.67358;3.67126;,
 -3.46310;2.22958;3.65806;,
 -0.17880;2.43078;3.83916;,
 -3.60630;2.15428;-2.29124;,
 -3.53690;2.29428;1.56186;,
 -3.38620;2.08008;-4.78874;,
 -3.60630;2.15428;-2.29124;,
 -3.53690;2.29428;1.56186;,
 -3.46310;2.22958;3.65806;;
 
 106;
 4;0,1,2,3;,
 4;2,1,4,5;,
 4;6,7,8,9;,
 4;7,10,11,8;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;18,17,20,21;,
 4;22,23,24,25;,
 4;26,25,24,27;,
 4;28,29,30,31;,
 4;29,32,33,30;,
 4;34,35,36,37;,
 4;35,38,39,36;,
 4;40,41,42,43;,
 4;42,41,6,9;,
 4;44,45,46,47;,
 4;48,44,47,49;,
 4;50,16,19,51;,
 4;52,0,3,53;,
 4;54,26,27,55;,
 4;4,28,31,5;,
 4;20,34,37,21;,
 4;13,12,40,43;,
 4;22,48,49,23;,
 4;11,10,56,57;,
 4;12,15,58,59;,
 4;41,40,60,61;,
 4;40,12,59,60;,
 4;6,41,61,62;,
 4;7,6,62,63;,
 4;10,7,63,64;,
 4;65,56,10,64;,
 4;1,0,66,67;,
 4;68,66,0,52;,
 4;4,1,67,69;,
 4;28,4,69,70;,
 4;71,29,28,70;,
 4;71,72,32,29;,
 4;73,3,2,74;,
 4;17,16,73,74;,
 4;73,16,50,75;,
 4;3,73,75,53;,
 4;74,2,5,76;,
 4;20,17,74,76;,
 4;76,5,31,77;,
 4;34,20,76,77;,
 4;77,31,30,78;,
 4;35,34,77,78;,
 4;33,79,78,30;,
 4;79,38,35,78;,
 4;80,81,82,83;,
 4;82,15,14,83;,
 4;84,58,15,82;,
 4;84,82,81,85;,
 4;49,47,86,87;,
 4;47,46,88,86;,
 4;86,88,72,71;,
 4;86,71,70,87;,
 4;23,49,87,89;,
 4;87,70,69,89;,
 4;24,23,89,90;,
 4;89,69,67,90;,
 4;27,24,90,91;,
 4;90,67,66,91;,
 4;27,91,92,55;,
 4;91,66,68,92;,
 4;93,94,95,96;,
 4;68,52,95,94;,
 4;94,97,92,68;,
 4;94,93,98,97;,
 4;97,98,99,100;,
 4;97,100,55,92;,
 4;96,95,101,102;,
 4;101,95,52,53;,
 4;103,104,105,106;,
 4;54,55,105,104;,
 4;65,107,108,56;,
 4;107,109,110,108;,
 4;56,108,111,57;,
 4;108,110,112,111;,
 4;102,101,113,114;,
 4;113,101,53,75;,
 4;115,113,75,50;,
 4;114,113,115,116;,
 4;116,117,111,112;,
 4;111,117,50,51;,
 4;118,119,120,121;,
 4;122,123,124,125;,
 4;126,127,128,129;,
 4;130,131,132,133;,
 4;134,135,136,137;,
 4;138,139,140,141;,
 4;142,143,144,145;,
 4;146,147,148,149;,
 4;150,151,152,153;,
 4;154,155,156,157;,
 3;158,159,160;,
 3;161,162,160;,
 3;163,164,160;,
 3;165,166,160;,
 3;167,168,160;,
 3;169,170,160;,
 3;171,172,160;,
 3;173,174,160;,
 3;175,176,160;,
 3;177,178,160;;
 
 MeshMaterialList {
  9;
  106;
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
  6,
  6,
  6,
  6,
  6,
  6;;
  Material {
   0.401600;0.401600;0.401600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.608800;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.787200;0.800000;0.016000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.633600;0.373600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.674400;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.184800;0.184800;0.184800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.348000;0.348000;0.398400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  111;
  0.828763;-0.225149;-0.512309;,
  -0.792012;-0.285284;-0.539750;,
  0.603224;-0.399285;0.690429;,
  -0.617585;-0.440749;0.651406;,
  -0.654286;-0.337083;-0.676967;,
  -0.010853;-0.557832;0.829883;,
  -0.807858;-0.538762;0.238958;,
  0.024118;-0.951699;0.306083;,
  0.836385;-0.482787;0.259571;,
  0.504258;0.723764;-0.471051;,
  -0.041356;0.658516;-0.751429;,
  0.310727;0.906089;0.287143;,
  -0.054944;0.888356;0.455855;,
  0.608221;0.793497;0.020711;,
  -0.571286;0.680876;-0.458301;,
  -0.662580;0.748752;0.018917;,
  -0.399025;0.874004;0.277300;,
  0.784223;-0.012947;-0.620344;,
  0.785644;-0.041775;-0.617267;,
  0.800698;-0.053940;-0.596635;,
  0.827218;-0.047940;-0.559832;,
  0.850171;-0.065881;-0.522369;,
  -0.868030;-0.457027;-0.194036;,
  -0.648731;-0.708715;-0.277259;,
  0.905188;-0.384400;-0.181304;,
  0.710134;0.382385;-0.591177;,
  -0.017075;0.272774;-0.961927;,
  0.665468;0.740585;-0.093197;,
  0.797543;0.600615;0.056444;,
  -0.751117;0.299516;-0.588315;,
  0.436002;0.789809;0.431398;,
  -0.041138;0.789553;0.612302;,
  -0.494804;0.753805;0.432373;,
  -0.839932;0.538429;0.067890;,
  -0.715024;0.693380;-0.089251;,
  -0.019570;0.610971;0.791411;,
  -0.523797;0.556461;0.644972;,
  -0.946067;0.282873;0.157926;,
  -0.187304;-0.124473;-0.974384;,
  -0.793274;-0.081344;-0.603407;,
  0.920844;0.371408;0.118756;,
  0.512218;0.593848;0.620465;,
  0.786389;0.038739;-0.616516;,
  -0.520516;-0.134020;-0.843269;,
  -0.786669;-0.142795;-0.600634;,
  0.013950;-0.055659;0.998352;,
  -0.499927;-0.104247;0.859771;,
  -0.954292;-0.144541;0.261602;,
  0.975045;-0.039535;0.218459;,
  0.555174;-0.053545;0.830009;,
  -0.603485;-0.097443;-0.791398;,
  -0.836667;-0.122855;-0.533756;,
  -0.959974;-0.187503;0.208068;,
  -0.637080;-0.230592;0.735497;,
  0.012735;-0.210173;0.977581;,
  -0.583590;-0.238767;0.776153;,
  0.969379;-0.108216;0.220440;,
  0.630253;-0.174467;0.756533;,
  -0.966507;0.077159;0.244765;,
  -0.504799;0.248902;0.826575;,
  -0.512690;-0.160006;-0.843532;,
  -0.789522;-0.127492;-0.600334;,
  0.965045;0.180906;0.189633;,
  0.534650;0.294841;0.791971;,
  -0.571560;-0.081671;-0.816486;,
  -0.820477;-0.113195;-0.560361;,
  -0.955508;-0.196746;0.219762;,
  -0.009654;-0.210367;0.977575;,
  0.972527;-0.096922;0.211655;,
  0.620544;-0.173291;0.764785;,
  -0.000798;0.299615;0.954060;,
  -0.535556;-0.110703;-0.837213;,
  -0.796438;-0.139426;-0.588427;,
  -0.947538;-0.217225;0.234490;,
  -0.527355;-0.242826;0.814206;,
  0.021876;-0.204068;0.978712;,
  0.972232;-0.111247;0.205886;,
  0.591244;-0.183935;0.785238;,
  -0.979492;-0.049302;-0.195359;,
  -0.971499;-0.136097;-0.194082;,
  -0.971255;-0.163577;-0.172933;,
  -0.979847;-0.144953;-0.137432;,
  -0.983434;-0.148231;-0.104332;,
  0.992187;-0.075387;-0.099405;,
  0.990382;-0.059232;-0.125042;,
  0.984641;-0.060588;-0.163740;,
  0.979121;-0.020019;-0.202290;,
  0.973191;0.085101;-0.213673;,
  0.864473;0.484175;-0.135136;,
  -0.909054;0.395411;-0.131420;,
  -0.983876;0.064167;-0.166945;,
  0.962933;0.201430;-0.179404;,
  -0.062121;0.894701;-0.442325;,
  0.233904;0.932171;-0.276309;,
  0.376442;0.924373;-0.061853;,
  0.337968;0.941032;0.015386;,
  -0.346036;0.900062;-0.264855;,
  0.136167;0.973154;0.185553;,
  -0.063098;0.953281;0.295422;,
  -0.256957;0.950581;0.174267;,
  -0.439084;0.898405;0.008610;,
  -0.471107;0.880247;-0.056775;,
  -0.071011;0.997456;-0.006310;,
  0.511681;-0.064977;-0.856715;,
  0.508914;-0.082687;-0.856837;,
  0.506869;-0.092268;-0.857071;,
  0.526787;-0.054102;-0.848274;,
  0.569900;-0.039746;-0.820752;,
  0.620480;-0.058653;-0.782026;,
  0.707014;-0.285346;-0.647077;,
  0.720937;-0.644011;-0.255930;;
  106;
  4;60,43,44,61;,
  4;44,43,71,72;,
  4;75,45,46,74;,
  4;45,70,59,46;,
  4;5,3,6,7;,
  4;58,47,46,59;,
  4;46,47,73,74;,
  4;77,76,48,49;,
  4;63,49,48,62;,
  4;64,50,51,65;,
  4;50,4,1,51;,
  4;66,52,53,55;,
  4;52,6,3,53;,
  4;67,54,55,53;,
  4;55,54,75,74;,
  4;57,2,8,56;,
  4;69,57,56,68;,
  4;37,58,59,36;,
  4;38,60,61,39;,
  4;41,63,62,40;,
  4;71,64,65,72;,
  4;73,66,55,74;,
  4;3,5,67,53;,
  4;77,69,68,76;,
  4;59,70,35,36;,
  4;5,7,8,2;,
  4;54,67,57,69;,
  4;67,5,2,57;,
  4;75,54,69,77;,
  4;45,75,77,49;,
  4;70,45,49,63;,
  4;41,35,70,63;,
  4;103,104,17,18;,
  4;42,17,104,105;,
  4;106,103,18,19;,
  4;107,106,19,20;,
  4;21,108,107,20;,
  4;21,0,109,108;,
  4;78,61,44,79;,
  4;47,58,78,79;,
  4;78,58,37,90;,
  4;61,78,90,39;,
  4;79,44,72,80;,
  4;73,47,79,80;,
  4;80,72,65,81;,
  4;66,73,80,81;,
  4;81,65,51,82;,
  4;52,66,81,82;,
  4;1,22,82,51;,
  4;22,6,52,82;,
  4;1,4,23,22;,
  4;23,7,6,22;,
  4;24,8,7,110;,
  4;24,110,109,0;,
  4;68,56,83,84;,
  4;56,8,24,83;,
  4;83,24,0,21;,
  4;83,21,20,84;,
  4;76,68,84,85;,
  4;84,20,19,85;,
  4;48,76,85,86;,
  4;85,19,18,86;,
  4;62,48,86,87;,
  4;86,18,17,87;,
  4;62,87,91,40;,
  4;87,17,42,91;,
  4;9,25,26,10;,
  4;42,38,26,25;,
  4;25,88,91,42;,
  4;25,9,27,88;,
  4;88,27,13,28;,
  4;88,28,40,91;,
  4;10,26,29,14;,
  4;29,26,38,39;,
  4;11,30,28,13;,
  4;41,40,28,30;,
  4;41,30,31,35;,
  4;30,11,12,31;,
  4;35,31,32,36;,
  4;31,12,16,32;,
  4;14,29,89,34;,
  4;89,29,39,90;,
  4;33,89,90,37;,
  4;34,89,33,15;,
  4;15,33,32,16;,
  4;32,33,37,36;,
  4;9,10,92,93;,
  4;27,9,93,94;,
  4;13,27,94,95;,
  4;10,14,96,92;,
  4;11,13,95,97;,
  4;12,11,97,98;,
  4;16,12,98,99;,
  4;34,15,100,101;,
  4;14,34,101,96;,
  4;15,16,99,100;,
  3;93,92,102;,
  3;94,93,102;,
  3;95,94,102;,
  3;92,96,102;,
  3;97,95,102;,
  3;98,97,102;,
  3;99,98,102;,
  3;101,100,102;,
  3;96,101,102;,
  3;100,99,102;;
 }
 MeshTextureCoords {
  179;
  0.500000;0.194940;,
  0.500000;0.159600;,
  0.625000;0.159600;,
  0.625000;0.195410;,
  0.500000;0.120150;,
  0.625000;0.120150;,
  0.500000;0.629850;,
  0.500000;0.590400;,
  0.625000;0.590400;,
  0.625000;0.629850;,
  0.500000;0.552580;,
  0.625000;0.552180;,
  0.500000;0.759600;,
  0.625000;0.750000;,
  0.625000;0.875000;,
  0.500000;0.875000;,
  0.750000;0.196560;,
  0.750000;0.159600;,
  0.875000;0.159600;,
  0.875000;0.197820;,
  0.750000;0.120150;,
  0.875000;0.120150;,
  0.125000;0.120150;,
  0.250000;0.120150;,
  0.250000;0.159600;,
  0.125000;0.159600;,
  0.125000;0.197020;,
  0.250000;0.195800;,
  0.500000;0.074720;,
  0.500000;0.024100;,
  0.625000;0.024340;,
  0.625000;0.074960;,
  0.500000;0.000000;,
  0.625000;0.000000;,
  0.750000;0.074990;,
  0.750000;0.024380;,
  0.875000;0.024380;,
  0.875000;0.074990;,
  0.750000;0.000000;,
  0.875000;0.000000;,
  0.500000;0.725900;,
  0.500000;0.675280;,
  0.625000;0.675010;,
  0.625000;0.725620;,
  0.125000;0.023830;,
  0.125000;0.000000;,
  0.250000;0.000000;,
  0.250000;0.023830;,
  0.125000;0.074440;,
  0.250000;0.074440;,
  0.750000;0.230950;,
  0.875000;0.232240;,
  0.500000;0.229200;,
  0.625000;0.229680;,
  0.125000;0.231450;,
  0.250000;0.230250;,
  0.500000;0.518100;,
  0.625000;0.517760;,
  0.375000;0.875000;,
  0.375000;0.750000;,
  0.375000;0.726170;,
  0.375000;0.675560;,
  0.375000;0.629850;,
  0.375000;0.590400;,
  0.375000;0.552980;,
  0.375000;0.518550;,
  0.375000;0.194780;,
  0.375000;0.159600;,
  0.375000;0.229090;,
  0.375000;0.120150;,
  0.375000;0.074480;,
  0.375000;0.023860;,
  0.375000;0.000000;,
  0.687500;0.195920;,
  0.687500;0.159600;,
  0.687500;0.230260;,
  0.687500;0.120150;,
  0.687500;0.074990;,
  0.687500;0.024380;,
  0.687500;0.000000;,
  0.625000;1.000000;,
  0.500000;1.000000;,
  0.500000;0.937500;,
  0.625000;0.937500;,
  0.375000;0.937500;,
  0.375000;1.000000;,
  0.312500;0.023830;,
  0.312500;0.074440;,
  0.312500;0.000000;,
  0.312500;0.120150;,
  0.312500;0.159600;,
  0.312500;0.195190;,
  0.312500;0.229580;,
  0.407460;0.279840;,
  0.394190;0.266790;,
  0.500010;0.251290;,
  0.500000;0.268410;,
  0.378930;0.313170;,
  0.396520;0.314160;,
  0.382460;0.375000;,
  0.378730;0.375000;,
  0.605780;0.266780;,
  0.592470;0.279840;,
  0.131750;0.250000;,
  0.128370;0.249490;,
  0.247510;0.249540;,
  0.245030;0.250000;,
  0.379110;0.495070;,
  0.499980;0.494910;,
  0.383230;0.489140;,
  0.499970;0.476250;,
  0.605730;0.480410;,
  0.592410;0.466250;,
  0.620980;0.313160;,
  0.603340;0.314160;,
  0.621100;0.375000;,
  0.617200;0.375000;,
  0.621100;0.380570;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.500000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;;
 }
}
