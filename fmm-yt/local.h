#include <glm/glm.hpp>

#include "constants.h"
#include "multipole.h"

struct local {
    glm::dvec2 center;
    double lx[6] = {};
    double ly[6] = {};

	local(glm::dvec2 center) : center(center) {}

	void add(multipole& m)
	{
        auto r = center - m.center;

        double a = r.x;
        double b = r.y;

        double d = std::sqrt(a * a + b * b);
        double d3 = d * d * d;
        double d5 = d3 * d * d;
        double d7 = d5 * d * d;
        double d9 = d7 * d * d;
        double d11 = d9 * d * d;

        double a2 = a * a, b2 = b * b;
        double a3 = a2 * a, a2b = a2 * b, ab2 = a * b2, b3 = b2 * b;
        double a4 = a3 * a, a3b = a3 * b, a2b2 = a2 * b2, ab3 = a * b3, b4 = b3 * b;
        double a5 = a4 * a, a4b = a4 * b, a3b2 = a3 * b2, a2b3 = a2 * b3, ab4 = a * b4, b5 = b4 * b;

        double derivatives[6][6] = {};
        derivatives[0][0] = -a / d3;
        derivatives[0][1] = (2 * a2 - b2) / d5;
        derivatives[0][2] = (3 * a * b) / d5;
        derivatives[0][3] = (9 * (ab2 - 2 * a3)) / d7;
        derivatives[0][4] = (3 * (b3 - 4 * a2b)) / d7;
        derivatives[0][5] = (3 * (a3 - 4 * ab2)) / d7;

        derivatives[1][0] = (b2 - 2 * a2) / d5;
        derivatives[1][1] = (6 * a3 - 9 * ab2) / d7;
        derivatives[1][2] = (12 * a2b- 3 * b3) / d7;
        derivatives[1][3] = (-24 * a4 + 72 * a2b2 - 9 * b4) / d9;
        derivatives[1][4] = (45 * ab3 - 60 * a3b) / d9;
        derivatives[1][5] = (12 * a4 - 81 * a2b2 + 12 * b4) / d9;

        derivatives[2][0] = -3 * a * b / d5;
        derivatives[2][1] = (12 * a2b - 3 * b3) / d7;
        derivatives[2][2] = (12 * ab2 - 3 * a3) / d7;
        derivatives[2][3] = (45 * ab3 - 60 * a3b) / d9;
        derivatives[2][4] = (12 * a4 - 81 * a2b2 + 12 * b4) / d9;
        derivatives[2][5] = (45 * a3b - 60 * ab3) / d9;

        derivatives[3][0] = (9 * ab2 - 6 * a3) / d7;
        derivatives[3][1] = (24 * a4 - 72 * a2b2 + 9 * b4) / d9;
        derivatives[3][2] = (60 * a3b - 45 * ab3) / d9;
        derivatives[3][3] = -15 * (8 * a5 - 40 * a3b2 + 15 * ab4) / d11;
        derivatives[3][4] = -45 * (8 * a4b - 12 * a2b3 + b5) / d11;
        derivatives[3][5] = 15 * (4 * a5 - 41 * a3b2 + 18 * ab4) / d11;

        derivatives[4][0] = (3 * b2 - 12 * a2b) / d7;
        derivatives[4][1] = 3 * (20 * a3b - 7 * ab2 - 8 * ab3) / d9;
        derivatives[4][2] = (-12 * a4 - 15 * b3 + 6 * a2b *(1 + 12 * b)) / d9;
        derivatives[4][3] = -3 * b * (120 * a4 + b3 * (7 + 8 * b) - 4 * a2b * (14 + 31 * b)) / d11;
        derivatives[4][4] = (60 * a5 + 3 * ab3 * (49 + 48 * b) - 6 * a3b * (7 + 92 * b)) / d11;
        derivatives[4][5] = (90 * b4 + 6 * a4 * (1 + 42 * b) - 3 * a2b2 * (31 + 168 * b)) / d11;

        derivatives[5][0] = (3 * a3 - 12 * ab2) / d7;
        derivatives[5][1] = -3 * (4 * a4 - 27 * a2b2 + 4 * b4) / d9;
        derivatives[5][2] = 15 * (-3 * a3b + 4 * ab3) / d9;
        derivatives[5][3] = 15 * (4 * a5 - 41 * a3b2 + 18 * ab4) / d11;
        derivatives[5][4] = 15 * (18 * a4b - 41 * a2b3 + 4 * b5) / d11;
        derivatives[5][5] = -45 * (a5 - 12 * a3b2 + 8 * ab4) / d11;

        for (int i = 0; i < 6; i++)
            for (int j = 0; j < 6; j++)
                lx[i] += derivatives[i][j] * m.values[j];

        derivatives[0][0] = -b / d3;
        derivatives[0][1] = 3 * a * b / d5;
        derivatives[0][2] = (2 * b2 - a2) / d5;
        derivatives[0][3] = (3 * b3 - 12 * a2b) / d7;
        derivatives[0][4] = (3 * a3 - 12 * ab2) / d7;
        derivatives[0][5] = (9 * a2b - 6 * b3) / d7;

        derivatives[1][0] = -3 * a * b / d5;
        derivatives[1][1] = (12 * a2b - 3 * b3) / d7;
        derivatives[1][2] = (12 * ab2 - 3 * a3) / d7;
        derivatives[1][3] = (45 * ab3 - 60 * a3b) / d9;
        derivatives[1][4] = (12 * a4 - 81 * a2b2 + 12 * b4) / d9;
        derivatives[1][5] = (45 * a3b - 60 * ab3) / d9;

        derivatives[2][0] = (a2 - 2 * b2) / d5;
        derivatives[2][1] = (12 * ab2 - 3 * a3) / d7;
        derivatives[2][2] = (6 * b3 - 9 * a2b) / d7;
        derivatives[2][3] = (12 * a4 - 81 * a2b2 + 12 * b4) / d9;
        derivatives[2][4] = (45 * a3b - 60 * ab3) / d9;
        derivatives[2][5] = -3 * (3 * a4 - 24 * a2b2 + 8 * b4) / d9;

        derivatives[3][0] = (3 * b3 - 12 * ab2) / d7;
        derivatives[3][1] = (72 * a2b2 - 21 * ab3 - 12 * b4) / d9;
        derivatives[3][2] = (-24 * a3b + 3 * a2b2 + 60 * ab3 - 12 * b4) / d9;
        derivatives[3][3] = -21 * (24 * a3b2 - 8 * a2b3 - 12 * a4 + b5) / d11;
        derivatives[3][4] = 3 * (48 * a4b - 21 * a3b2 - 184 * a2b3 + 42 *ab4 + 20 * b5) / d11;
        derivatives[3][5] = 3 * (-8 * a5 + 6 *a4b + 124 * a3b2 - 37 * a2b3 - 120 * ab4 + 20 * b5) / d11;

        derivatives[4][0] = (3 * a3 - 12 * ab2) / d7;
        derivatives[4][1] = -3 * (4 * a4 - 27 * a2b2 + 4 * b4) / d9;
        derivatives[4][2] = 15 * (-3 * a3b + 4 * ab3) / d9;
        derivatives[4][3] = 15 * (4 * a5 - 41 * a3b2 + 18 * ab4) / d11;
        derivatives[4][4] = 15 * (18 * a4b - 41 * a2b3 + 4 * b5) / d11;
        derivatives[4][5] = -45 * (a5 - 12 * a3b2 + 8 * ab4) / d11;

        derivatives[5][0] = (9 * a2b - 6 * b3) / d7;
        derivatives[5][1] = (-45 * a3b + 60 * ab3) / d9;
        derivatives[5][2] = (9 * a4 - 72 * a2b2 + 24 * b4) / d9;
        derivatives[5][3] = 15 * (18 * a4b - 41 * a2b3 + 4 * b5) / d11;
        derivatives[5][4] = -45 * (a5 - 12 * a3b2 + 8 * ab4) / d11;
        derivatives[5][5] = -15 * (15 * a4b - 40 * a2b3 + 8 * b5) / d11;

        for (int i = 0; i < 6; i++)
            for (int j = 0; j < 6; j++)
                ly[i] += derivatives[i][j] * m.values[j];
	}

    glm::dvec2 calc(glm::dvec2 point, double m)
    {
        glm::dvec2 result = { 0.0, 0.0 };
        result.x += lx[0];
        result.x += lx[1] * (point.x - center.x);
        result.x += lx[2] * (point.y - center.y);
        result.x += lx[3] * 0.5 * (point.x - center.x) * (point.x - center.x);
        result.x += lx[4] * (point.x - center.x) * (point.y - center.y);
        result.x += lx[5] * 0.5 * (point.y - center.y) * (point.y - center.y);

        result.y += ly[0];
        result.y += ly[1] * (point.x - center.x);
        result.y += ly[2] * (point.y - center.y);
        result.y += ly[3] * 0.5 * (point.x - center.x) * (point.x - center.x);
        result.y += ly[4] * (point.x - center.x) * (point.y - center.y);
        result.y += ly[5] * 0.5 * (point.y - center.y) * (point.y - center.y);

        return G * m * result;
    }
};