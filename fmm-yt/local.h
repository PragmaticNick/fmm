#pragma once

#include <glm/glm.hpp>

#include "constants.h"
#include "multipole.h"

struct local {
    glm::dvec2 center;
    double lx[6] = {};
    double ly[6] = {};

	local(glm::dvec2 center) : center(center) {}

	void add(multipole* m)
	{
        auto r = center - m->center;

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
        double a10 = a5 * a5, a8 = a4 * a4, a6 = a3 * a3;
        double b6 = b3 * b3, b8 = b4 * b4, b10 = b5 * b5;

        double derivatives[6][6] = {};

        derivatives[0][0] = -a / d3;
        derivatives[0][1] = (2 * a2 - b2) / d5;
        derivatives[0][2] = (3 * a * b) / d5;
        derivatives[0][3] = 3 * a * (-2 * a2 + 3 * b2) / d7;
        derivatives[0][4] = 3 * b * (-4 * a2 + b2) / d7;
        derivatives[0][5] = 3 * a * (a2 - 4 * b2) / d7;

        derivatives[1][0] = (-2 * a2 + b2) / d5;
        derivatives[1][1] = 3 * a * (2 * a2 - 3 * b2) / d7;
        derivatives[1][2] = 3 * b * (4 * a2 - b2) / d7;
        derivatives[1][3] = 3 * (7 * a2 * (-2 * a2 + 3 * b2) + 3 * (a2 + b2) * (2 * a2 - b2)) / d9;
        derivatives[1][4] = 15 * a * b * (-4 * a2 + 3 * b2) / d9;
        derivatives[1][5] = 3 * (7 * b2 * (-4 * a2 + b2) + (a2 + b2) * (4 * a2 - 3 * b2)) / d9;

        derivatives[2][0] = -3 * a * b / d5;
        derivatives[2][1] = 3 * b * (4 * a2 - b2) / d7;
        derivatives[2][2] = 3 * a * (-a2 + 4 * b2) / d7;
        derivatives[2][3] = 15 * a * b * (-4 * a2 + 3 * b2) / d9;
        derivatives[2][4] = 3 * (7 * b2 * (-4 * a2 + b2) + (a2 + b2) * (4 * a2 - 3 * b2)) / d9;
        derivatives[2][5] = 15 * a * b * (3 * a2 - 4 * b2) / d9;

        derivatives[3][0] = (-6 * a3 + 9 * a * b2) / d7;
        derivatives[3][1] = 3 * (7 * a2 * (2 * a2 - 3 * b2) + 3 * (-2 * a2 + b2) * (a2 + b2)) / d9;
        derivatives[3][2] = 15 * a * b * (4 * a2 - 3 * b2) / d9;
        derivatives[3][3] = -120 * a5 / d11 + 600 * a3 * b2 / d11 - 225 * a * b4 / d11;
        derivatives[3][4] = -360 * a4 * b / d11 + 540 * a2 * b3 / d11 - 45 * b5 / d11;
        derivatives[3][5] = 15 * a * (b2 * (-36 * a2 + 27 * b2) + (a2 + b2) * (4 * a2 - 9 * b2)) / d11;

        derivatives[4][0] = (-12 * a2 * b + 3 * b2) / d7;
        derivatives[4][1] = 3 * a * b * (20 * a2 - 8 * b2 - 7 * b) / d9;
        derivatives[4][2] = 3 * (7 * b2 * (4 * a2 - b) + 2 * (-2 * a2 + b) * (a2 + b2)) / d9;
        derivatives[4][3] = 3 * b * (a2 * (-180 * a2 + 72 * b2 + 63 * b) + (a2 + b2) * (60 * a2 - 8 * b2 - 7 * b)) / d11;
        derivatives[4][4] = 3 * a * (b2 * (-180 * a2 + 72 * b2 + 63 * b) + (a2 + b2) * (20 * a2 - 8 * b2 - b * (16 * b + 7) - 7 * b)) / d11;
        derivatives[4][5] = 3 * (-9 * b * (7 * b2 * (4 * a2 - b) - 2 * (a2 + b2) * (2 * a2 - b)) + (a2 + b2) * (2 * a2 - 5 * b2 - 4 * b * (2 * a2 - b) + 14 * b * (4 * a2 - b))) / d11;

        derivatives[5][0] = (3 * a3 - 12 * a * b2) / d7;
        derivatives[5][1] = 3 * (-7 * a2 * (a2 - 4 * b2) + (a2 + b2) * (3 * a2 - 4 * b2)) / d9;
        derivatives[5][2] = 15 * a * b * (-3 * a2 + 4 * b2) / d9;
        derivatives[5][3] = 60 * a5 / d11 - 615 * a3 * b2 / d11 + 270 * a * b4 / d11;
        derivatives[5][4] = 270 * a4 * b / d11 - 615 * a2 * b3 / d11 + 60 * b5 / d11;
        derivatives[5][5] = 45 * a * (b2 * (9 * a2 - 12 * b2) + (-a2 + 4 * b2) * (a2 + b2)) / d11;

        for (int i = 0; i < 6; i++)
            for (int j = 0; j < 6; j++)
                lx[i] += derivatives[j][i] * m->values[j];

        derivatives[0][0] = -b / d3;
        derivatives[0][1] = 3 * a * b / d5;
        derivatives[0][2] = (-a2 + 2 * b2) / d5;
        derivatives[0][3] = 3 * b * (-4 * a2 + b2) / d7;
        derivatives[0][4] = 3 * a * (a2 - 4 * b2) / d7;
        derivatives[0][5] = 3 * b * (3 * a2 - 2 * b2) / d7;

        derivatives[1][0] = -3 * a * b / d5;
        derivatives[1][1] = 3 * b * (4 * a2 - b2) / d7;
        derivatives[1][2] = 3 * a * (-a2 + 4 * b2) / d7;
        derivatives[1][3] = 15 * a * b * (-4 * a2 + 3 * b2) / d9;
        derivatives[1][4] = 3 * (7 * b2 * (-4 * a2 + b2) + (a2 + b2) * (4 * a2 - 3 * b2)) / d9;
        derivatives[1][5] = 15 * a * b * (3 * a2 - 4 * b2) / d9;

        derivatives[2][0] = (a2 - 2 * b2) / d5;
        derivatives[2][1] = 3 * a * (-a2 + 4 * b2) / d7;
        derivatives[2][2] = 3 * b * (-3 * a2 + 2 * b2) / d7;
        derivatives[2][3] = 3 * (7 * a2 * (a2 - 4 * b2) + (-3 * a2 + 4 * b2) * (a2 + b2)) / d9;
        derivatives[2][4] = 15 * a * b * (3 * a2 - 4 * b2) / d9;
        derivatives[2][5] = 3 * (7 * b2 * (3 * a2 - 2 * b2) + 3 * (-a2 + 2 * b2) * (a2 + b2)) / d9;

        derivatives[3][0] = (-12 * a * b2 + 3 * b3) / d7;
        derivatives[3][1] = b2 * (-12 * a2 + 21 * a * (4 * a - b) - 12 * b2) / d9;
        derivatives[3][2] = 3 * b * (7 * b2 * (4 * a - b) + (-8 * a + 3 * b) * (a2 + b2)) / d9;
        derivatives[3][3] = 3 * b2 * (9 * a * (4 * a2 - 7 * a * (4 * a - b) + 4 * b2) + (48 * a - 7 * b) * (a2 + b2)) / d11;
        derivatives[3][4] = 3 * b * (b2 * (36 * a2 - 63 * a * (4 * a - b) + 36 * b2) + (a2 + b2) * (-8 * a2 + 14 * a * (4 * a - b) - 8 * b2 + b * (-7 * a - 8 * b))) / d11;
        derivatives[3][5] = 3 * (-8 * a5 + 6 * a4 * b + 124 * a3 * b2 - 37 * a2 * b3 - 120 * a * b4 + 20 * b5) / (sqrt(a2 + b2) * (a10 + 5 * a8 * b2 + 10 * a6 * b4 + 10 * a4 * b6 + 5 * a2 * b8 + b10));

        derivatives[4][0] = (3 * a3 - 12 * a * b2) / d7;
        derivatives[4][1] = 3 * (-7 * a2 * (a2 - 4 * b2) + (a2 + b2) * (3 * a2 - 4 * b2)) / d9;
        derivatives[4][2] = 15 * a * b * (-3 * a2 + 4 * b2) / d9;
        derivatives[4][3] = 60 * a5 / d11 - 615 * a3 * b2 / d11 + 270 * a * b4 / d11;
        derivatives[4][4] = 270 * a4 * b / d11 - 615 * a2 * b3 / d11 + 60 * b5 / d11;
        derivatives[4][5] = 45 * a * (b2 * (9 * a2 - 12 * b2) + (-a2 + 4 * b2) * (a2 + b2)) / d11;

        derivatives[5][0] = (9 * a2 * b - 6 * b3) / d7;
        derivatives[5][1] = 15 * a * b * (-3 * a2 + 4 * b2) / d9;
        derivatives[5][2] = 3 * (-7 * b2 * (3 * a2 - 2 * b2) + 3 * (a2 - 2 * b2) * (a2 + b2)) / d9;
        derivatives[5][3] = 15 * b * (a2 * (27 * a2 - 36 * b2) + (-9 * a2 + 4 * b2) * (a2 + b2)) / d11;
        derivatives[5][4] = 45 * a * (b2 * (9 * a2 - 12 * b2) + (-a2 + 4 * b2) * (a2 + b2)) / d11;
        derivatives[5][5] = -225 * a4 * b / d11 + 600 * a2 * b3 / d11 - 120 * b5 / d11;

        for (int i = 0; i < 6; i++)
            for (int j = 0; j < 6; j++)
                ly[i] += derivatives[j][i] * m->values[j];
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

    local* translate(glm::dvec2 pos)
    {
        glm::vec2 r = pos - center;
        double da = r.x;
        double db = r.y;
        
        local* l = new local(pos);

        l->lx[0] = lx[0] + lx[1] * da + lx[2] * db + lx[3] * da * da + lx[4] * da * db + lx[5] * db * db;
        l->lx[1] = lx[1] + 2 * lx[3] * da + lx[4] * db;
        l->lx[2] = lx[2] + 2 * lx[4] * da + 2 * lx[5] * db;
        l->lx[3] = lx[3];
        l->lx[4] = lx[4];
        l->lx[5] = lx[5];

        l->ly[0] = ly[0] + ly[1] * da + ly[2] * db + ly[3] * da * da + ly[4] * da * db + ly[5] * db * db;
        l->ly[1] = ly[1] + 2 * ly[3] * da + ly[4] * db;
        l->ly[2] = ly[2] + 2 * ly[4] * da + 2 * ly[5] * db;
        l->ly[3] = ly[3];
        l->ly[4] = ly[4];
        l->ly[5] = ly[5];

        return l;
    }

    void add(local* other)
    {
        if (other->center != center)
            other = other->translate(center);

        for (int i = 0; i < 6; i++)
        {
            lx[i] += other->lx[i];
            ly[i] += other->ly[i];
        }
    }
};