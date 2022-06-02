
// Copyright 2022, Jefferson Science Associates, LLC.
// Subject to the terms in the LICENSE file found in the top-level directory.

#include <catch.hpp>
#include "feedforward_model.h"

#include "surrogate.h"

using namespace phasm;
namespace phasm::test::pytorch_model_tests {

double square(double x) {
    return x*x;
}

TEST_CASE("Can we build against pytorch at all?") {

    auto m = std::make_shared<FeedForwardModel>();
    m->add_input<double>("x", new Primitive<double>(), "x");
    m->add_output<double>("y");

    double x, y;
    auto s = Surrogate([&]() { y = square(x); }, m);
    s.bind("x", &x);
    s.bind("y", &y);

    for (int i = -3; i < 3; ++i) {
        // x =
        // s.call_original_and_capture();

    }
}
} // namespace phasm::test::pytorch_model_tests