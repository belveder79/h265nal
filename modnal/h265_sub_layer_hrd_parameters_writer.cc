#include "h265_sub_layer_hrd_parameters_writer.h"

bool WriteSubLayerHrdParameters( H265SubLayerHrdParametersParser::SubLayerHrdParametersState* sub_layer_hrd_parameters,
                                BitBufferWriter* bit_buffer, uint32_t CpbCnt,
                                uint32_t sub_pic_hrd_params_present_flag) noexcept {
    
    // H265 sub_layer_hrd_parameters NAL Unit.
    // Section E.2.3 ("Sub-layer HRD parameters syntax") of the H.265
    // standard for a complete description.
    
    for (uint32_t i = 0; i < CpbCnt; i++) {
        // bit_rate_value_minus1[i]  ue(v)
        if (!bit_buffer->WriteExponentialGolomb(sub_layer_hrd_parameters->bit_rate_value_minus1[i])) {
            return false;
        }
        
        // cpb_size_value_minus1[i]  ue(v)
        if (!bit_buffer->WriteExponentialGolomb(sub_layer_hrd_parameters->cpb_size_value_minus1[i])) {
            return false;
        }
        
        if (sub_pic_hrd_params_present_flag) {
            // cpb_size_du_value_minus1[i]  ue(v)
            if (!bit_buffer->WriteExponentialGolomb(sub_layer_hrd_parameters->cpb_size_du_value_minus1[i])) {
                return false;
            }
            
            // bit_rate_du_value_minus1[i]  ue(v)
            if (!bit_buffer->WriteExponentialGolomb(sub_layer_hrd_parameters->bit_rate_du_value_minus1[i])) {
                return false;
            }
        //} else {
        //    sub_layer_hrd_parameters->cpb_size_du_value_minus1.push_back(0);
        //    sub_layer_hrd_parameters->bit_rate_du_value_minus1.push_back(0);
        }
        
        // cbr_flag[i]  u(1)
        if (!bit_buffer->WriteBits(sub_layer_hrd_parameters->cbr_flag[i], 1)) {
            return false;
        }
    }
    
    return true;
}
