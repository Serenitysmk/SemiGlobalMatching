#ifndef SRC_SEMI_GLOBAL_MATCHING_H_
#define SRC_SRMI_GLOBAL_MATCHING_H_

#include <util/types.h>

class SemiGlobalMatching {
 public:
  struct Options {
    uint8_t num_paths = 8;

    int32_t min_disparity = 0;

    int32_t max_disparity = 640;

    int32_t p1 = 10;

    int32_t p2_int = 150;
  };

  SemiGlobalMatching(const SemiGlobalMatching::Options& options,
                     const size_t width, const size_t height);

 private:


  size_t width_;

  size_t height_;

  Options options_;
};

#endif