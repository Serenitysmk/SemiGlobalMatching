#ifndef SRC_SEMI_GLOBAL_MATCHING_H_
#define SRC_SRMI_GLOBAL_MATCHING_H_

#include <opencv2/opencv.hpp>

#include <mvs/mat.h>
#include <util/types.h>

class SemiGlobalMatching {
 public:
  struct Options {
    uint8_t num_paths = 8;

    int32_t min_disparity = 0;

    int32_t max_disparity = 64;

    int32_t p1 = 10;

    int32_t p2_int = 150;
  };

  SemiGlobalMatching(const SemiGlobalMatching::Options& options,
                     const size_t width, const size_t height);

  bool Match(const cv::Mat& img_left, const cv::Mat& img_right,
             cv::Mat& disparity_map);

 protected:
  void CensusTransform(const cv::Mat& img_left, const cv::Mat& img_right);
  void ComputeCost();
  // Compute disparity via Winner-Takes-All.
  void ComputeDisparity();
  void ToDisparityMap(cv::Mat& disparity_map);

  void ComputeCensus(const cv::Mat& img, colmap::mvs::Mat<uint32_t>& census,
                     const size_t window_size);

  uint16_t HammingDist(const uint32_t& x, const uint32_t& y);

  Options options_;

  size_t width_;
  size_t height_;

  colmap::mvs::Mat<uint32_t> census_left_;
  colmap::mvs::Mat<uint32_t> census_right_;

  // Initial cost.
  colmap::mvs::Mat<uint8_t> cost_init_;
  // Aggregated cost.
  colmap::mvs::Mat<uint16_t> cost_aggr_;

  colmap::mvs::Mat<float> disp_left_;
};

#endif