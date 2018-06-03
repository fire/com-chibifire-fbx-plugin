cc_binary(
    name = "FBX2glTF",
    srcs = [
        "thirdparty/FBX2glTF/src/main.cpp",
        "thirdparty/fbx20181_1_fbxsdk_vs2015_win/lib/vs2015/x64/release/libfbxsdk-md.lib",
    ],
    includes = [
        "thirdparty/cppcodec",
        "thirdparty/stb",
        "thirdparty/draco/src/draco/src",
        "thirdparty/cxxopts/include",
        "thirdparty/fifo_map/src",
        "thirdparty/fmt",
        "thirdparty/fbx20181_1_fbxsdk_vs2015_win/include",
        "thirdparty/mathfu/include",
        "thirdparty/mathfu/dependencies/vectorial/include",
        "thirdparty/json/src",        
        "thirdparty/FBX2glTF/src",
        "thirdparty/godot/modules/gdnative/include",
        "thirdparty/godot_headers/",
        "thirdparty/godot-cpp/include/",
        "thirdparty/godot-cpp/include/core",
    ],
    copts = ["-O2", "/MD"],
    deps = ["//:libFBX2glTF"]
)

cc_binary(
    features = ["use_linker", "windows_export_all_symbols"],
    name = "com_chibifire_fbx_importer.dll",
    linkstatic = 1,
    linkshared = 1,
    copts = ["-O2", "/MD"],
    linkopts = ["-NODEFAULTLIB:LIBCMT"],
    deps = ["//:libcom_chibifire_fbx_importer"],
)

cc_library(
    name = "libcom_chibifire_fbx_importer",
    srcs = [
        "sample/addons/com_chibifire_fbx_importer/src/Godot.cpp",
        "sample/addons/com_chibifire_fbx_importer/src/ComChibifireFbxImporter.cpp",
        "sample/addons/com_chibifire_fbx_importer/src/ComChibifireFbxImporter.h",
        "thirdparty/fbx20181_1_fbxsdk_vs2015_win/lib/vs2015/x64/release/libfbxsdk-md.lib",
        "thirdparty/godot-cpp/bin/godot-cpp.windows.64.lib",
        "thirdparty/godot-cpp/include/gdnative.hpp",
        "thirdparty/godot_headers/gdnative_api_struct.gen.h",
    ],
    includes = [
        "thirdparty/stb",
        "thirdparty/draco/src/draco/src",
        "thirdparty/fifo_map/src",
        "thirdparty/fbx20181_1_fbxsdk_vs2015_win/include",
        "thirdparty/mathfu/include",
        "thirdparty/mathfu/dependencies/vectorial/include",
        "thirdparty/json/src",        
        "thirdparty/FBX2glTF/src",
        "thirdparty/godot/modules/gdnative/include",
        "thirdparty/godot-cpp/include/",
        "thirdparty/godot-cpp/include/core",
    ],
    copts = ["-O2", "/MD"],
    linkopts = ["-NODEFAULTLIB:LIBCMT"],
    deps = ["//:libFBX2glTF"],
)


