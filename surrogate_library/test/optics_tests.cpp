
// Copyright 2022, Jefferson Science Associates, LLC.
// Subject to the terms in the LICENSE file found in the top-level directory.


#include <catch.hpp>
#include "optics.h"

// class Profunctor (p :: * -> * -> *) where
//   dimap :: (a' -> a) -> (b -> b') -> p a b -> p a' b'

template <template <typename, typename> typename P, typename A, typename B, typename AA, typename BB>
P<AA, BB> dimap (std::function<A(AA)>, std::function<BB(B)>, P<A,B>);


TEST_CASE("Demonstrate two-way binding of a primitive") {

    int x = 22;

    // Write out x into the tensor at index [1,1]
    optics::Primitive<int> p;
    auto t = p.to(&x);

    float* tp = t[0].data_ptr<float>();
    REQUIRE(*tp == 22);

    // Modify the tensor
    *tp = 33;

    // Write back to primitive variable
    p.from(t, &x);
    REQUIRE(x == 33);
}

TEST_CASE("Pytorch simplest possible tensor") {
    float x = 22.2;
    auto t = torch::tensor(at::ArrayRef(&x, 1), torch::TensorOptions().dtype(torch::kFloat32));
    REQUIRE(t.size(0) == 1);
    REQUIRE(t.dim() == 1);
    auto tt = torch::tensor({22.2});
    float y = *(t[0].data_ptr<float>());
    REQUIRE(x == y);
}

TEST_CASE("PyTorch tensor operations") {

   /*
    float arr[9] = {1.2,2,3,4,5,6,7,8,9};
    std::vector<size_t> shape {3,3};
    std::vector<size_t> strides {1,3};

    auto t = torch::tensor({arr, 9});
    std::cout << t << std::endl;

    // auto t = torch::from_blob(arr);// , shape, strides).clone();

    */
}

struct MyStruct {
    float x;
    float y;
};

TEST_CASE("Composition of a Field lens with a Primitive") {
    MyStruct s { 49.0, 7.6};

    auto primitive_lens = optics::Primitive<float>();
    auto field_lens = optics::Field<MyStruct, float, optics::Primitive<float>>(primitive_lens, [](MyStruct* s){return &(s->y);});

    // Obviously we need to get template type deduction working... maybe just use a newer compiler?
    // auto field_lens = optics::Field(primitive_lens, [](MyStruct* s){return &(s->y);});

    // Should extract y and stick it in a tensor
    auto t = field_lens.to(&s);
    float* tp = t[0].data_ptr<float>();
    REQUIRE(*tp == (float) 7.6);

}