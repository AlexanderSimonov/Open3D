#pragma once

typedef unsigned int uint;

namespace open3d {
// 2D tensor, row major
typedef struct _Matrix3d {
    static const uint ROWS = 3;
    static const uint COLS = 3;

    double s[ROWS][COLS];

    double* operator[](const uint& i);
    const double* operator[](const uint& i) const;
} Matrix3d;
}  // namespace open3d