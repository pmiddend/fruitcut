// ps_4_0 output by Cg compiler
// cgc version 3.1.0010, build date Feb 10 2012
// command line args: -fastprecision -fastmath -profile ps_5_0
// source file: /home/philipp/projects/fruitcut/media/shaders/postprocessing/desaturate.cg
//vendor NVIDIA Corporation
//version 3.1.0.10
//profile ps_5_0
//program pixel_main
//semantic pixel_main.factor
//semantic pixel_main.input_texture
//var float factor :  : _factor : 1 : 1
//var sampler2D input_texture :  : _input_texture : 2 : 1
//var float2 vertex_data.texture_coordinate : $vin.TEXCOORD0 :  : 0 : 1
//var float4 pixel_main : $vout.COLOR : COLOR : -1 : 1

#pragma pack_matrix(row_major)

struct vertex_outputs {
    float4 _position : SV_Position;
    float2 _texture_coordinate : TEXCOORD0;
};

struct X1X {
    float4 _POSITION : SV_Position;
    float2 _TEXCOORD0 : TEXCOORD0;
};

static float3 _TMP0;
Texture2D<float4> _TMP6;

 // main procedure, the original name was pixel_main
float4 main(uniform float _factor, uniform SamplerState _input_texture,  in X1X cin) : SV_Target
{

    vertex_outputs _vertex_data;
    float4 _texture_value;
    float _gray;

    _vertex_data._texture_coordinate = cin._TEXCOORD0;
    _texture_value = _TMP6.Sample(_input_texture, _vertex_data._texture_coordinate);
    _gray = dot(_texture_value.xyz, float3(   2.11999997496604919E-01f,   7.16000020503997803E-01f,   7.19999969005584717E-02f));
    _TMP0 = _gray.xxx + _factor*(_texture_value.xyz - _gray.xxx);
    return float4(_TMP0.x, _TMP0.y, _TMP0.z,   1.00000000000000000E00f);
} // main end
