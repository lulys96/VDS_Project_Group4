//
// Created by tobias on 21.11.16.
//

#ifndef VDS_PROJECT_GTESTS_H
#define VDS_PROJECT_GTESTS_H

#include "gtest/gtest.h"
#include "../Manager.h"
#include "../ManagerInterface.h"

using namespace ClassProject;
class ManagerTest : public ::testing::Test{
protected:
    ManagerInterface *my_manager = NULL;
};

TEST_F(ManagerTest, ManagerInitTest) {
    my_manager = new Manager();
    ASSERT_TRUE (my_manager != NULL);
}

#endif //VDS_PROJECT_TESTS_H

