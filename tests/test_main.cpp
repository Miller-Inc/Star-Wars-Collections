//
// Created by James Miller on 10/12/2025.
//
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <cstring>
#include <cstddef>
#include <string>
#include "doctest.h"
#include "EngineTypes/InputValidation.h"
#include "EngineTypes/Vector.h"
#include "EngineTypes/Array.h"
#include "EngineTypes/String.h"
#include "EngineTypes/Map.h"
#include "EngineTypes/Image.h"
#include "Network/NetworkManager.h"
#include "Game/WindowTypes.h"

using namespace MillerInc;
using namespace MillerInc::Game;

TEST_CASE("MVector2 basic operations") {
    MVector2 a{3.0f, 4.0f};
    MVector2 b{1.0f, 2.0f};

    CHECK((a.Length() == doctest::Approx(5.0f)));
    CHECK(((a + b).x == 4.0f));
    CHECK(((a + b).y == 6.0f));
    CHECK(((a - b).x == 2.0f));
    CHECK(((a * 2.0f).x == 6.0f));
    CHECK(((a / 2.0f).x == 1.5f));
}

TEST_CASE("MArray basic usage") {
    MArray<int> arr;
    arr.Add(1);
    arr.Add(2);
    arr.Add(3);

    CHECK((arr.Len() == 3));
    CHECK((arr[0] == 1));
    CHECK((arr[1] == 2));
    CHECK((arr[2] == 3));

    arr.RemoveAt(1);
    CHECK((arr.Len() == 2));
    CHECK((arr[1] == 3));
}

TEST_CASE("MString alias and MMap usage") {
    MString s = "hello";
    CHECK((s == "hello"));

    MMap<MString, int> m;
    m["a"] = 1;
    CHECK((m["a"] == 1));
}

TEST_CASE("InputValidation::IsValidIP") {
    using namespace MillerInc::InputValidation;
    CHECK(IsValidIP("127.0.0.1"));
    CHECK(IsValidIP("192.168.1.1"));
    CHECK_FALSE(IsValidIP("999.999.999.999"));
    CHECK_FALSE(IsValidIP("not.an.ip"));
}

TEST_CASE("WindowType enum values") {
    using namespace MillerInc::Game;
    CHECK((static_cast<int>(WindowType::OpeningWindow) == 0));
    CHECK((static_cast<int>(WindowType::NetworkJoinSetupWindow) == 7));
}

TEST_CASE("MImage struct default values") {
    MImage img{};
    CHECK((img.ref_count == 0));
    CHECK((img.Scale.x == 1.0f));
    CHECK((img.Scale.y == 1.0f));
}