cc_library(
    features = ["use_linker"],
    name = "libFBX2glTF",
    srcs = [
    "thirdparty/FBX2glTF/src/utils/File_Utils.cpp",
    "thirdparty/FBX2glTF/src/utils/Image_Utils.cpp",
    "thirdparty/FBX2glTF/src/utils/String_Utils.cpp",
    "thirdparty/FBX2glTF/src/Fbx2Raw.cpp",
    "thirdparty/FBX2glTF/src/Raw2Gltf.cpp",
    "thirdparty/FBX2glTF/src/RawModel.cpp",
    "thirdparty/FBX2glTF/src/glTF/BufferData.cpp",
    "thirdparty/FBX2glTF/src/glTF/MaterialData.cpp",
    "thirdparty/FBX2glTF/src/glTF/MeshData.cpp",
    "thirdparty/FBX2glTF/src/glTF/NodeData.cpp",
    "thirdparty/FBX2glTF/src/glTF/PrimitiveData.cpp",
    "thirdparty/FBX2glTF/src/glTF/BufferViewData.cpp",
    "thirdparty/FBX2glTF/src/glTF/BufferViewData.h",
    "thirdparty/FBX2glTF/src/glTF/AccessorData.cpp",
    "thirdparty/FBX2glTF/src/glTF/AccessorData.h",
    "thirdparty/FBX2glTF/src/glTF/ImageData.cpp",
    "thirdparty/FBX2glTF/src/glTF/TextureData.cpp",
    "thirdparty/FBX2glTF/src/glTF/SkinData.cpp",
    "thirdparty/FBX2glTF/src/glTF/AnimationData.cpp",
    "thirdparty/FBX2glTF/src/glTF/CameraData.cpp",
    "thirdparty/FBX2glTF/src/glTF/SceneData.cpp",
    ],
    includes = [
        "thirdparty/cppcodec",
        "thirdparty/stb",
        "thirdparty/draco/src/draco/src",
        "thirdparty/cxxopts/include",
        "thirdparty/fifo_map/src",
        "thirdparty/fmt",
        "thirdparty/fbx20181_1_fbxsdk_vs2015_win/include",
        "thirdparty/mathfu/include",
        "thirdparty/mathfu/dependencies/vectorial/include",
        "thirdparty/json/src",        
        "thirdparty/FBX2glTF/src",
    ],
    copts = ["-O2", "/MD", "/wd4127", "/wd4577", "/DMATHFU_COMPILE_FORCE_PADDING=1"],
    deps = ["//:draco", "//:fmt"]
)


cc_library(
    name = "fmt",
    srcs = [
        "thirdparty/fmt/fmt/format.cc",
        "thirdparty/fmt/fmt/printf.cc",
    ],
    includes = [
        "thirdparty/fmt",
    ],  
    copts = ["-O2", "/MD"],
)


