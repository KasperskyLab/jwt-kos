/*
 * © 2024 AO Kaspersky Lab
 * Licensed under the MIT License
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <unistd.h>

int main(int argc, char** argv)
{
    sleep(10);
    ::testing::InitGoogleMock(&argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
