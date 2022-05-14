#include "semi_global_matching.h"

SemiGlobalMatching::SemiGlobalMatching(
    const SemiGlobalMatching::Options& options, const size_t width,
    const size_t height)
    : options_(options), width_(width), height_(height) {
  census_left_ = colmap::mvs::Mat<uint32_t>(width_, height_, 1);
  census_right_ = colmap::mvs::Mat<uint32_t>(width_, height_, 1);

  const int32_t disp_range = options_.max_disparity - options.min_disparity;
  CHECK(disp_range > 0);

  cost_init_ = colmap::mvs::Mat<uint8_t>(width_, height_, disp_range);
  cost_aggr_ = colmap::mvs::Mat<uint16_t>(width_, height_, disp_range);

  disp_left_ = colmap::mvs::Mat<float>(width_, height_, 1);
}

bool SemiGlobalMatching::Match(const cv::Mat& img_left,
                               const cv::Mat& img_right,
                               cv::Mat& disparity_map) {
  if (img_left.empty() || img_right.empty()) {
    std::cerr << "ERROR: Image is empty" << std::endl;
    return false;
  }

  // Compute census transoform.
  CensusTransform(img_left, img_right);

  std::cout << "Computing cost ..." << std::endl;
  // Compute cost based on census transform.
  ComputeCost();

  std::cout << "Computing disparity ..." << std::endl;
  // Compute disparity via Winner-Takes-All.
  ComputeDisparity();

  ToDisparityMap(disparity_map);

  std::cout << "Finished stereo matching" << std::endl;
  return true;
}

void SemiGlobalMatching::CensusTransform(const cv::Mat& img_left,
                                         const cv::Mat& img_right) {
  ComputeCensus(img_left, census_left_, 5);
  ComputeCensus(img_right, census_right_, 5);
}

void SemiGlobalMatching::ComputeCost() {
  for (size_t i = 0; i < height_; i++) {
    for (size_t j = 0; j < width_; j++) {
      for (size_t disp_idx = 0; disp_idx < cost_init_.GetDepth(); disp_idx++) {
        const int32_t disp = disp_idx + options_.min_disparity;
        uint8_t cost = 0;
        if (j - disp < 0 || j - disp >= width_) {
          cost = std::numeric_limits<uint8_t>::max() / 2;
        } else {
          const uint32_t census_val_left = census_left_.Get(i, j);
          const uint32_t census_val_right = census_right_.Get(i, j - disp);
          cost = static_cast<uint8_t>(
              HammingDist(census_val_left, census_val_right));
          cost_init_.Set(i, j, disp_idx, cost);
        }
      }
    }
  }
}

void SemiGlobalMatching::ComputeDisparity() {
  for (size_t i = 0; i < height_; i++) {
    for (size_t j = 0; j < width_; j++) {
      uint16_t min_cost = std::numeric_limits<uint16_t>::max();
      uint16_t max_cost = 0;
      int32_t best_disparity = 0;

      for (size_t disp_idx = 0; disp_idx < cost_init_.GetDepth(); disp_idx++) {
        const int32_t disp = disp_idx + options_.min_disparity;
        const uint8_t cost = cost_init_.Get(i, j, disp_idx);
        if (cost < min_cost) {
          min_cost = cost;
          best_disparity = disp;
        }
        max_cost = std::max(max_cost, static_cast<uint16_t>(cost));
      }

      if (max_cost != min_cost) {
        disp_left_.Set(i, j, best_disparity);
      } else {
        disp_left_.Set(i, j, std::numeric_limits<float>::infinity());
      }
    }
  }
}

void SemiGlobalMatching::ToDisparityMap(cv::Mat& disparity_map) {
  disparity_map = cv::Mat(height_, width_, CV_8UC1);

  for (size_t i = 0; i < height_; i++) {
    for (size_t j = 0; j < width_; j++) {
      const float disp = disp_left_.Get(i, j);
      if (disp == std::numeric_limits<float>::infinity()) {
        disparity_map.at<uint8_t>(i, j) = 0;
      } else {
        disparity_map.at<uint8_t>(i, j) = 2 * static_cast<uint8_t>(disp);
      }
    }
  }
}

void SemiGlobalMatching::ComputeCensus(const cv::Mat& img,
                                       colmap::mvs::Mat<uint32_t>& census,
                                       const size_t window_size) {
  const size_t width = img.cols;
  const size_t height = img.rows;
  const int step = floor(window_size / 2);

  for (size_t i = step; i < height - step; i++) {
    for (size_t j = step; j < width - step; j++) {
      const uint8_t pixel = img.at<uint8_t>(i, j);
      uint32_t census_val = 0u;
      for (int r = -step; r <= step; r++) {
        for (int c = -step; c <= step; c++) {
          census_val <<= 1;
          if (img.at<uint8_t>(i + r, j + c) < pixel) {
            census_val += 1;
          }
        }
      }
      census.Set(i, j, census_val);
    }
  }
}

uint16_t SemiGlobalMatching::HammingDist(const uint32_t& x, const uint32_t& y) {
  uint32_t dist = 0;
  uint32_t val = x ^ y;

  while (val) {
    ++dist;
    val &= val - 1;
  }
  return dist;
}