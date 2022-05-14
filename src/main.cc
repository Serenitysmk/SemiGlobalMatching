#include <iostream>

#include <boost/program_options.hpp>

#include <chrono>

#include "semi_global_matching.h"

std::string FLAGS_image_path_left;
std::string FLAGS_image_path_right;
std::string FLAGS_output_path;

bool Initialize(int argc, char* argv[]) {
  namespace config = boost::program_options;
  config::options_description desc;
  desc.add_options()("help, h", "Produce help message")(
      "image_path_left", config::value<std::string>(&FLAGS_image_path_left),
      "Image path left")("image_path_right",
                         config::value<std::string>(&FLAGS_image_path_right),
                         "Image path right")(
      "output_path", config::value<std::string>(&FLAGS_output_path),
      "Output path");

  config::variables_map vmap;
  config::store(config::parse_command_line(argc, argv, desc), vmap);
  config::notify(vmap);

  if (vmap.count("help")) {
    std::cout << desc << std::endl;
    return EXIT_FAILURE;
  }
  if (FLAGS_image_path_left == "" || FLAGS_image_path_right == "") {
    std::cerr << "ERROR: Image path is empty" << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
  if (Initialize(argc, argv) == EXIT_FAILURE) {
    return EXIT_FAILURE;
  }

  cv::Mat img_left = cv::imread(FLAGS_image_path_left, cv::IMREAD_GRAYSCALE);
  cv::Mat img_right = cv::imread(FLAGS_image_path_right, cv::IMREAD_GRAYSCALE);

  CHECK(!img_left.empty());
  CHECK(!img_right.empty());

  const size_t width = img_left.cols;
  const size_t height = img_left.rows;

  cv::imshow("Left", img_left);
  cv::imshow("Right", img_right);

  SemiGlobalMatching::Options options;
  options.max_disparity = 64;
  SemiGlobalMatching sgm(options, width, height);

  cv::Mat disparity_map;
  sgm.Match(img_left, img_right, disparity_map);

  cv::imshow("Disparity map", disparity_map);
  cv::imwrite(FLAGS_output_path, disparity_map);
  cv::waitKey(0);
  return EXIT_SUCCESS;
}