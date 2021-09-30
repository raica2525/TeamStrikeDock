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
 207;
 2.30651;231.29684;-0.35136;,
 -5.00420;222.62195;-0.35136;,
 -9.05721;209.75493;-0.35136;,
 -1.51891;209.93448;2.21384;,
 -1.51891;209.93448;-2.91666;,
 -1.14134;44.93954;2.21384;,
 -8.67963;44.76031;-0.35136;,
 -1.14134;44.93954;-2.91666;,
 8.05263;210.16187;-1.55185;,
 7.73301;235.11404;-0.35136;,
 8.05263;210.16187;0.84935;,
 10.16611;45.20846;-2.91666;,
 10.16611;45.20846;2.21384;,
 7.53251;44.95366;5.85549;,
 6.06813;47.25686;5.85549;,
 5.34426;36.42175;5.85549;,
 4.91617;45.66132;5.85549;,
 -1.68876;31.30250;5.85549;,
 3.14251;31.30229;5.85549;,
 6.13622;49.56540;5.85549;,
 -6.33836;49.56550;5.85549;,
 -5.81948;47.25697;5.85549;,
 3.88532;30.73352;5.85549;,
 1.49980;32.78686;5.85549;,
 -4.08745;30.73342;5.85549;,
 2.65836;44.69699;5.85549;,
 -4.23210;44.69699;5.85549;,
 2.27911;40.51638;5.85549;,
 -4.20700;40.51638;5.85549;,
 2.11647;38.72453;5.85549;,
 -2.61931;38.72453;5.85549;,
 -3.64639;37.52173;5.85549;,
 -5.54660;36.42185;5.85549;,
 -5.07510;43.49409;5.85549;,
 -6.00617;38.21329;5.85549;,
 -7.07844;42.39411;5.85549;,
 -7.73496;44.95376;5.85549;,
 10.62391;53.69935;-4.26756;,
 6.41631;49.98857;-6.89143;,
 -10.82605;53.69935;-4.26756;,
 5.11290;49.98846;-6.89143;,
 2.76651;49.98857;-6.89143;,
 -6.61866;49.98846;-6.89143;,
 4.06459;30.31056;-6.89143;,
 6.75383;21.31731;-4.26756;,
 -6.95597;21.31731;-4.26756;,
 -4.26672;30.31056;-6.89143;,
 -8.07781;45.16943;-6.89143;,
 -13.22746;45.76905;-4.26756;,
 10.62391;53.69935;3.60396;,
 6.41631;49.98857;6.22773;,
 7.87557;45.16943;6.22773;,
 13.02511;45.76916;3.60396;,
 -5.79113;36.25418;-6.89143;,
 -10.71884;35.98853;-4.26756;,
 14.69302;36.25408;-6.89133;,
 13.31681;45.16943;-6.89143;,
 17.05239;45.76916;-4.26756;,
 19.62041;35.98864;-4.26766;,
 -10.82605;53.69935;3.60396;,
 -6.61866;49.98846;6.22773;,
 -8.07781;45.16943;6.22773;,
 -13.22746;45.76905;3.60396;,
 -10.66696;36.25418;6.22773;,
 -12.95343;45.16953;6.22773;,
 -16.07902;45.76905;3.60396;,
 -15.59435;35.98864;3.60396;,
 6.75383;21.31731;3.60396;,
 4.06459;30.31056;6.22773;,
 -4.26672;30.31056;6.22773;,
 -6.95597;21.31731;3.60396;,
 17.05239;45.76916;3.60396;,
 13.31681;45.16943;6.22773;,
 14.69302;36.25408;6.22773;,
 19.62041;35.98864;3.60396;,
 -16.07902;45.76905;-4.26756;,
 -15.59435;35.98864;-4.26756;,
 7.87557;45.16943;-6.89133;,
 13.02511;45.76916;-4.26756;,
 5.58900;36.25408;-6.89143;,
 10.51680;35.98864;-4.26756;,
 10.51670;35.98864;3.60396;,
 5.58900;36.25408;6.22773;,
 -5.79113;36.25418;6.22773;,
 -10.71884;35.98853;3.60396;,
 -12.95343;45.16953;-6.89143;,
 -10.66696;36.25418;-6.89143;,
 -6.27163;38.12638;6.22773;,
 -7.39169;42.49493;6.22773;,
 -6.27163;38.12638;-6.89143;,
 -7.39169;42.49493;-6.89143;,
 6.34519;47.57607;-6.89143;,
 5.14135;45.90900;-6.89143;,
 -6.07646;47.57607;-6.89143;,
 2.78209;44.90136;-6.89143;,
 -4.41744;44.90136;-6.89143;,
 -5.29862;43.64449;-6.89143;,
 3.28831;30.90495;-6.89143;,
 2.38611;40.53302;-6.89143;,
 2.21625;38.66052;-6.89143;,
 -4.39139;40.53302;-6.89143;,
 -2.73258;38.66052;-6.89143;,
 -3.80568;37.40344;-6.89143;,
 1.57166;32.45583;-6.89143;,
 -1.76020;30.90505;-6.89143;,
 7.53251;44.95366;-6.51899;,
 6.06813;47.25686;-6.51909;,
 6.13622;49.56540;-6.51909;,
 4.88876;49.56550;-6.51909;,
 -5.81948;47.25697;-6.51909;,
 2.64330;49.56540;-6.51909;,
 3.88532;30.73352;-6.51909;,
 -4.08745;30.73342;-6.51909;,
 -1.68876;31.30250;-6.51909;,
 2.65836;44.69699;-6.51909;,
 2.27911;40.51638;-6.51909;,
 -4.23210;44.69699;-6.51909;,
 -6.33836;49.56550;-6.51899;,
 -4.20700;40.51638;-6.51909;,
 2.11647;38.72453;-6.51899;,
 1.49980;32.78686;-6.51909;,
 -2.61931;38.72453;-6.51909;,
 -3.64639;37.52173;-6.51909;,
 -5.54660;36.42185;-6.51909;,
 -5.07510;43.49409;-6.51909;,
 -6.00617;38.21329;-6.51899;,
 -7.07844;42.39411;-6.51909;,
 -7.73496;44.95376;-6.51909;,
 5.34426;36.42175;-6.51909;,
 4.91627;45.66132;-6.51909;,
 3.14251;31.30229;-6.51909;,
 6.34519;47.57607;6.22773;,
 -6.07646;47.57607;6.22773;,
 5.14135;45.90900;6.22773;,
 2.78209;44.90136;6.22773;,
 3.28831;30.90495;6.22773;,
 1.57166;32.45583;6.22773;,
 -1.76020;30.90505;6.22773;,
 2.21625;38.66052;6.22773;,
 2.38611;40.53302;6.22773;,
 -4.39139;40.53302;6.22773;,
 -2.73258;38.66052;6.22773;,
 -3.80558;37.40344;6.22773;,
 -4.41744;44.90136;6.22773;,
 -5.29862;43.64449;6.22773;,
 -10.73631;33.37206;3.20809;,
 -10.73620;60.88581;3.20809;,
 -19.20379;53.90372;1.74214;,
 -19.20379;23.31781;1.74214;,
 -10.73620;60.88581;-3.91080;,
 -19.20379;53.90372;-2.44475;,
 -10.73631;33.37206;-3.91080;,
 -19.20379;23.31781;-2.44475;,
 -34.19431;18.46951;0.68545;,
 -31.13909;40.72912;0.68545;,
 -31.13909;40.72912;-1.38827;,
 -34.19431;18.46951;-1.38827;,
 -19.20379;24.69695;1.21845;,
 -19.20379;24.69695;-1.92106;,
 -19.20389;36.16403;-1.92106;,
 -19.20379;47.63077;-1.92106;,
 -19.20379;47.63077;1.21845;,
 -19.20389;36.16403;1.21845;,
 -28.60422;20.00950;1.21845;,
 -31.58224;18.29504;1.21845;,
 -31.58224;18.29504;-1.92106;,
 -28.60422;20.00950;-1.92106;,
 -26.74104;36.16414;-1.92106;,
 -26.74104;36.16414;1.21845;,
 -28.16546;20.41813;0.65429;,
 -19.24029;24.86827;0.65429;,
 -19.24029;35.75539;0.65429;,
 -26.39620;35.75539;0.65429;,
 -19.24039;24.86827;-1.35689;,
 -28.16546;20.41813;-1.35689;,
 -26.39620;35.75539;-1.35689;,
 -19.24029;35.75539;-1.35689;,
 4.38066;21.71401;2.54833;,
 1.77090;21.71391;2.54833;,
 4.38066;-23.48000;2.54833;,
 1.77090;-22.11142;2.54833;,
 0.03093;-26.21014;2.54833;,
 0.03103;-23.48000;2.54833;,
 -4.31881;-23.48000;2.54833;,
 -1.70895;-22.11153;2.54833;,
 -1.70895;21.71401;2.54833;,
 -4.31870;21.71391;2.54833;,
 4.38066;21.71401;-3.25114;,
 1.77090;21.71391;-3.25114;,
 4.38066;-23.48000;-3.25114;,
 1.77090;-22.11142;-3.25114;,
 0.03103;-26.21014;-3.25114;,
 0.03103;-23.48000;-3.25114;,
 -1.70895;-22.11153;-3.25114;,
 -4.31881;-23.48000;-3.25114;,
 -1.70895;21.71401;-3.25114;,
 -4.31870;21.71391;-3.25114;,
 -1.70895;21.71401;-2.11905;,
 -1.70895;21.71401;1.41623;,
 1.77090;21.71391;1.41623;,
 1.77090;21.71391;-2.11905;,
 1.77090;-22.11142;1.41623;,
 0.03103;-23.48000;1.41623;,
 -1.70895;-22.11153;1.41623;,
 1.77090;-22.11142;-2.11905;,
 0.03103;-23.48000;-2.11905;,
 -1.70895;-22.11153;-2.11905;;
 
 260;
 4;0,1,2,3;,
 4;4,2,1,0;,
 3;5,6,7;,
 3;8,9,10;,
 4;3,2,6,5;,
 4;7,6,2,4;,
 4;11,8,10,12;,
 4;11,12,5,7;,
 4;8,11,7,4;,
 4;9,8,4,0;,
 4;10,9,0,3;,
 4;12,10,3,5;,
 3;13,14,15;,
 3;14,16,15;,
 3;15,16,17;,
 3;17,16,18;,
 3;13,19,14;,
 3;19,20,14;,
 3;14,20,21;,
 3;22,23,24;,
 3;25,26,27;,
 3;27,26,28;,
 3;29,30,23;,
 3;23,30,24;,
 3;30,31,24;,
 3;31,28,24;,
 3;24,28,32;,
 3;28,26,32;,
 3;26,33,32;,
 3;33,21,32;,
 3;32,21,34;,
 3;21,20,34;,
 3;34,20,35;,
 3;35,20,36;,
 3;37,38,39;,
 3;38,40,39;,
 3;40,41,39;,
 3;39,41,42;,
 4;43,44,45,46;,
 4;39,42,47,48;,
 4;49,50,51,52;,
 4;53,46,45,54;,
 4;55,56,57,58;,
 4;59,60,50,49;,
 4;61,60,59,62;,
 4;63,64,65,66;,
 4;67,68,69,70;,
 4;71,72,73,74;,
 4;57,71,74,58;,
 4;44,67,70,45;,
 4;65,75,76,66;,
 4;59,39,48,62;,
 4;37,39,59,49;,
 4;77,38,37,78;,
 4;37,49,52,78;,
 4;43,79,80,44;,
 4;81,67,44,80;,
 4;82,68,67,81;,
 4;79,55,58,80;,
 4;78,57,56,77;,
 4;77,56,55,79;,
 4;81,74,73,82;,
 4;82,73,72,51;,
 4;51,72,71,52;,
 4;80,58,74,81;,
 4;52,71,57,78;,
 4;69,83,84,70;,
 4;54,45,70,84;,
 4;75,85,86,76;,
 4;83,63,66,84;,
 4;62,65,64,61;,
 3;83,87,63;,
 3;87,88,63;,
 3;88,61,63;,
 3;63,61,64;,
 3;53,54,86;,
 3;86,54,76;,
 3;54,84,76;,
 3;76,84,66;,
 4;48,75,65,62;,
 3;53,86,89;,
 3;89,86,90;,
 3;90,86,47;,
 3;47,86,85;,
 4;47,85,75,48;,
 3;91,92,93;,
 3;92,94,93;,
 3;94,95,93;,
 3;93,95,96;,
 3;92,97,94;,
 3;97,98,94;,
 3;97,99,98;,
 3;98,99,100;,
 3;99,101,100;,
 3;100,101,102;,
 3;97,103,99;,
 3;103,97,104;,
 3;105,106,107;,
 3;107,106,108;,
 3;106,109,108;,
 3;108,109,110;,
 3;111,112,113;,
 3;114,115,116;,
 3;110,109,117;,
 3;115,118,116;,
 3;119,120,121;,
 3;120,113,121;,
 3;113,112,121;,
 3;121,112,122;,
 3;122,112,118;,
 3;112,123,118;,
 3;118,123,116;,
 3;116,123,124;,
 3;124,123,109;,
 3;123,125,109;,
 3;109,125,117;,
 3;125,126,117;,
 3;117,126,127;,
 3;105,128,106;,
 3;106,128,129;,
 3;129,128,130;,
 3;131,132,133;,
 3;133,134,135;,
 3;135,136,137;,
 3;136,135,138;,
 3;135,139,138;,
 3;139,140,138;,
 3;138,140,141;,
 3;141,140,142;,
 3;139,135,134;,
 3;133,132,134;,
 3;134,132,143;,
 3;143,132,144;,
 4;68,22,24,69;,
 4;69,24,32,83;,
 4;83,32,34,87;,
 4;87,34,35,88;,
 4;88,35,36,61;,
 4;61,36,20,60;,
 4;60,20,19,50;,
 4;50,19,13,51;,
 4;51,13,15,82;,
 4;82,15,22,68;,
 4;137,17,18,135;,
 4;135,18,16,133;,
 4;133,16,14,131;,
 4;131,14,21,132;,
 4;132,21,33,144;,
 4;144,33,26,143;,
 4;143,26,25,134;,
 4;134,25,27,139;,
 4;139,27,28,140;,
 4;140,28,31,142;,
 4;142,31,30,141;,
 4;141,30,29,138;,
 4;138,29,23,136;,
 4;136,23,17,137;,
 4;79,128,105,77;,
 4;77,105,107,38;,
 4;38,107,108,40;,
 4;40,108,110,41;,
 4;41,110,117,42;,
 4;42,117,127,47;,
 4;47,127,126,90;,
 4;90,126,125,89;,
 4;89,125,123,53;,
 4;53,123,112,46;,
 4;46,112,111,43;,
 4;43,111,128,79;,
 4;104,113,120,103;,
 4;103,120,119,99;,
 4;99,119,121,101;,
 4;101,121,122,102;,
 4;102,122,118,100;,
 4;100,118,115,98;,
 4;98,115,114,94;,
 4;94,114,116,95;,
 4;95,116,124,96;,
 4;96,124,109,93;,
 4;93,109,106,91;,
 4;91,106,129,92;,
 4;92,129,130,97;,
 4;97,130,113,104;,
 4;145,146,147,148;,
 4;146,149,150,147;,
 4;149,151,152,150;,
 4;151,145,148,152;,
 4;153,154,155,156;,
 4;151,149,146,145;,
 4;148,157,158,152;,
 3;152,158,150;,
 3;150,159,160;,
 3;159,150,158;,
 4;150,160,161,147;,
 3;157,148,162;,
 3;162,148,161;,
 3;161,148,147;,
 4;163,164,165,166;,
 3;160,159,155;,
 3;159,167,155;,
 3;167,166,155;,
 3;166,165,155;,
 3;155,165,156;,
 4;160,155,154,161;,
 3;161,154,162;,
 3;154,168,162;,
 3;154,153,168;,
 3;168,153,163;,
 3;163,153,164;,
 4;164,153,156,165;,
 4;169,170,171,172;,
 4;173,174,175,176;,
 4;163,169,172,168;,
 4;168,172,171,162;,
 4;162,171,170,157;,
 4;158,173,176,159;,
 4;159,176,175,167;,
 4;167,175,174,166;,
 4;169,174,173,170;,
 4;166,174,169,163;,
 4;157,170,173,158;,
 3;177,178,179;,
 3;179,180,181;,
 3;180,182,181;,
 3;181,182,183;,
 3;182,184,183;,
 3;185,186,184;,
 3;184,186,183;,
 3;180,179,178;,
 4;177,187,188,178;,
 3;189,190,187;,
 3;187,190,188;,
 3;189,191,190;,
 3;190,191,192;,
 3;192,191,193;,
 3;191,194,193;,
 3;193,194,195;,
 3;195,194,196;,
 4;181,183,194,191;,
 4;183,186,196,194;,
 4;189,187,177,179;,
 4;189,179,181,191;,
 4;195,196,186,185;,
 4;197,198,199,200;,
 3;199,198,201;,
 3;201,198,202;,
 3;202,198,203;,
 3;204,205,200;,
 3;200,205,197;,
 3;197,205,206;,
 4;188,200,199,178;,
 4;185,198,197,195;,
 4;178,199,201,180;,
 4;180,201,202,182;,
 4;182,202,203,184;,
 4;184,203,198,185;,
 4;190,204,200,188;,
 4;195,197,206,193;,
 4;193,206,205,192;,
 4;192,205,204,190;;
 
 MeshMaterialList {
  10;
  260;
  4,
  4,
  0,
  4,
  4,
  4,
  4,
  0,
  4,
  4,
  4,
  4,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  3,
  3,
  3,
  3,
  3,
  0,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  0,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  0,
  0,
  3,
  3,
  3,
  3,
  3,
  3,
  0,
  0,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3;;
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
  249;
  0.999918;0.012812;0.000000;,
  0.023768;-0.999718;-0.000000;,
  0.999918;0.012809;0.000000;,
  -0.250134;0.125395;0.960057;,
  0.999918;0.012810;0.000000;,
  -0.286897;0.062495;0.955921;,
  0.023772;-0.999717;-0.000000;,
  -0.114214;0.051019;-0.992145;,
  -0.099958;0.099345;0.990020;,
  -0.114217;0.051019;0.992145;,
  0.174646;0.099188;0.979623;,
  -0.174648;0.099188;0.979622;,
  0.230506;0.373849;0.898390;,
  -0.230511;0.373850;0.898388;,
  0.152733;0.449170;-0.880295;,
  -0.230521;0.373856;-0.898383;,
  0.174655;0.099226;-0.979617;,
  -0.174660;0.099227;-0.979616;,
  -0.296380;0.039066;0.954271;,
  0.061440;-0.018606;0.997937;,
  0.230520;0.030383;-0.972593;,
  -0.084813;-0.025689;-0.996066;,
  0.178316;0.696601;-0.694947;,
  -0.090448;0.504554;-0.858630;,
  -0.437683;0.132531;0.889308;,
  -0.437697;0.132536;-0.889301;,
  -0.238294;-0.205672;-0.949165;,
  -0.238286;-0.205664;0.949169;,
  0.238293;-0.205672;-0.949166;,
  0.238287;-0.205665;0.949169;,
  0.437692;0.132523;-0.889305;,
  0.437679;0.132528;0.889311;,
  0.238163;-0.061087;-0.969302;,
  0.461707;-0.118420;-0.879092;,
  0.968647;-0.248440;0.000006;,
  0.238159;-0.061083;0.969303;,
  0.269488;0.054886;-0.961438;,
  0.849706;0.205646;-0.485499;,
  0.849706;0.205646;0.485500;,
  0.269489;0.054888;0.961438;,
  -0.238158;-0.061083;0.969304;,
  -0.461695;-0.118415;0.879099;,
  -0.461709;-0.118419;-0.879091;,
  -0.238166;-0.061085;-0.969302;,
  -0.114796;-0.015587;0.993267;,
  -0.880998;-0.070881;0.467780;,
  -0.291857;-0.039629;0.955641;,
  -0.881002;-0.070882;-0.467772;,
  -0.114800;-0.015588;-0.993266;,
  -0.291865;-0.039630;-0.955638;,
  0.433701;0.111233;0.894165;,
  0.433628;0.111231;0.894200;,
  0.000000;0.000000;1.000000;,
  0.433772;0.111249;-0.894128;,
  0.433713;0.111256;-0.894156;,
  -0.000011;0.577302;-0.816531;,
  0.000000;0.000000;-1.000000;,
  0.000010;0.577369;-0.816483;,
  0.372553;-0.261141;0.890511;,
  0.372602;-0.261175;-0.890481;,
  -0.173043;0.371750;0.912063;,
  -0.173068;0.371804;-0.912036;,
  0.194411;-0.393106;0.898706;,
  -0.114088;0.245096;0.962763;,
  -0.114104;0.245131;-0.962752;,
  0.244874;-0.171645;-0.954240;,
  -0.295981;0.328122;0.897068;,
  0.120144;0.133179;0.983783;,
  0.242259;0.062150;0.968219;,
  0.345176;0.088542;0.934352;,
  0.525752;-0.000343;0.850638;,
  0.100555;-0.131068;0.986260;,
  -0.181343;-0.236375;0.954590;,
  -0.396525;-0.000278;0.918024;,
  -0.345104;0.088512;0.934382;,
  -0.173043;0.371750;0.912063;,
  -0.000012;-0.367543;-0.930007;,
  0.244844;-0.171623;0.954252;,
  0.125579;-0.253925;0.959037;,
  0.000000;0.000000;1.000000;,
  -0.084415;0.181349;0.979789;,
  -0.396553;-0.000284;-0.918012;,
  -0.345137;0.088525;-0.934368;,
  -0.181356;-0.236426;-0.954575;,
  0.000000;-0.281660;-0.959514;,
  -0.000004;-0.353081;-0.935593;,
  0.118296;-0.154205;-0.980932;,
  0.525866;-0.000346;-0.850567;,
  0.433806;0.111251;-0.894112;,
  0.345170;0.088562;-0.934352;,
  0.242261;0.062162;-0.968218;,
  0.120160;0.133202;-0.983778;,
  -0.296024;0.328184;-0.897031;,
  -0.114096;0.245136;-0.962752;,
  -0.084427;0.181374;-0.979783;,
  0.000022;0.000015;-1.000000;,
  0.000006;0.000004;-1.000000;,
  0.000000;0.000000;-1.000000;,
  0.142494;-0.099881;-0.984743;,
  -0.000082;0.000005;-1.000000;,
  0.000027;-0.000001;-1.000000;,
  -0.170589;0.000000;0.985342;,
  -0.636184;0.771538;0.000000;,
  -0.986434;0.164159;-0.000085;,
  -0.170600;0.000000;-0.985340;,
  -0.999036;-0.041915;-0.013060;,
  -0.998493;-0.052430;0.016193;,
  -1.000000;0.000005;0.000055;,
  -1.000000;0.000003;0.000142;,
  -0.990712;0.135979;0.000000;,
  -0.830127;0.557574;0.000000;,
  0.225869;-0.974158;-0.000000;,
  0.594073;-0.804411;-0.000000;,
  -0.999766;-0.000002;0.021636;,
  -0.999765;0.000000;-0.021664;,
  -0.055945;-0.183689;0.981391;,
  0.214328;-0.234154;-0.948280;,
  0.461760;-0.053262;0.885404;,
  0.330309;-0.365238;0.870343;,
  -0.000003;-0.454609;0.890691;,
  -0.000003;-0.454610;-0.890690;,
  0.330309;-0.365239;-0.870343;,
  0.461760;-0.053262;-0.885404;,
  -0.000039;1.000000;0.000000;,
  0.000030;1.000000;0.000000;,
  -0.286907;0.062498;-0.955917;,
  -0.250143;0.125400;-0.960054;,
  -0.099953;0.099346;-0.990020;,
  -0.322132;-0.000737;0.946694;,
  -0.128296;0.002179;0.991734;,
  -0.128294;0.002180;-0.991734;,
  -0.322144;-0.000737;-0.946690;,
  0.023776;-0.999717;-0.000000;,
  0.061680;0.037974;-0.997373;,
  0.070639;0.005011;-0.997489;,
  0.052649;0.070893;-0.996093;,
  0.061662;0.037976;0.997374;,
  0.052630;0.070898;0.996094;,
  0.070623;0.005010;0.997491;,
  0.000000;-1.000000;-0.000000;,
  -0.957082;0.289818;0.000000;,
  0.000000;1.000000;0.000000;,
  0.957088;0.289799;0.000000;,
  0.461696;-0.118416;0.879098;,
  0.000000;-0.994921;-0.100656;,
  0.000000;-0.998730;-0.050392;,
  -0.000001;0.993695;-0.112117;,
  -0.000002;0.974859;-0.222821;,
  0.000000;-0.998730;0.050392;,
  0.000000;-0.994921;0.100656;,
  0.000000;0.974858;0.222828;,
  0.000000;0.993695;0.112121;,
  -0.968648;-0.248439;0.000000;,
  -0.000011;-0.994913;0.100735;,
  -0.000016;-0.998728;0.050432;,
  -0.000018;-0.999435;0.033616;,
  0.000005;0.993698;0.112086;,
  0.000010;0.974873;0.222760;,
  0.000000;-0.994914;-0.100731;,
  -0.000015;-0.997738;-0.067230;,
  -0.000011;-0.994914;-0.100731;,
  -0.000022;-0.999435;-0.033615;,
  0.000005;0.993699;-0.112082;,
  0.000010;0.974875;-0.222752;,
  0.775481;0.198907;0.599221;,
  0.775463;0.198940;0.599232;,
  0.775647;0.198964;0.598987;,
  0.360608;-0.470033;0.805624;,
  -0.360591;-0.470018;0.805641;,
  -0.775378;0.198869;0.599367;,
  -0.775369;0.198869;0.599378;,
  -0.000022;-0.683635;-0.729824;,
  0.880819;-0.108792;-0.460784;,
  0.884402;-0.362275;-0.294262;,
  0.806623;-0.582430;-0.100669;,
  0.000003;0.759159;-0.650905;,
  -0.873637;-0.172834;-0.454849;,
  0.000000;-0.876573;0.481268;,
  -0.958572;0.086925;0.271264;,
  0.000000;0.999003;-0.044647;,
  -0.886563;-0.165960;-0.431815;,
  0.755175;-0.644721;0.118514;,
  0.420150;-0.907011;-0.028352;,
  0.000000;-0.985535;-0.169473;,
  -0.956779;0.099383;0.273306;,
  -0.775455;0.198893;-0.599258;,
  -0.775464;0.198896;-0.599246;,
  -0.360649;-0.470113;-0.805560;,
  0.000000;-0.660672;-0.750675;,
  -0.000002;-0.660695;-0.750654;,
  0.360706;-0.470161;-0.805506;,
  0.775560;0.198974;-0.599096;,
  0.775559;0.198927;-0.599113;,
  -0.775447;0.198889;-0.599270;,
  -0.956806;0.099388;-0.273209;,
  -0.000002;-0.985547;0.169404;,
  0.420136;-0.907020;0.028288;,
  0.755156;-0.644735;-0.118559;,
  -0.886566;-0.165946;0.431812;,
  0.000000;0.999003;0.044634;,
  -0.958587;0.086927;-0.271208;,
  0.000000;-0.876630;-0.481165;,
  -0.873687;-0.172844;0.454747;,
  0.000003;0.759249;0.650800;,
  0.806646;-0.582420;0.100547;,
  0.884459;-0.362274;0.294090;,
  0.880923;-0.108808;0.460583;,
  -0.000022;-0.683739;0.729726;,
  -0.959740;0.280889;0.000000;,
  0.939383;-0.342870;-0.000000;,
  0.764883;-0.644169;-0.000000;,
  1.000000;-0.000004;-0.000000;,
  -1.000000;0.000000;0.000100;,
  -1.000000;0.000000;0.000000;,
  -0.044595;0.000000;-0.999005;,
  -0.016698;-0.259791;-0.965520;,
  -0.073060;0.036032;-0.996676;,
  0.262244;-0.021806;-0.964755;,
  -0.106389;0.025708;-0.993992;,
  -0.198039;0.027182;-0.979817;,
  -0.500587;0.865686;0.000000;,
  -0.044613;0.000000;0.999004;,
  -0.050142;0.043582;0.997791;,
  -0.016704;-0.259775;0.965525;,
  -0.126310;0.109082;0.985975;,
  -0.136393;0.156539;0.978209;,
  -0.171674;0.298199;0.938938;,
  -0.066644;-0.997777;0.000000;,
  0.817689;-0.094317;0.567881;,
  -0.997919;-0.000005;0.064478;,
  -0.993941;-0.105013;0.032448;,
  -0.993941;-0.105011;-0.032465;,
  -0.997914;0.000000;-0.064563;,
  0.569747;-0.821820;-0.000006;,
  0.446217;-0.894925;-0.000012;,
  -0.531612;-0.846988;-0.000005;,
  -0.875104;-0.483935;-0.000003;,
  -1.000000;0.000002;0.000000;,
  0.875106;-0.483931;0.000003;,
  1.000000;0.000000;0.000000;,
  0.531621;-0.846982;0.000005;,
  -0.000039;1.000000;0.000000;,
  0.000000;0.000000;0.000000;,
  -0.899514;0.436893;0.000000;,
  -0.618250;0.785982;0.000000;,
  0.618197;0.786023;0.000000;,
  0.899499;0.436923;0.000000;,
  -0.899514;0.436893;0.000000;,
  -0.618250;0.785982;0.000000;;
  260;
  4;8,3,5,9;,
  4;7,125,126,127;,
  3;6,1,6;,
  3;4,2,4;,
  4;9,5,128,129;,
  4;130,131,125,7;,
  4;0,4,4,0;,
  4;132,132,6,6;,
  4;133,134,130,7;,
  4;135,133,7,127;,
  4;136,137,8,9;,
  4;138,136,9,129;,
  3;73,52,74;,
  3;52,52,74;,
  3;74,52,75;,
  3;76,97,76;,
  3;73,72,52;,
  3;72,71,52;,
  3;52,71,52;,
  3;66,80,67;,
  3;79,78,52;,
  3;52,78,52;,
  3;52,52,80;,
  3;80,52,67;,
  3;52,52,67;,
  3;52,52,67;,
  3;67,52,68;,
  3;52,78,68;,
  3;78,77,68;,
  3;77,52,68;,
  3;68,52,69;,
  3;52,71,69;,
  3;69,71,50;,
  3;50,71,70;,
  3;22,14,23;,
  3;14,55,23;,
  3;55,57,23;,
  3;23,57,15;,
  4;16,28,26,17;,
  4;23,15,20,25;,
  4;12,12,18,31;,
  4;43,17,26,42;,
  4;36,36,37,37;,
  4;13,13,12,12;,
  4;19,13,13,24;,
  4;44,46,45,45;,
  4;29,10,11,27;,
  4;38,39,39,38;,
  4;37,38,38,37;,
  4;139,139,139,139;,
  4;45,47,47,45;,
  4;140,140,140,140;,
  4;22,141,141,141;,
  4;21,14,22,30;,
  4;142,142,142,142;,
  4;16,32,33,28;,
  4;34,34,34,34;,
  4;35,10,29,143;,
  4;144,144,145,145;,
  4;146,146,147,147;,
  4;21,36,36,32;,
  4;148,148,149,149;,
  4;35,39,39,18;,
  4;150,150,151,151;,
  4;145,145,148,148;,
  4;151,151,146,146;,
  4;11,40,41,27;,
  4;152,152,152,152;,
  4;47,49,48,47;,
  4;153,153,154,155;,
  4;156,156,157,157;,
  3;40,51,44;,
  3;51,50,44;,
  3;50,19,44;,
  3;44,19,46;,
  3;158,159,160;,
  3;160,159,161;,
  3;159,155,161;,
  3;161,155,154;,
  4;162,162,156,156;,
  3;43,48,54;,
  3;54,48,53;,
  3;53,48,20;,
  3;20,48,49;,
  4;163,163,162,162;,
  3;56,56,56;,
  3;56,56,56;,
  3;56,65,56;,
  3;56,65,59;,
  3;56,56,56;,
  3;56,56,56;,
  3;56,56,56;,
  3;56,56,56;,
  3;56,56,56;,
  3;56,56,56;,
  3;56,64,56;,
  3;64,56,61;,
  3;81,100,83;,
  3;83,100,84;,
  3;100,99,84;,
  3;84,99,85;,
  3;92,91,94;,
  3;97,56,98;,
  3;85,99,86;,
  3;56,56,98;,
  3;95,93,96;,
  3;93,94,96;,
  3;94,91,96;,
  3;96,91,56;,
  3;56,91,56;,
  3;91,90,56;,
  3;56,90,98;,
  3;98,90,65;,
  3;65,90,99;,
  3;90,89,99;,
  3;99,89,86;,
  3;89,88,86;,
  3;86,88,87;,
  3;81,82,100;,
  3;100,82,56;,
  3;56,82,56;,
  3;52,52,52;,
  3;52,52,52;,
  3;52,63,60;,
  3;63,52,52;,
  3;52,52,52;,
  3;52,52,52;,
  3;52,52,52;,
  3;52,52,52;,
  3;52,52,52;,
  3;52,52,52;,
  3;52,52,62;,
  3;62,52,58;,
  4;10,66,67,11;,
  4;164,67,68,165;,
  4;165,68,69,51;,
  4;51,69,50,50;,
  4;50,50,70,166;,
  4;19,70,71,167;,
  4;167,71,72,168;,
  4;168,72,73,18;,
  4;18,73,74,169;,
  4;169,74,66,170;,
  4;171,76,76,171;,
  4;172,172,173,173;,
  4;173,173,174,174;,
  4;175,175,175,175;,
  4;176,176,176,176;,
  4;58,77,78,62;,
  4;62,78,177,177;,
  4;178,178,178,178;,
  4;179,179,179,179;,
  4;180,180,180,180;,
  4;181,181,182,182;,
  4;182,182,183,183;,
  4;184,184,184,184;,
  4;63,80,75,60;,
  4;185,82,81,186;,
  4;21,81,83,187;,
  4;187,83,84,188;,
  4;188,84,85,189;,
  4;189,85,86,190;,
  4;190,86,87,20;,
  4;20,87,88,53;,
  4;53,88,89,54;,
  4;54,89,90,191;,
  4;191,90,91,192;,
  4;17,91,92,16;,
  4;193,92,82,185;,
  4;61,94,93,64;,
  4;194,194,194,194;,
  4;195,195,196,196;,
  4;196,196,197,197;,
  4;198,198,198,198;,
  4;199,199,199,199;,
  4;200,200,200,200;,
  4;201,201,201,201;,
  4;65,98,65,59;,
  4;202,202,202,202;,
  4;203,203,203,203;,
  4;204,204,205,205;,
  4;205,205,206,206;,
  4;207,207,207,207;,
  4;101,101,101,101;,
  4;102,102,103,208;,
  4;104,104,104,104;,
  4;209,209,210,210;,
  4;109,110,110,109;,
  4;209,211,211,209;,
  4;212,106,105,213;,
  3;213,105,103;,
  3;103,114,107;,
  3;114,103,105;,
  4;103,107,108,208;,
  3;106,212,113;,
  3;113,212,108;,
  3;108,212,208;,
  4;112,111,111,112;,
  3;214,215,216;,
  3;215,116,216;,
  3;116,217,216;,
  3;217,218,216;,
  3;216,218,219;,
  4;220,110,110,220;,
  3;221,222,223;,
  3;222,115,223;,
  3;222,224,115;,
  3;115,224,225;,
  3;225,224,226;,
  4;111,227,227,111;,
  4;117,52,119,118;,
  4;56,122,121,120;,
  4;228,117,118,228;,
  4;115,118,119,223;,
  4;113,229,230,106;,
  4;105,231,232,114;,
  4;215,120,121,116;,
  4;116,121,122,217;,
  4;233,233,234,234;,
  4;112,233,233,112;,
  4;106,230,231,105;,
  3;52,52,52;,
  3;52,52,52;,
  3;52,52,52;,
  3;52,52,52;,
  3;52,52,52;,
  3;79,52,52;,
  3;52,52,52;,
  3;52,52,52;,
  4;123,123,123,123;,
  3;56,56,56;,
  3;56,56,56;,
  3;56,56,56;,
  3;56,56,56;,
  3;56,56,56;,
  3;56,56,56;,
  3;56,56,56;,
  3;56,56,56;,
  4;235,236,236,235;,
  4;236,237,237,236;,
  4;238,239,239,238;,
  4;238,238,240,240;,
  4;241,241,241,241;,
  4;124,124,124,124;,
  3;52,52,52;,
  3;52,52,52;,
  3;52,52,52;,
  3;56,56,56;,
  3;56,56,56;,
  3;56,56,56;,
  4;242,242,242,242;,
  4;242,242,242,242;,
  4;213,213,243,243;,
  4;243,243,244,244;,
  4;245,245,246,246;,
  4;246,246,239,239;,
  4;247,247,213,213;,
  4;239,239,246,246;,
  4;246,246,245,245;,
  4;248,248,247,247;;
 }
}
