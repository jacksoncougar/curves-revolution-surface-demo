//
// Created by root on 2/12/19.
//

#include "hit.h"

hit intersect(ray ray, sphere sphere) {
    hit hit{};

    auto& [e, d] = ray;
    auto& [c, r] = sphere;

    const auto ce = e - c;
    const auto discriminant = d.dot(ce) * d.dot(ce) - d.dot(d) * (ce.dot(ce) - r * r);

    if (discriminant == 0) {
        const auto t0 = (-d.dot(ce) + sqrt(discriminant)) / d.dot(d);
        hit.did_intersect = true;
        hit.t = t0;
    }
    else if (discriminant > 0) {
        const auto t0 = (-d.dot(ce) + sqrt(discriminant)) / d.dot(d);
        const auto t1 = (-d.dot(ce) - sqrt(discriminant)) / d.dot(d);

        hit.did_intersect = true;
        hit.t = std::min(t0, t1);
    }

    return hit;
}