cc_library(
    name = "draco",
    srcs = [
        "thirdparty/draco/src/draco/attributes/attribute_octahedron_transform.cc",
        "thirdparty/draco/src/draco/attributes/attribute_octahedron_transform.h",
        "thirdparty/draco/src/draco/attributes/attribute_quantization_transform.cc",
        "thirdparty/draco/src/draco/attributes/attribute_quantization_transform.h",
        "thirdparty/draco/src/draco/attributes/attribute_transform.cc",
        "thirdparty/draco/src/draco/attributes/attribute_transform.h",
        "thirdparty/draco/src/draco/attributes/attribute_transform_data.h",
        "thirdparty/draco/src/draco/attributes/attribute_transform_type.h",
        "thirdparty/draco/src/draco/attributes/geometry_attribute.cc",
        "thirdparty/draco/src/draco/attributes/geometry_attribute.h",
        "thirdparty/draco/src/draco/attributes/geometry_indices.h",
        "thirdparty/draco/src/draco/attributes/point_attribute.cc",
        "thirdparty/draco/src/draco/attributes/point_attribute.h",
    #set(draco_compression_attributes_dec_sources
        "thirdparty/draco/src/draco/compression/attributes/attributes_decoder.cc",
        "thirdparty/draco/src/draco/compression/attributes/attributes_decoder.h",
        "thirdparty/draco/src/draco/compression/attributes/kd_tree_attributes_decoder.cc",
        "thirdparty/draco/src/draco/compression/attributes/kd_tree_attributes_decoder.h",
        "thirdparty/draco/src/draco/compression/attributes/kd_tree_attributes_shared.h",
        "thirdparty/draco/src/draco/compression/attributes/mesh_attribute_indices_encoding_data.h",
        "thirdparty/draco/src/draco/compression/attributes/mesh_traversal_sequencer.h",
        "thirdparty/draco/src/draco/compression/attributes/normal_compression_utils.h",
        "thirdparty/draco/src/draco/compression/attributes/sequential_attribute_decoder.cc",
        "thirdparty/draco/src/draco/compression/attributes/sequential_attribute_decoder.h",
        "thirdparty/draco/src/draco/compression/attributes/sequential_attribute_decoders_controller.cc",
        "thirdparty/draco/src/draco/compression/attributes/sequential_attribute_decoders_controller.h",
        "thirdparty/draco/src/draco/compression/attributes/sequential_integer_attribute_decoder.cc",
        "thirdparty/draco/src/draco/compression/attributes/sequential_integer_attribute_decoder.h",
        "thirdparty/draco/src/draco/compression/attributes/sequential_normal_attribute_decoder.cc",
        "thirdparty/draco/src/draco/compression/attributes/sequential_normal_attribute_decoder.h",
        "thirdparty/draco/src/draco/compression/attributes/sequential_quantization_attribute_decoder.cc",
        "thirdparty/draco/src/draco/compression/attributes/sequential_quantization_attribute_decoder.h",
    #set(draco_compression_attributes_enc_sources
        "thirdparty/draco/src/draco/compression/attributes/attributes_encoder.cc",
        "thirdparty/draco/src/draco/compression/attributes/attributes_encoder.h",
        "thirdparty/draco/src/draco/compression/attributes/kd_tree_attributes_encoder.cc",
        "thirdparty/draco/src/draco/compression/attributes/kd_tree_attributes_encoder.h",
        "thirdparty/draco/src/draco/compression/attributes/linear_sequencer.h",
        "thirdparty/draco/src/draco/compression/attributes/mesh_attribute_indices_encoding_observer.h",
        "thirdparty/draco/src/draco/compression/attributes/points_sequencer.h",
        "thirdparty/draco/src/draco/compression/attributes/sequential_attribute_encoder.cc",
        "thirdparty/draco/src/draco/compression/attributes/sequential_attribute_encoder.h",
        "thirdparty/draco/src/draco/compression/attributes/sequential_attribute_encoders_controller.cc",
        "thirdparty/draco/src/draco/compression/attributes/sequential_attribute_encoders_controller.h",
        "thirdparty/draco/src/draco/compression/attributes/sequential_integer_attribute_encoder.cc",
        "thirdparty/draco/src/draco/compression/attributes/sequential_integer_attribute_encoder.h",
        "thirdparty/draco/src/draco/compression/attributes/sequential_normal_attribute_encoder.cc",
        "thirdparty/draco/src/draco/compression/attributes/sequential_normal_attribute_encoder.h",
        "thirdparty/draco/src/draco/compression/attributes/sequential_quantization_attribute_encoder.cc",
        "thirdparty/draco/src/draco/compression/attributes/sequential_quantization_attribute_encoder.h",
    #set(draco_compression_attributes_pred_schemes_dec_sources
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/mesh_prediction_scheme_constrained_multi_parallelogram_decoder.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/mesh_prediction_scheme_constrained_multi_parallelogram_shared.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/mesh_prediction_scheme_data.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/mesh_prediction_scheme_decoder.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/mesh_prediction_scheme_geometric_normal_decoder.h",
#       "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/mesh_prediction_scheme_geometric_normal_predictor_area.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/mesh_prediction_scheme_geometric_normal_predictor_base.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/mesh_prediction_scheme_multi_parallelogram_decoder.h",
#       "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/mesh_prediction_scheme_parallelogram_encoder.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/mesh_prediction_scheme_parallelogram_shared.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/mesh_prediction_scheme_tex_coords_decoder.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/mesh_prediction_scheme_tex_coords_portable_decoder.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/mesh_prediction_scheme_tex_coords_portable_predictor.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/prediction_scheme_decoder.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/prediction_scheme_decoder_factory.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/prediction_scheme_decoder_interface.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/prediction_scheme_decoding_transform.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/prediction_scheme_delta_decoder.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/prediction_scheme_factory.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/prediction_scheme_interface.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/prediction_scheme_normal_octahedron_canonicalized_decoding_transform.h",
#       "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/prediction_scheme_normal_octahedron_canonicalized_transform_base.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/prediction_scheme_normal_octahedron_decoding_transform.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/prediction_scheme_normal_octahedron_transform_base.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/prediction_scheme_wrap_decoding_transform.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/prediction_scheme_wrap_transform_base.h",
    #set(draco_compression_attributes_pred_schemes_enc_sources
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/mesh_prediction_scheme_constrained_multi_parallelogram_encoder.h",
#       "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/mesh_prediction_scheme_constrained_multi_parallelogram_shared.h",
#       "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/mesh_prediction_scheme_data.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/mesh_prediction_scheme_encoder.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/mesh_prediction_scheme_geometric_normal_encoder.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/mesh_prediction_scheme_geometric_normal_predictor_area.h",
#       "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/mesh_prediction_scheme_geometric_normal_predictor_base.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/mesh_prediction_scheme_multi_parallelogram_encoder.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/mesh_prediction_scheme_parallelogram_encoder.h",
#       "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/mesh_prediction_scheme_parallelogram_shared.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/mesh_prediction_scheme_tex_coords_encoder.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/mesh_prediction_scheme_tex_coords_portable_encoder.h",
#       "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/mesh_prediction_scheme_tex_coords_portable_predictor.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/prediction_scheme_delta_encoder.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/prediction_scheme_encoder.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/prediction_scheme_encoder_factory.cc",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/prediction_scheme_encoder_factory.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/prediction_scheme_encoder_interface.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/prediction_scheme_encoding_transform.h",
#       "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/prediction_scheme_factory.h",
#       "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/prediction_scheme_interface.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/prediction_scheme_normal_octahedron_canonicalized_encoding_transform.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/prediction_scheme_normal_octahedron_canonicalized_transform_base.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/prediction_scheme_normal_octahedron_encoding_transform.h",
#       "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/prediction_scheme_normal_octahedron_transform_base.h",
        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/prediction_scheme_wrap_encoding_transform.h",
#       "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/prediction_scheme_wrap_transform_base.h",
    #set(draco_enc_config_sources
        "thirdparty/draco/src/draco/compression/config/compression_shared.h",
        "thirdparty/draco/src/draco/compression/config/draco_options.h",
        "thirdparty/draco/src/draco/compression/config/encoder_options.h",
        "thirdparty/draco/src/draco/compression/config/encoding_features.h",
    #set(draco_dec_config_sources
    #    "thirdparty/draco/src/draco/compression/config/compression_shared.h",
        "thirdparty/draco/src/draco/compression/config/decoder_options.h",
    #   "thirdparty/draco/src/draco/compression/config/draco_options.h",
    #set(draco_compression_decode_sources
        "thirdparty/draco/src/draco/compression/decode.cc",
        "thirdparty/draco/src/draco/compression/decode.h",
    #set(draco_compression_encode_sources
        "thirdparty/draco/src/draco/compression/encode.cc",
        "thirdparty/draco/src/draco/compression/encode.h",
        "thirdparty/draco/src/draco/compression/encode_base.h",
        "thirdparty/draco/src/draco/compression/expert_encode.cc",
        "thirdparty/draco/src/draco/compression/expert_encode.h",
    #set(draco_compression_mesh_dec_sources
        "thirdparty/draco/src/draco/compression/mesh/mesh_decoder.cc",
        "thirdparty/draco/src/draco/compression/mesh/mesh_decoder.h",
        "thirdparty/draco/src/draco/compression/mesh/mesh_decoder_helpers.h",
        "thirdparty/draco/src/draco/compression/mesh/mesh_edgebreaker_decoder.cc",
        "thirdparty/draco/src/draco/compression/mesh/mesh_edgebreaker_decoder.h",
        "thirdparty/draco/src/draco/compression/mesh/mesh_edgebreaker_decoder_impl.cc",
        "thirdparty/draco/src/draco/compression/mesh/mesh_edgebreaker_decoder_impl.h",
        "thirdparty/draco/src/draco/compression/mesh/mesh_edgebreaker_decoder_impl_interface.h",
        "thirdparty/draco/src/draco/compression/mesh/mesh_edgebreaker_shared.h",
        "thirdparty/draco/src/draco/compression/mesh/mesh_edgebreaker_traversal_decoder.h",
        "thirdparty/draco/src/draco/compression/mesh/mesh_edgebreaker_traversal_predictive_decoder.h",
        "thirdparty/draco/src/draco/compression/mesh/mesh_edgebreaker_traversal_valence_decoder.h",
        "thirdparty/draco/src/draco/compression/mesh/mesh_sequential_decoder.cc",
        "thirdparty/draco/src/draco/compression/mesh/mesh_sequential_decoder.h",
    #set(draco_compression_mesh_enc_sources
        "thirdparty/draco/src/draco/compression/mesh/mesh_edgebreaker_encoder.cc",
        "thirdparty/draco/src/draco/compression/mesh/mesh_edgebreaker_encoder.h",
        "thirdparty/draco/src/draco/compression/mesh/mesh_edgebreaker_encoder_impl.cc",
        "thirdparty/draco/src/draco/compression/mesh/mesh_edgebreaker_encoder_impl.h",
        "thirdparty/draco/src/draco/compression/mesh/mesh_edgebreaker_encoder_impl_interface.h",
#       "thirdparty/draco/src/draco/compression/mesh/mesh_edgebreaker_shared.h",
        "thirdparty/draco/src/draco/compression/mesh/mesh_edgebreaker_traversal_encoder.h",
        "thirdparty/draco/src/draco/compression/mesh/mesh_edgebreaker_traversal_predictive_encoder.h",
        "thirdparty/draco/src/draco/compression/mesh/mesh_edgebreaker_traversal_valence_encoder.h",
        "thirdparty/draco/src/draco/compression/mesh/mesh_encoder.cc",
        "thirdparty/draco/src/draco/compression/mesh/mesh_encoder.h",
        "thirdparty/draco/src/draco/compression/mesh/mesh_encoder_helpers.h",
        "thirdparty/draco/src/draco/compression/mesh/mesh_sequential_encoder.cc",
        "thirdparty/draco/src/draco/compression/mesh/mesh_sequential_encoder.h",
    #set(draco_compression_point_cloud_dec_sources
        "thirdparty/draco/src/draco/compression/point_cloud/point_cloud_decoder.cc",
        "thirdparty/draco/src/draco/compression/point_cloud/point_cloud_decoder.h",
        "thirdparty/draco/src/draco/compression/point_cloud/point_cloud_kd_tree_decoder.cc",
        "thirdparty/draco/src/draco/compression/point_cloud/point_cloud_kd_tree_decoder.h",
        "thirdparty/draco/src/draco/compression/point_cloud/point_cloud_sequential_decoder.cc",
        "thirdparty/draco/src/draco/compression/point_cloud/point_cloud_sequential_decoder.h",
    #set(draco_compression_point_cloud_enc_sources
        "thirdparty/draco/src/draco/compression/point_cloud/point_cloud_encoder.cc",
        "thirdparty/draco/src/draco/compression/point_cloud/point_cloud_encoder.h",
        "thirdparty/draco/src/draco/compression/point_cloud/point_cloud_kd_tree_encoder.cc",
        "thirdparty/draco/src/draco/compression/point_cloud/point_cloud_kd_tree_encoder.h",
        "thirdparty/draco/src/draco/compression/point_cloud/point_cloud_sequential_encoder.cc",
        "thirdparty/draco/src/draco/compression/point_cloud/point_cloud_sequential_encoder.h",
    #set(draco_core_sources
        "thirdparty/draco/src/draco/core/ans.h",
        "thirdparty/draco/src/draco/core/bit_utils.h",
        "thirdparty/draco/src/draco/core/cycle_timer.cc",
        "thirdparty/draco/src/draco/core/cycle_timer.h",
        "thirdparty/draco/src/draco/core/data_buffer.cc",
        "thirdparty/draco/src/draco/core/data_buffer.h",
        "thirdparty/draco/src/draco/core/decoder_buffer.cc",
        "thirdparty/draco/src/draco/core/decoder_buffer.h",
        "thirdparty/draco/src/draco/core/divide.cc",
        "thirdparty/draco/src/draco/core/divide.h",
        "thirdparty/draco/src/draco/core/draco_index_type.h",
        "thirdparty/draco/src/draco/core/draco_index_type_vector.h",
        "thirdparty/draco/src/draco/core/draco_types.cc",
        "thirdparty/draco/src/draco/core/draco_types.h",
        "thirdparty/draco/src/draco/core/encoder_buffer.cc",
        "thirdparty/draco/src/draco/core/encoder_buffer.h",
        "thirdparty/draco/src/draco/core/hash_utils.cc",
        "thirdparty/draco/src/draco/core/hash_utils.h",
        "thirdparty/draco/src/draco/core/macros.h",
        "thirdparty/draco/src/draco/core/math_utils.h",
        "thirdparty/draco/src/draco/core/options.cc",
        "thirdparty/draco/src/draco/core/options.h",
        "thirdparty/draco/src/draco/core/quantization_utils.cc",
        "thirdparty/draco/src/draco/core/quantization_utils.h",
        "thirdparty/draco/src/draco/core/rans_symbol_coding.h",
        "thirdparty/draco/src/draco/core/rans_symbol_decoder.h",
        "thirdparty/draco/src/draco/core/rans_symbol_encoder.h",
        "thirdparty/draco/src/draco/core/shannon_entropy.cc",
        "thirdparty/draco/src/draco/core/shannon_entropy.h",
        "thirdparty/draco/src/draco/core/status.h",
        "thirdparty/draco/src/draco/core/statusor.h",
        "thirdparty/draco/src/draco/core/symbol_coding_utils.cc",
        "thirdparty/draco/src/draco/core/symbol_coding_utils.h",
        "thirdparty/draco/src/draco/core/symbol_decoding.cc",
        "thirdparty/draco/src/draco/core/symbol_decoding.h",
        "thirdparty/draco/src/draco/core/symbol_encoding.cc",
        "thirdparty/draco/src/draco/core/symbol_encoding.h",
        "thirdparty/draco/src/draco/core/varint_decoding.h",
        "thirdparty/draco/src/draco/core/varint_encoding.h",
        "thirdparty/draco/src/draco/core/vector_d.h",
    #set(draco_core_bit_coders_sources
        "thirdparty/draco/src/draco/core/bit_coders/adaptive_rans_bit_coding_shared.h",
        "thirdparty/draco/src/draco/core/bit_coders/adaptive_rans_bit_decoder.cc",
        "thirdparty/draco/src/draco/core/bit_coders/adaptive_rans_bit_decoder.h",
        "thirdparty/draco/src/draco/core/bit_coders/adaptive_rans_bit_encoder.cc",
        "thirdparty/draco/src/draco/core/bit_coders/adaptive_rans_bit_encoder.h",
        "thirdparty/draco/src/draco/core/bit_coders/direct_bit_decoder.cc",
        "thirdparty/draco/src/draco/core/bit_coders/direct_bit_decoder.h",
        "thirdparty/draco/src/draco/core/bit_coders/direct_bit_encoder.cc",
        "thirdparty/draco/src/draco/core/bit_coders/direct_bit_encoder.h",
        "thirdparty/draco/src/draco/core/bit_coders/folded_integer_bit_decoder.h",
        "thirdparty/draco/src/draco/core/bit_coders/folded_integer_bit_encoder.h",
        "thirdparty/draco/src/draco/core/bit_coders/rans_bit_decoder.cc",
        "thirdparty/draco/src/draco/core/bit_coders/rans_bit_decoder.h",
        "thirdparty/draco/src/draco/core/bit_coders/rans_bit_encoder.cc",
        "thirdparty/draco/src/draco/core/bit_coders/rans_bit_encoder.h",
        "thirdparty/draco/src/draco/core/bit_coders/symbol_bit_decoder.cc",
        "thirdparty/draco/src/draco/core/bit_coders/symbol_bit_decoder.h",
        "thirdparty/draco/src/draco/core/bit_coders/symbol_bit_encoder.cc",
        "thirdparty/draco/src/draco/core/bit_coders/symbol_bit_encoder.h",
    #set(draco_io_sources
        "thirdparty/draco/src/draco/io/mesh_io.cc",
        "thirdparty/draco/src/draco/io/mesh_io.h",
        "thirdparty/draco/src/draco/io/obj_decoder.cc",
        "thirdparty/draco/src/draco/io/obj_decoder.h",
        "thirdparty/draco/src/draco/io/obj_encoder.cc",
        "thirdparty/draco/src/draco/io/obj_encoder.h",
        "thirdparty/draco/src/draco/io/parser_utils.cc",
        "thirdparty/draco/src/draco/io/parser_utils.h",
        "thirdparty/draco/src/draco/io/ply_decoder.cc",
        "thirdparty/draco/src/draco/io/ply_decoder.h",
        "thirdparty/draco/src/draco/io/ply_encoder.cc",
        "thirdparty/draco/src/draco/io/ply_encoder.h",
        "thirdparty/draco/src/draco/io/ply_property_reader.h",
        "thirdparty/draco/src/draco/io/ply_property_writer.h",
        "thirdparty/draco/src/draco/io/ply_reader.cc",
        "thirdparty/draco/src/draco/io/ply_reader.h",
        "thirdparty/draco/src/draco/io/point_cloud_io.cc",
        "thirdparty/draco/src/draco/io/point_cloud_io.h",
    #set(draco_mesh_sources
        "thirdparty/draco/src/draco/mesh/corner_table.cc",
        "thirdparty/draco/src/draco/mesh/corner_table.h",
        "thirdparty/draco/src/draco/mesh/corner_table_iterators.h",
        "thirdparty/draco/src/draco/mesh/corner_table_traversal_processor.h",
        "thirdparty/draco/src/draco/mesh/edgebreaker_observer.h",
        "thirdparty/draco/src/draco/mesh/edgebreaker_traverser.h",
        "thirdparty/draco/src/draco/mesh/mesh.cc",
        "thirdparty/draco/src/draco/mesh/mesh.h",
        "thirdparty/draco/src/draco/mesh/mesh_are_equivalent.cc",
        "thirdparty/draco/src/draco/mesh/mesh_are_equivalent.h",
        "thirdparty/draco/src/draco/mesh/mesh_attribute_corner_table.cc",
        "thirdparty/draco/src/draco/mesh/mesh_attribute_corner_table.h",
        "thirdparty/draco/src/draco/mesh/mesh_cleanup.cc",
        "thirdparty/draco/src/draco/mesh/mesh_cleanup.h",
        "thirdparty/draco/src/draco/mesh/mesh_misc_functions.cc",
        "thirdparty/draco/src/draco/mesh/mesh_misc_functions.h",
        "thirdparty/draco/src/draco/mesh/mesh_stripifier.cc",
        "thirdparty/draco/src/draco/mesh/mesh_stripifier.h",
        "thirdparty/draco/src/draco/mesh/triangle_soup_mesh_builder.cc",
        "thirdparty/draco/src/draco/mesh/triangle_soup_mesh_builder.h",
    #set(draco_point_cloud_sources
        "thirdparty/draco/src/draco/point_cloud/point_cloud.cc",
        "thirdparty/draco/src/draco/point_cloud/point_cloud.h",
        "thirdparty/draco/src/draco/point_cloud/point_cloud_builder.cc",
        "thirdparty/draco/src/draco/point_cloud/point_cloud_builder.h",
    #set(draco_points_common_sources
        "thirdparty/draco/src/draco/compression/point_cloud/algorithms/point_cloud_compression_method.h",
        "thirdparty/draco/src/draco/compression/point_cloud/algorithms/point_cloud_types.h",
        "thirdparty/draco/src/draco/compression/point_cloud/algorithms/quantize_points_3.h",
        "thirdparty/draco/src/draco/compression/point_cloud/algorithms/queuing_policy.h",
    #set(draco_points_dec_sources
        "thirdparty/draco/src/draco/compression/point_cloud/algorithms/dynamic_integer_points_kd_tree_decoder.cc",
        "thirdparty/draco/src/draco/compression/point_cloud/algorithms/dynamic_integer_points_kd_tree_decoder.h",
        "thirdparty/draco/src/draco/compression/point_cloud/algorithms/float_points_tree_decoder.cc",
        "thirdparty/draco/src/draco/compression/point_cloud/algorithms/float_points_tree_decoder.h",
    #set(draco_points_enc_sources
        "thirdparty/draco/src/draco/compression/point_cloud/algorithms/dynamic_integer_points_kd_tree_encoder.cc",
        "thirdparty/draco/src/draco/compression/point_cloud/algorithms/dynamic_integer_points_kd_tree_encoder.h",
        "thirdparty/draco/src/draco/compression/point_cloud/algorithms/float_points_tree_encoder.cc",
        "thirdparty/draco/src/draco/compression/point_cloud/algorithms/float_points_tree_encoder.h",
    #set(draco_metadata_sources
        "thirdparty/draco/src/draco/metadata/geometry_metadata.cc",
        "thirdparty/draco/src/draco/metadata/geometry_metadata.h",
        "thirdparty/draco/src/draco/metadata/metadata.cc",
        "thirdparty/draco/src/draco/metadata/metadata.h",
    #set(draco_metadata_enc_sources
        "thirdparty/draco/src/draco/metadata/metadata_encoder.cc",
        "thirdparty/draco/src/draco/metadata/metadata_encoder.h",
    #set(draco_metadata_dec_sources
        "thirdparty/draco/src/draco/metadata/metadata_decoder.cc",
        "thirdparty/draco/src/draco/metadata/metadata_decoder.h",
    #set(draco_js_dec_sources
#        "thirdparty/draco/src/draco/javascript/emscripten/decoder_webidl_wrapper.cc",
#        "thirdparty/draco/src/draco/javascript/emscripten/draco_decoder_glue_wrapper.cc",
    #set(draco_js_enc_sources
#        "thirdparty/draco/src/draco/javascript/emscripten/draco_encoder_glue_wrapper.cc",
#        "thirdparty/draco/src/draco/javascript/emscripten/encoder_webidl_wrapper.cc",
    #set(draco_test_sources
#        "thirdparty/draco/src/draco/attributes/point_attribute_test.cc",
#        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/prediction_scheme_normal_octahedron_canonicalized_transform_test.cc",
#        "thirdparty/draco/src/draco/compression/attributes/prediction_schemes/prediction_scheme_normal_octahedron_transform_test.cc",
#        "thirdparty/draco/src/draco/compression/attributes/sequential_integer_attribute_encoding_test.cc",
#        "thirdparty/draco/src/draco/compression/decode_test.cc",
#        "thirdparty/draco/src/draco/compression/encode_test.cc",
#        "thirdparty/draco/src/draco/compression/mesh/mesh_edgebreaker_encoding_test.cc",
#        "thirdparty/draco/src/draco/compression/mesh/mesh_encoder_test.cc",
#        "thirdparty/draco/src/draco/compression/point_cloud/point_cloud_kd_tree_encoding_test.cc",
#        "thirdparty/draco/src/draco/compression/point_cloud/point_cloud_sequential_encoding_test.cc",
#        "thirdparty/draco/src/draco/core/bit_coders/rans_coding_test.cc",
#        "thirdparty/draco/src/draco/core/buffer_bit_coding_test.cc",
#        "thirdparty/draco/src/draco/core/draco_test_base.h",
#        "thirdparty/draco/src/draco/core/draco_test_utils.cc",
#        "thirdparty/draco/src/draco/core/draco_test_utils.h",
#        "thirdparty/draco/src/draco/core/draco_tests.cc",
#        "thirdparty/draco/src/draco/core/math_utils_test.cc",
#        "thirdparty/draco/src/draco/core/quantization_utils_test.cc",
#        "thirdparty/draco/src/draco/core/status_test.cc",
#        "thirdparty/draco/src/draco/core/symbol_coding_test.cc",
#        "thirdparty/draco/src/draco/core/vector_d_test.cc",
#        "thirdparty/draco/src/draco/io/obj_decoder_test.cc",
#        "thirdparty/draco/src/draco/io/obj_encoder_test.cc",
#        "thirdparty/draco/src/draco/io/ply_decoder_test.cc",
#        "thirdparty/draco/src/draco/io/ply_reader_test.cc",
#        "thirdparty/draco/src/draco/io/point_cloud_io_test.cc",
#        "thirdparty/draco/src/draco/mesh/mesh_are_equivalent_test.cc",
#        "thirdparty/draco/src/draco/mesh/mesh_cleanup_test.cc",
#        "thirdparty/draco/src/draco/mesh/triangle_soup_mesh_builder_test.cc",
#        "thirdparty/draco/src/draco/metadata/metadata_encoder_test.cc",
#        "thirdparty/draco/src/draco/metadata/metadata_test.cc",
#        "thirdparty/draco/src/draco/point_cloud/point_cloud_builder_test.cc",
#        "thirdparty/draco/src/draco/point_cloud/point_cloud_test.cc",
    #set(draco_version_sources
#       "thirdparty/draco/src/draco/core/draco_version.cc",
        "thirdparty/draco/src/draco/core/draco_version.h",
    ],
    includes = [
        "thirdparty/draco/src/"
    ],  
    copts = [
        "-O2", 
        "/MD",
        "-DDRACO_MESH_COMPRESSION_SUPPORTED",
        "-DDRACO_POINT_CLOUD_COMPRESSION_SUPPORTED",
        "-DDRACO_PREDICTIVE_EDGEBREAKER_SUPPORTED",
        "-DDRACO_STANDARD_EDGEBREAKER_SUPPORTED",
        "-DDRACO_BACKWARDS_COMPATIBILITY_SUPPORTED",
    ],
)