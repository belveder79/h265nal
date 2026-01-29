#include "h265_scaling_list_data_writer.h"

bool WriteScalingListData(const H265ScalingListDataParser::ScalingListDataState* scaling_list_data,
    BitBufferWriter* bit_buffer) noexcept {
  // H265 scaling_list_data() NAL Unit.
  // Section 7.3.4 ("Scaling list data syntax") of the H.265
  // standard for a complete description.
    
  // calculate the values
  for (uint32_t sizeId = 0; sizeId < 4; sizeId++) {
    for (uint32_t matrixId = 0; matrixId < 6;
         matrixId += (sizeId == 3) ? 3 : 1) {
      // scaling_list_pred_mode_flag[sizeId][matrixId]  u(1)
      if (!bit_buffer->WriteBits(
              scaling_list_data
                     ->scaling_list_pred_mode_flag[sizeId][matrixId],1)) {
        return false;
      }

      if (!scaling_list_data->scaling_list_pred_mode_flag[sizeId][matrixId]) {
        // scaling_list_pred_matrix_id_delta[sizeId][matrixId]  ue(v)
        if (!bit_buffer->WriteExponentialGolomb(
                scaling_list_data
                    ->scaling_list_pred_matrix_id_delta[sizeId][matrixId])) {
          return false;
        }

      } else {
/*
        uint32_t nextCoef = 8;
        uint32_t coefNum =
            std::min(64U, static_cast<uint32_t>(1 << (4 + (sizeId << 1))));
        if (sizeId > 1) {
          // scaling_list_dc_coef_minus8[sizeId - 2][matrixId]  se(v)
          if (!bit_buffer->WriteSignedExponentialGolomb(
                  scaling_list_data
                      ->scaling_list_dc_coef_minus8[sizeId - 2][matrixId])) {
            return false;
          }
          nextCoef = static_cast<uint32_t>(
              scaling_list_data
                  ->scaling_list_dc_coef_minus8[sizeId - 2][matrixId] +
              8);
        }
#ifdef CHECK
        for (uint32_t i = 0; i < coefNum; i++) {
          // scaling_list_delta_coef  se(v)
          int32_t scaling_list_delta_coef;
          if (!bit_buffer->ReadSignedExponentialGolomb(
                  scaling_list_delta_coef)) {
            return false;
          }
          nextCoef = static_cast<uint32_t>(static_cast<int32_t>(nextCoef) +
                                           scaling_list_delta_coef + 256) %
                     256;
          scaling_list_data->ScalingList[sizeId][matrixId].push_back(nextCoef);
        }
#else
        std::cout << "UNIMPLEMENTED!" << std::endl;
#endif
*/
          std::cout << "ScalingList UNIMPLEMENTED!" << std::endl;
      }
    }
  }

  return true;
}
