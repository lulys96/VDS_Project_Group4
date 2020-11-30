//
// Created by tobias on 21.11.16.
//

#ifndef VDS_PROJECT_GTESTS_H
#define VDS_PROJECT_GTESTS_H

#include "gtest/gtest.h"
#include "../Manager.h"
#include "../ManagerInterface.h"

using namespace ClassProject;
class ManagerTest : public ::testing::Test {
protected:
    Manager *my_manager = NULL;
};

TEST_F(ManagerTest, CtorTableSize) {
    my_manager = new Manager();
    ASSERT_TRUE(my_manager->uni_table.size()==2);
}


#endif //VDS_PROJECT_TESTS_H

