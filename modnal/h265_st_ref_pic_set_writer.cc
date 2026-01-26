#include "h265_st_ref_pic_set_writer.h"

bool WriteStRefPicSet(H265StRefPicSetParser::StRefPicSetState* st_ref_pic_set,
    BitBufferWriter* bit_buffer) noexcept {
    
#pragma unused(st_ref_pic_set)
#pragma unused(bit_buffer)
    std::cout << "StRefPicSet UNIMPLEMENTED!" << std::endl;
/*

  uint32_t bits_tmp;
  uint32_t golomb_tmp;

  if (num_short_term_ref_pic_sets >
      h265limits::NUM_SHORT_TERM_REF_PIC_SETS_MAX) {
#ifdef FPRINT_ERRORS
    fprintf(stderr,
            "error: num_short_term_ref_pic_sets == %" PRIu32
            " > h265limits::NUM_SHORT_TERM_REF_PIC_SETS_MAX\n",
            num_short_term_ref_pic_sets);
#endif  // FPRINT_ERRORS
    return nullptr;
  }

  // H265 st_ref_pic_set() NAL Unit.
  // Section 7.3.7 ("Short-term reference picture set syntax parameter set
  // syntax") of the H.265 standard for a complete description.
  auto st_ref_pic_set = std::make_unique<StRefPicSetState>();

  st_ref_pic_set->stRpsIdx = stRpsIdx;
  st_ref_pic_set->num_short_term_ref_pic_sets = num_short_term_ref_pic_sets;

  if (stRpsIdx != 0) {
    // inter_ref_pic_set_prediction_flag  u(1)
    if (!bit_buffer->ReadBits(
            1, st_ref_pic_set->inter_ref_pic_set_prediction_flag)) {
      return nullptr;
    }
  }

  if (st_ref_pic_set->inter_ref_pic_set_prediction_flag) {
    if (stRpsIdx == st_ref_pic_set->num_short_term_ref_pic_sets) {
      // delta_idx_minus1  ue(v)
      if (!bit_buffer->ReadExponentialGolomb(
              st_ref_pic_set->delta_idx_minus1)) {
        return nullptr;
      }
    }
    if (st_ref_pic_set->delta_idx_minus1 < kDeltaIdxMinus1Min ||
        st_ref_pic_set->delta_idx_minus1 > (st_ref_pic_set->stRpsIdx - 1)) {
#ifdef FPRINT_ERRORS
      fprintf(stderr,
              "invalid delta_idx_minus1: %" PRIu32
              " not in range "
              "[%" PRIu32 ", %" PRIu32 "]\n",
              st_ref_pic_set->delta_idx_minus1, kDeltaIdxMinus1Min,
              st_ref_pic_set->stRpsIdx - 1);
#endif  // FPRINT_ERRORS
      return nullptr;
    }

    // delta_rps_sign  u(1)
    if (!bit_buffer->ReadBits(1, st_ref_pic_set->delta_rps_sign)) {
      return nullptr;
    }

    // abs_delta_rps_minus1  ue(v)
    if (!bit_buffer->ReadExponentialGolomb(
            st_ref_pic_set->abs_delta_rps_minus1)) {
      return nullptr;
    }
    if (st_ref_pic_set->abs_delta_rps_minus1 < kAbsDeltaRpsMinus1Min ||
        st_ref_pic_set->abs_delta_rps_minus1 > kAbsDeltaRpsMinus1Max) {
#ifdef FPRINT_ERRORS
      fprintf(stderr,
              "invalid abs_delta_rps_minus1: %" PRIu32
              " not in range "
              "[%" PRIu32 ", %" PRIu32 "]\n",
              st_ref_pic_set->abs_delta_rps_minus1, kAbsDeltaRpsMinus1Min,
              kAbsDeltaRpsMinus1Max);
#endif  // FPRINT_ERRORS
      return nullptr;
    }

    // Equation 7-59
    const uint32_t RefRpsIdx =
        st_ref_pic_set->stRpsIdx - (st_ref_pic_set->delta_idx_minus1 + 1);

    // Equation 7-71
    const auto& ref = (*st_ref_pic_set_state_vector)[RefRpsIdx];
    uint32_t NumDeltaPocs_RefRpsIdx =
        ref->num_negative_pics + ref->num_positive_pics;

    // Section F.7.4.8: DeltaPoCs shall be in range 0 to MaxDpbSize-1,
    // inclusive
    if (NumDeltaPocs_RefRpsIdx >= h265limits::HEVC_MAX_DPB_SIZE) {
      return nullptr;
    }

    for (uint32_t j = 0; j <= NumDeltaPocs_RefRpsIdx; j++) {
      // used_by_curr_pic_flag[j]  u(1)
      if (!bit_buffer->ReadBits(1, bits_tmp)) {
        return nullptr;
      }
      st_ref_pic_set->used_by_curr_pic_flag.push_back(bits_tmp);

      if (!st_ref_pic_set->used_by_curr_pic_flag.back()) {
        // use_delta_flag[j]  u(1)
        if (!bit_buffer->ReadBits(1, bits_tmp)) {
          return nullptr;
        }
      } else {
        // default use_delta_flag value (page 103)
        bits_tmp = 1;
      }
      st_ref_pic_set->use_delta_flag.push_back(bits_tmp);
    }

    st_ref_pic_set->DeriveValues(st_ref_pic_set_state_vector, RefRpsIdx);

  } else {
    // num_negative_pics  ue(v)
    if (!bit_buffer->ReadExponentialGolomb(st_ref_pic_set->num_negative_pics)) {
      return nullptr;
    }
    if (st_ref_pic_set->num_negative_pics < kNumNegativePicsMin ||
        st_ref_pic_set->num_negative_pics > max_num_pics) {
#ifdef FPRINT_ERRORS
      fprintf(stderr,
              "invalid num_negative_pics: %" PRIu32
              " not in range "
              "[%" PRIu32 ", %" PRIu32 "]\n",
              st_ref_pic_set->num_negative_pics, kNumNegativePicsMin,
              max_num_pics);
#endif  // FPRINT_ERRORS
      return nullptr;
    }

    // num_positive_pics  ue(v)
    if (!bit_buffer->ReadExponentialGolomb(st_ref_pic_set->num_positive_pics)) {
      return nullptr;
    }
    if (st_ref_pic_set->num_positive_pics < kNumPositivePicsMin ||
        st_ref_pic_set->num_positive_pics >
            (max_num_pics - st_ref_pic_set->num_negative_pics)) {
#ifdef FPRINT_ERRORS
      fprintf(stderr,
              "invalid num_positive_pics: %" PRIu32
              " not in range "
              "[%" PRIu32 ", %" PRIu32 "]\n",
              st_ref_pic_set->num_positive_pics, kNumPositivePicsMin,
              max_num_pics - st_ref_pic_set->num_negative_pics);
#endif  // FPRINT_ERRORS
      return nullptr;
    }

    for (uint32_t i = 0; i < st_ref_pic_set->num_negative_pics; i++) {
      // delta_poc_s0_minus1[i] ue(v)
      if (!bit_buffer->ReadExponentialGolomb(golomb_tmp)) {
        return nullptr;
      }
      st_ref_pic_set->delta_poc_s0_minus1.push_back(golomb_tmp);

      // used_by_curr_pic_s0_flag[i] u(1)
      if (!bit_buffer->ReadBits(1, bits_tmp)) {
        return nullptr;
      }
      st_ref_pic_set->used_by_curr_pic_s0_flag.push_back(bits_tmp);
    }

    for (uint32_t i = 0; i < st_ref_pic_set->num_positive_pics; i++) {
      // delta_poc_s1_minus1[i] ue(v)
      if (!bit_buffer->ReadExponentialGolomb(golomb_tmp)) {
        return nullptr;
      }
      st_ref_pic_set->delta_poc_s1_minus1.push_back(golomb_tmp);

      // used_by_curr_pic_s1_flag[i] u(1)
      if (!bit_buffer->ReadBits(1, bits_tmp)) {
        return nullptr;
      }
      st_ref_pic_set->used_by_curr_pic_s1_flag.push_back(bits_tmp);
    }
  }

  return st_ref_pic_set;
*/
    return true;
}

