#include "semi_global_matching.h"

SemiGlobalMatching::SemiGlobalMatching(
    const SemiGlobalMatching::Options& options, const size_t width,
    const size_t height)
    : options_(options), width_(width), height_(height) {}