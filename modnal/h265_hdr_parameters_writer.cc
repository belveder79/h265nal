#include "h265_hdr_parameters_writer.h"
#include "h265_sub_layer_hdr_parameters_writer.h"

bool WriteHrdParameters(H265HrdParametersParser::HrdParametersState* hrd_parameters,
                        BitBufferWriter* bit_buffer,
                        uint32_t commonInfPresentFlag, uint32_t maxNumSubLayersMinus1) noexcept {
    
    // H265 hrd_parameters NAL Unit.
    // Section E.2.2 ("HRD parameters syntax") of the H.265
    // standard for a complete description.
    
    if (commonInfPresentFlag) {
        // nal_hrd_parameters_present_flag  u(1)
        if (!bit_buffer->WriteBits(
                                   hrd_parameters->nal_hrd_parameters_present_flag, 1)) {
                                       return false;
                                   }
        
        // vcl_hrd_parameters_present_flag  u(1)
        if (!bit_buffer->WriteBits(
                                   hrd_parameters->vcl_hrd_parameters_present_flag, 1)) {
                                       return false;
                                   }
        
        if (hrd_parameters->nal_hrd_parameters_present_flag ||
            hrd_parameters->vcl_hrd_parameters_present_flag) {
            // sub_pic_hrd_params_present_flag  u(1)
            if (!bit_buffer->WriteBits(
                                       hrd_parameters->sub_pic_hrd_params_present_flag, 1)) {
                                           return false;
                                       }
            
            if (hrd_parameters->sub_pic_hrd_params_present_flag) {
                // tick_divisor_minus2  u(8)
                if (!bit_buffer->WriteBits(hrd_parameters->tick_divisor_minus2, 8)) {
                    return false;
                }
                
                // du_cpb_removal_delay_increment_length_minus1  u(5)
                if (!bit_buffer->WriteBits(
                                           hrd_parameters->du_cpb_removal_delay_increment_length_minus1, 5)) {
                                               return false;
                                           }
                
                // sub_pic_cpb_params_in_pic_timing_sei_flag  u(1)
                if (!bit_buffer->WriteBits(
                                           hrd_parameters->sub_pic_cpb_params_in_pic_timing_sei_flag, 1)) {
                                               return false;
                                           }
                
                // dpb_output_delay_du_length_minus1  u(5)
                if (!bit_buffer->WriteBits(
                                           hrd_parameters->dpb_output_delay_du_length_minus1, 5)) {
                                               return false;
                                           }
            }
            
            // bit_rate_scale  u(4)
            if (!bit_buffer->WriteBits(hrd_parameters->bit_rate_scale, 4)) {
                return false;
            }
            
            // cpb_size_scale  u(4)
            if (!bit_buffer->WriteBits(hrd_parameters->cpb_size_scale, 4)) {
                return false;
            }
            
            if (hrd_parameters->sub_pic_hrd_params_present_flag) {
                // cpb_size_du_scale  u(4)
                if (!bit_buffer->WriteBits(hrd_parameters->cpb_size_du_scale, 4)) {
                    return false;
                }
            }
            
            // initial_cpb_removal_delay_length_minus1  u(5)
            if (!bit_buffer->WriteBits(
                                       hrd_parameters->initial_cpb_removal_delay_length_minus1, 5)) {
                                           return false;
                                       }
            
            // au_cpb_removal_delay_length_minus1  u(5)
            if (!bit_buffer->WriteBits(
                                       hrd_parameters->au_cpb_removal_delay_length_minus1, 5)) {
                                           return false;
                                       }
            
            // dpb_output_delay_length_minus1  u(5)
            if (!bit_buffer->WriteBits(
                                       hrd_parameters->dpb_output_delay_length_minus1, 5)) {
                                           return false;
                                       }
        }
    }
    
    for (uint32_t i = 0; i <= maxNumSubLayersMinus1; i++) {
        // fixed_pic_rate_general_flag[i]  u(1)
        if (!bit_buffer->WriteBits(hrd_parameters->fixed_pic_rate_general_flag[i], 1)) {
            return false;
        }
        
        if (!hrd_parameters->fixed_pic_rate_general_flag[i]) {
            // fixed_pic_rate_within_cvs_flag[i]  u(1)
            if (!bit_buffer->WriteBits(hrd_parameters->fixed_pic_rate_within_cvs_flag[i], 1)) {
                return false;
            }
            // } else {
            //  hrd_parameters->fixed_pic_rate_within_cvs_flag.push_back(0);
        }
        
        if (hrd_parameters->fixed_pic_rate_within_cvs_flag[i]) {
            // elemental_duration_in_tc_minus1[i]  ue(v)
            if (!bit_buffer->WriteExponentialGolomb(hrd_parameters->elemental_duration_in_tc_minus1[i])) {
                return false;
            }
            // // need to set low_delay_hrd_flag[i] to 0
            // hrd_parameters->low_delay_hrd_flag.push_back(0);
        } else {
            // need to set elemental_duration_in_tc_minus1[i] to 0
            // hrd_parameters->elemental_duration_in_tc_minus1.push_back(0);
            
            // low_delay_hrd_flag[i]  u(1)
            if (!bit_buffer->WriteBits(hrd_parameters->low_delay_hrd_flag[i], 1)) {
                return false;
            }
        }
        
        if (!hrd_parameters->low_delay_hrd_flag[i]) {
            // cpb_cnt_minus1[i]  ue(v)
            if (!bit_buffer->WriteExponentialGolomb(hrd_parameters->cpb_cnt_minus1[i])) {
                return false;
            }
            //} else {
            //  // need to set cpb_cnt_minus1[i] to 0
            //  hrd_parameters->cpb_cnt_minus1.push_back(0);
        }
        
        if (hrd_parameters->nal_hrd_parameters_present_flag) {
            // sub_layer_hrd_parameters(i)
            auto CpbCnt = hrd_parameters->cpb_cnt_minus1[i] + 1;
            if(!WriteSubLayerHrdParameters(
                                           hrd_parameters->sub_layer_hrd_parameters_vector[i].get(),
                                           bit_buffer, CpbCnt,
                                           hrd_parameters->sub_pic_hrd_params_present_flag)) {
                                               return false;
                                           }
        }
        
        if (hrd_parameters->vcl_hrd_parameters_present_flag) {
            // sub_layer_hrd_parameters(i)
            auto CpbCnt = hrd_parameters->cpb_cnt_minus1[i] + 1;
            if(!WriteSubLayerHrdParameters(hrd_parameters->sub_layer_hrd_parameters_vector[i].get(),
                                           bit_buffer, CpbCnt,
                                           hrd_parameters->sub_pic_hrd_params_present_flag)) {
                return false;
            }
        }
    }
    return true;
}
