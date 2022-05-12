#include <iostream>

#include <boost/program_options.hpp>

#include <opencv2/opencv.hpp>

#include "semi_global_matching.h"

std::string FLAGS_image_path_left;
std::string FLAGS_image_path_right;

bool Initialize(int argc, char* argv[]) {
  namespace config = boost::program_options;
  config::options_description desc;
  desc.add_options()("help, h", "Produce help message")(
      "image_path_left", config::value<std::string>(&FLAGS_image_path_left),
      "Image path left")("image_path_right",
                         config::value<std::string>(&FLAGS_image_path_right),
                         "Image path right");

  config::variables_map vmap;
  config::store(config::parse_command_line(argc, argv, desc), vmap);
  config::notify(vmap);

  if (vmap.count("help")) {
    std::cout << desc << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
  if (Initialize(argc, argv) == EXIT_FAILURE) {
    return EXIT_FAILURE;
  }

  cv::Mat img_left = cv::imread(FLAGS_image_path_left);
  cv::Mat img_right = cv::imread(FLAGS_image_path_right);

  cv::imshow("Left", img_left);
  cv::imshow("Right", img_right);
  cv::waitKey(0);

  return EXIT_SUCCESS;
}