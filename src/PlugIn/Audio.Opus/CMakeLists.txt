IF(WIN32)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/opus)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/opus/include)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/opus/src)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/opus/win32)
    
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/opus/celt)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/opus/silk)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/opus/silk/float)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/opus/silk/fixed)
    
    add_definitions(-DHAVE_CONFIG_H)
    add_definitions(-D_LIB)
    
    SET(SILK_COMMON_SOURCE  opus/silk/A2NLSF.c
                            opus/silk/ana_filt_bank_1.c
                            opus/silk/biquad_alt.c
                            opus/silk/bwexpander.c
                            opus/silk/bwexpander_32.c
                            opus/silk/check_control_input.c
                            opus/silk/CNG.c
                            opus/silk/code_signs.c
                            opus/silk/control_audio_bandwidth.c
                            opus/silk/control_codec.c
                            opus/silk/control_SNR.c
                            opus/silk/debug.c
                            opus/silk/decoder_set_fs.c
                            opus/silk/decode_core.c
                            opus/silk/decode_frame.c
                            opus/silk/decode_indices.c
                            opus/silk/decode_parameters.c
                            opus/silk/decode_pitch.c
                            opus/silk/decode_pulses.c
                            opus/silk/dec_API.c
                            opus/silk/encode_indices.c
                            opus/silk/encode_pulses.c
                            opus/silk/enc_API.c
                            opus/silk/gain_quant.c
                            opus/silk/HP_variable_cutoff.c
                            opus/silk/init_decoder.c
                            opus/silk/init_encoder.c
                            opus/silk/inner_prod_aligned.c
                            opus/silk/interpolate.c
                            opus/silk/lin2log.c
                            opus/silk/log2lin.c
                            opus/silk/LPC_analysis_filter.c
                            opus/silk/LPC_inv_pred_gain.c
                            opus/silk/LP_variable_cutoff.c
                            opus/silk/NLSF2A.c
                            opus/silk/NLSF_decode.c
                            opus/silk/NLSF_del_dec_quant.c
                            opus/silk/NLSF_encode.c
                            opus/silk/NLSF_stabilize.c
                            opus/silk/NLSF_unpack.c
                            opus/silk/NLSF_VQ.c
                            opus/silk/NLSF_VQ_weights_laroia.c
                            opus/silk/NSQ.c
                            opus/silk/NSQ_del_dec.c
                            opus/silk/pitch_est_tables.c
                            opus/silk/PLC.c
                            opus/silk/process_NLSFs.c
                            opus/silk/quant_LTP_gains.c
                            opus/silk/resampler.c
                            opus/silk/resampler_down2.c
                            opus/silk/resampler_down2_3.c
                            opus/silk/resampler_private_AR2.c
                            opus/silk/resampler_private_down_FIR.c
                            opus/silk/resampler_private_IIR_FIR.c
                            opus/silk/resampler_private_up2_HQ.c
                            opus/silk/resampler_rom.c
                            opus/silk/shell_coder.c
                            opus/silk/sigm_Q15.c
                            opus/silk/sort.c
                            opus/silk/stereo_decode_pred.c
                            opus/silk/stereo_encode_pred.c
                            opus/silk/stereo_find_predictor.c
                            opus/silk/stereo_LR_to_MS.c
                            opus/silk/stereo_MS_to_LR.c
                            opus/silk/stereo_quant_pred.c
                            opus/silk/sum_sqr_shift.c
                            opus/silk/tables_gain.c
                            opus/silk/tables_LTP.c
                            opus/silk/tables_NLSF_CB_NB_MB.c
                            opus/silk/tables_NLSF_CB_WB.c
                            opus/silk/tables_other.c
                            opus/silk/tables_pitch_lag.c
                            opus/silk/tables_pulses_per_block.c
                            opus/silk/table_LSF_cos.c
                            opus/silk/VAD.c
                            opus/silk/VQ_WMat_EC.c
                            opus/silk/x86/NSQ_del_dec_sse.c
                            opus/silk/x86/NSQ_sse.c
                            opus/silk/x86/VAD_sse.c
                            opus/silk/x86/VQ_WMat_EC_sse.c
                            opus/silk/x86/x86_silk_map.c)
                            
    SET(SILK_FIXED_SOURCE   opus/silk/fixed/apply_sine_window_FIX.c
                            opus/silk/fixed/autocorr_FIX.c
                            opus/silk/fixed/burg_modified_FIX.c
                            opus/silk/fixed/corrMatrix_FIX.c
                            opus/silk/fixed/encode_frame_FIX.c
                            opus/silk/fixed/find_LPC_FIX.c
                            opus/silk/fixed/find_LTP_FIX.c
                            opus/silk/fixed/find_pitch_lags_FIX.c
                            opus/silk/fixed/find_pred_coefs_FIX.c
                            opus/silk/fixed/k2a_FIX.c
                            opus/silk/fixed/k2a_Q16_FIX.c
                            opus/silk/fixed/LTP_analysis_filter_FIX.c
                            opus/silk/fixed/LTP_scale_ctrl_FIX.c
                            opus/silk/fixed/noise_shape_analysis_FIX.c
                            opus/silk/fixed/pitch_analysis_core_FIX.c
                            opus/silk/fixed/prefilter_FIX.c
                            opus/silk/fixed/process_gains_FIX.c
                            opus/silk/fixed/regularize_correlations_FIX.c
                            opus/silk/fixed/residual_energy16_FIX.c
                            opus/silk/fixed/residual_energy_FIX.c
                            opus/silk/fixed/schur64_FIX.c
                            opus/silk/fixed/schur_FIX.c
                            opus/silk/fixed/solve_LS_FIX.c
                            opus/silk/fixed/vector_ops_FIX.c
                            opus/silk/fixed/warped_autocorrelation_FIX.c
                            opus/silk/fixed/x86/burg_modified_FIX_sse.c
                            opus/silk/fixed/x86/prefilter_FIX_sse.c
                            opus/silk/fixed/x86/vector_ops_FIX_sse.c)
    
    SET(SILK_FLOAT_SOURCE   opus/silk/float/apply_sine_window_FLP.c
                            opus/silk/float/autocorrelation_FLP.c
                            opus/silk/float/burg_modified_FLP.c
                            opus/silk/float/bwexpander_FLP.c
                            opus/silk/float/corrMatrix_FLP.c
                            opus/silk/float/encode_frame_FLP.c
                            opus/silk/float/energy_FLP.c
                            opus/silk/float/find_LPC_FLP.c
                            opus/silk/float/find_LTP_FLP.c
                            opus/silk/float/find_pitch_lags_FLP.c
                            opus/silk/float/find_pred_coefs_FLP.c
                            opus/silk/float/inner_product_FLP.c
                            opus/silk/float/k2a_FLP.c
                            opus/silk/float/levinsondurbin_FLP.c
                            opus/silk/float/LPC_analysis_filter_FLP.c
                            opus/silk/float/LPC_inv_pred_gain_FLP.c
                            opus/silk/float/LTP_analysis_filter_FLP.c
                            opus/silk/float/LTP_scale_ctrl_FLP.c
                            opus/silk/float/noise_shape_analysis_FLP.c
                            opus/silk/float/pitch_analysis_core_FLP.c
                            opus/silk/float/prefilter_FLP.c
                            opus/silk/float/process_gains_FLP.c
                            opus/silk/float/regularize_correlations_FLP.c
                            opus/silk/float/residual_energy_FLP.c
                            opus/silk/float/scale_copy_vector_FLP.c
                            opus/silk/float/scale_vector_FLP.c
                            opus/silk/float/schur_FLP.c
                            opus/silk/float/solve_LS_FLP.c
                            opus/silk/float/sort_FLP.c
                            opus/silk/float/warped_autocorrelation_FLP.c
                            opus/silk/float/wrappers_FLP.c)
    
    SET(CELT_SOURCE opus/celt/bands.c
                    opus/celt/celt.c
                    opus/celt/celt_decoder.c
                    opus/celt/celt_encoder.c
                    opus/celt/celt_lpc.c
                    opus/celt/x86/celt_lpc_sse.c
                    opus/celt/cwrs.c
                    opus/celt/entcode.c
                    opus/celt/entdec.c
                    opus/celt/entenc.c
                    opus/celt/kiss_fft.c
                    opus/celt/laplace.c
                    opus/celt/mathops.c
                    opus/celt/mdct.c
                    opus/celt/modes.c
                    opus/celt/pitch.c
                    opus/celt/x86/pitch_sse.c
                    opus/celt/x86/pitch_sse2.c
                    opus/celt/x86/pitch_sse4_1.c
                    opus/celt/quant_bands.c
                    opus/celt/rate.c
                    opus/celt/vq.c
                    opus/celt/x86/x86_celt_map.c
                    opus/celt/x86/x86cpu.c)
    
    SET(OPUS_SOURCE    opus/src/analysis.c
                    opus/src/mlp.c
                    opus/src/mlp_data.c
                    opus/src/opus.c
                    opus/src/opus_compare.c
                    opus/src/opus_decoder.c
                    opus/src/opus_encoder.c
                    opus/src/opus_multistream.c
                    opus/src/opus_multistream_decoder.c
                    opus/src/opus_multistream_encoder.c
                    opus/src/repacketizer.c)

    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/opusfile/include)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/opusfile/src)

    SET(OPUSFILE_SOURCE    opusfile/src/info.c
                        opusfile/src/internal.c
                        opusfile/src/opusfile.c
                        opusfile/src/stream.c)

    add_library(CMP.Audio.Opus SHARED OpusRead.cpp ${OGG_SOURCE} ${SILK_COMMON_SOURCE} ${SILK_FIXED_SOURCE} ${SILK_FLOAT_SOURCE} ${CELT_SOURCE} ${OPUS_SOURCE} ${OPUSFILE_SOURCE})

    SOURCE_GROUP("LibOGG"                    FILES ${OGG_SOURCE})
    SOURCE_GROUP("LibOpus"                    FILES ${OPUS_SOURCE})
    SOURCE_GROUP("LibOpus\\CELT"            FILES ${CELT_SOURCE})
    SOURCE_GROUP("LibOpus\\SILK\\Common"    FILES ${SILK_COMMON_SOURCE})
    SOURCE_GROUP("LibOpus\\SILK\\Fixed"     FILES ${SILK_FIXED_SOURCE})
    SOURCE_GROUP("LibOpus\\SILK\\Float"     FILES ${SILK_FLOAT_SOURCE})
    SOURCE_GROUP("LibOpusFile"                FILES ${OPUSFILE_SOURCE})
ELSE()
    FIND_PATH(OPUS_INCLUDE_DIR
        NAMES opus.h
        HINTS
        /usr/include/opus
        /usr/local/include/opus)
    INCLUDE_DIRECTORIES(${OPUS_INCLUDE_DIR})

    FIND_PATH(OPUS_LIBRARY_DIR
        NAMES libopus.so
        HINTS
        ${LIB_3RD_FIND_HINT})
    LINK_DIRECTORIES(${OPUS_LIBRARY_DIR})

    add_library(CMP.Audio.Opus SHARED OpusRead.cpp)
    target_link_libraries(CMP.Audio.Opus opus opusfile)

    add_executable(OpusTest OpusTest.cpp)
    target_link_libraries(OpusTest opus opusfile)
ENDIF()
