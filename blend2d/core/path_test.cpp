// This file is part of Blend2D project <https://blend2d.com>
//
// See blend2d.h or LICENSE.md for license and copyright information
// SPDX-License-Identifier: Zlib

#include <blend2d/core/api-build_test_p.h>
#if defined(BL_TEST)

#include <blend2d/core/object_p.h>
#include <blend2d/core/path_p.h>

// bl::Path - Tests
// ================

namespace bl {
namespace Tests {

UNIT(path_stroke_degenerate_offset_leg, BL_TEST_GROUP_GEOMETRY_CONTAINERS) {
  // A quad of the cubic's approximation is split at its turning point, where the leg next to the split is a few ULPs
  // of rounding noise that points backwards. Offsetting with the direction of that leg used to put a control point
  // of the outline about 1e15 units away.
  INFO("Testing whether stroking a cubic with a cusp stays within the stroke width");

  BLPath path;
  path.move_to(1989.6699680076856, 1068.4518265722766);
  path.line_to(1990.2205198971101, 1068.4518265722766);
  path.cubic_to(1989.9452439523977, 1069.2776544064131,
                1990.2205198971101, 1068.8647404893447,
                1989.8076059800417, 1069.2776544064131);

  BLStrokeOptions options;
  options.width = 1.0;
  options.start_cap = BL_STROKE_CAP_ROUND;
  options.end_cap = BL_STROKE_CAP_ROUND;
  options.join = BL_STROKE_JOIN_ROUND;

  BLPath stroked;
  EXPECT_SUCCESS(stroked.add_stroked_path(path, options, bl_default_approximation_options));

  // Round caps and joins extend half of the width beyond the path; allow for the approximation tolerance.
  BLBox path_box, stroked_box;
  EXPECT_SUCCESS(path.get_bounding_box(&path_box));
  EXPECT_SUCCESS(stroked.get_bounding_box(&stroked_box));
  EXPECT_LE(path_box.x0 - stroked_box.x0, 0.6);
  EXPECT_LE(path_box.y0 - stroked_box.y0, 0.6);
  EXPECT_LE(stroked_box.x1 - path_box.x1, 0.6);
  EXPECT_LE(stroked_box.y1 - path_box.y1, 0.6);
}

UNIT(path_allocation_strategy, BL_TEST_GROUP_GEOMETRY_CONTAINERS) {
  BLPath p;
  size_t kNumItems = 1000000;
  size_t capacity = p.capacity();

  for (size_t i = 0; i < kNumItems; i++) {
    if (i == 0)
      p.move_to(0, 0);
    else
      p.move_to(double(i), double(i));

    if (capacity != p.capacity()) {
      size_t impl_size = PathInternal::impl_size_from_capacity(p.capacity()).value();
      INFO("Capacity increased from %zu to %zu [ImplSize=%zu]\n", capacity, p.capacity(), impl_size);

      capacity = p.capacity();
    }
  }
}

} // {Tests}
} // {bl}

#endif // BL_TEST
