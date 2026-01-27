#include "h265_st_ref_pic_set_writer.h"

bool WriteStRefPicSet(H265StRefPicSetParser::StRefPicSetState* st_ref_pic_set,
    BitBufferWriter* bit_buffer) noexcept {
    
  // H265 st_ref_pic_set() NAL Unit.
  // Section 7.3.7 ("Short-term reference picture set syntax parameter set
  // syntax") of the H.265 standard for a complete description.

  if (st_ref_pic_set->stRpsIdx != 0) {
    // inter_ref_pic_set_prediction_flag  u(1)
    if (!bit_buffer->WriteBits(
            st_ref_pic_set->inter_ref_pic_set_prediction_flag, 1)) {
      return false;
    }
  }

  if (st_ref_pic_set->inter_ref_pic_set_prediction_flag) {
    if (st_ref_pic_set->stRpsIdx == st_ref_pic_set->num_short_term_ref_pic_sets) {
      // delta_idx_minus1  ue(v)
      if (!bit_buffer->WriteExponentialGolomb(
              st_ref_pic_set->delta_idx_minus1)) {
        return false;
      }
    }

    // delta_rps_sign  u(1)
    if (!bit_buffer->WriteBits(st_ref_pic_set->delta_rps_sign, 1)) {
      return false;
    }

    // abs_delta_rps_minus1  ue(v)
    if (!bit_buffer->WriteExponentialGolomb(
            st_ref_pic_set->abs_delta_rps_minus1)) {
      return false;
    }

    for (uint32_t j = 0; j <= st_ref_pic_set->used_by_curr_pic_flag.size(); j++) {
      // used_by_curr_pic_flag[j]  u(1)
      if (!bit_buffer->WriteBits(st_ref_pic_set->used_by_curr_pic_flag[j], 1)) {
        return false;
      }

        if (!st_ref_pic_set->used_by_curr_pic_flag.back()) {
            // use_delta_flag[j]  u(1)
            if (!bit_buffer->WriteBits(st_ref_pic_set->use_delta_flag[j], 1)) {
                return false;
            }
        }
    }

    // st_ref_pic_set->DeriveValues(st_ref_pic_set_state_vector, RefRpsIdx);

  } else {
    // num_negative_pics  ue(v)
    if (!bit_buffer->WriteExponentialGolomb(st_ref_pic_set->num_negative_pics)) {
      return false;
    }

    // num_positive_pics  ue(v)
    if (!bit_buffer->WriteExponentialGolomb(st_ref_pic_set->num_positive_pics)) {
      return false;
    }

    for (uint32_t i = 0; i < st_ref_pic_set->num_negative_pics; i++) {
      // delta_poc_s0_minus1[i] ue(v)
      if (!bit_buffer->WriteExponentialGolomb(st_ref_pic_set->delta_poc_s0_minus1[i])) {
        return false;
      }

      // used_by_curr_pic_s0_flag[i] u(1)
      if (!bit_buffer->WriteBits(st_ref_pic_set->used_by_curr_pic_s0_flag[i], 1)) {
        return false;
      }
    }

    for (uint32_t i = 0; i < st_ref_pic_set->num_positive_pics; i++) {
      // delta_poc_s1_minus1[i] ue(v)
      if (!bit_buffer->WriteExponentialGolomb(st_ref_pic_set->delta_poc_s1_minus1[i])) {
        return false;
      }

      // used_by_curr_pic_s1_flag[i] u(1)
      if (!bit_buffer->WriteBits(st_ref_pic_set->used_by_curr_pic_s1_flag[i], 1)) {
        return false;
      }
    }
  }
    return true;
}

