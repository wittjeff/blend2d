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

UNIT(path_stroke_subpixel_cubic, BL_TEST_GROUP_GEOMETRY_CONTAINERS) {
  // The cubic is approximated by quads, one of which is nearly collinear and turns back on itself. After a split at
  // its turning point, the derivative of the rest of the quad is only rounding noise, and the offsetting loop used to
  // split the same curve forever (#269).
  INFO("Testing whether stroking a sub-pixel cubic terminates");

  BLPath path;
  path.move_to(623.61953000604717, 711.94969687666287);
  path.line_to(624.3877062003628, 712.71734821265682);
  path.cubic_to(624.96383834609946, 712.52543537865824,
                624.57975024894165, 712.71734821265682,
                624.96383834609946, 712.33352254465979);

  BLStrokeOptions options;
  options.width = 1.0;
  options.start_cap = BL_STROKE_CAP_ROUND;
  options.end_cap = BL_STROKE_CAP_ROUND;
  options.join = BL_STROKE_JOIN_ROUND;

  BLPath stroked;
  EXPECT_SUCCESS(stroked.add_stroked_path(path, options, bl_default_approximation_options));

  BLBox box;
  EXPECT_SUCCESS(stroked.get_bounding_box(&box));
  EXPECT_GT(box.x1, box.x0);
  EXPECT_GT(box.y1, box.y0);
}

} // {Tests}
} // {bl}

#endif // BL_TEST
