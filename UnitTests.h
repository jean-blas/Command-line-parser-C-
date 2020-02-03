//
// Created by jimbert on 2/3/20.
//

#ifndef COMMAND_LINE_PARSER_C__UNITTESTS_H
#define COMMAND_LINE_PARSER_C__UNITTESTS_H

#include <gtest/gtest.h>

#include "CommandLineParser.h"
#include "CLUtils.h"
#include <string>

using namespace std;

TEST(UtilsTest, split) {
  std::vector<std::string> v = split("a -bb ++ccc ", ' ');
  ASSERT_EQ(3, v.size());
  EXPECT_EQ("a", v[0]);
  EXPECT_EQ("-bb", v[1]);
  EXPECT_EQ("++ccc", v[2]);
}

TEST(builderTest, simple) {
  CLArg<string> clBuild =
      CLArg<string>::Builder{}.setCltype(CLTYPE::STRING).setOpt("-a").setLongOpt("--aaa").isMandatory().setDoc("Documentation of option a").build();
  EXPECT_EQ("-a", clBuild.getOption());
  EXPECT_EQ("--aaa", clBuild.getLongOption());
  EXPECT_EQ("Documentation of option a", clBuild.getDoc());
  EXPECT_EQ(CLTYPE::STRING, clBuild.getType());
  EXPECT_EQ(true, clBuild.isMandatory());
}

TEST(builderTest, defaults) {
  CLArg<string> clBuild = CLArg<string>::Builder{}.build();
  EXPECT_EQ("", clBuild.getOption());
  EXPECT_EQ("", clBuild.getLongOption());
  EXPECT_EQ("", clBuild.getDoc());
  EXPECT_EQ(CLTYPE::NONE, clBuild.getType());
  EXPECT_EQ(false, clBuild.isMandatory());
}

#endif //COMMAND_LINE_PARSER_C__UNITTESTS_H
