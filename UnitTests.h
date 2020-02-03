//
// Created by jimbert on 2/3/20.
//

#ifndef COMMAND_LINE_PARSER_C__UNITTESTS_H
#define COMMAND_LINE_PARSER_C__UNITTESTS_H

#include <gtest/gtest.h>

#include "CommandLineParser.h"
#include "CLUtils.h"
#include "CLParser.h"
#include <string>

using namespace std;

TEST(CLArgBase, operatorsEqual) {
  CLArg<string> o1 =
      CLArg<string>::Builder{}.setCltype(CLTYPE::STRING).setOpt("-a").setLongOpt("--aaa").isMandatory().setDoc("Documentation of option a").build();
  CLArg<string> o2 = CLArg<string>::Builder{}.setCltype(CLTYPE::STRING).setOpt("-a").setLongOpt("--aaa").build();
  EXPECT_EQ(true, o1 == o2);
  EXPECT_EQ(false, o1 != o2);
  // Same LongOption but different Option
  CLArg<string> o3 = CLArg<string>::Builder{}.setCltype(CLTYPE::STRING).setOpt("b").setLongOpt("--aaa").build();
  EXPECT_EQ(true, o1 != o3);
  EXPECT_EQ(false, o1 == o3);
  // Same Option but different LongOption
  CLArg<string> o4 = CLArg<string>::Builder{}.setCltype(CLTYPE::STRING).setOpt("-a").setLongOpt("--aa").build();
  EXPECT_EQ(true, o1 != o4);
  EXPECT_EQ(false, o1 == o4);
  // Different Option and different LongOption
  EXPECT_EQ(true, o3 != o4);
  EXPECT_EQ(false, o3 == o4);
  // Same Option but NULL LongOption
  CLArg<string> o5 = CLArg<string>::Builder{}.setCltype(CLTYPE::STRING).setOpt("-a").build();
  EXPECT_EQ(true, o1 != o5);
  EXPECT_EQ(false, o1 == o5);
  // Same LongOption but different Option
  CLArg<string> o6 = CLArg<string>::Builder{}.setCltype(CLTYPE::STRING).setLongOpt("--aaa").build();
  EXPECT_EQ(true, o1 != o6);
  EXPECT_EQ(false, o1 == o6);
}

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
  EXPECT_EQ(false, clBuild.isPresent());
}

TEST(builderTest, defaults) {
  CLArg<string> clBuild = CLArg<string>::Builder{}.build();
  EXPECT_EQ("", clBuild.getOption());
  EXPECT_EQ("", clBuild.getLongOption());
  EXPECT_EQ("", clBuild.getDoc());
  EXPECT_EQ(CLTYPE::NONE, clBuild.getType());
  EXPECT_EQ(false, clBuild.isMandatory());
  EXPECT_EQ(false, clBuild.isPresent());
}

TEST(CLParser, addSafe) {
  CLParser parser;
  CLArg<string> arg1 = CLArg<string>::Builder{}.setOpt("-a").setLongOpt("--aaa").build();
  bool ok1 = parser.addSafe(arg1);
  EXPECT_TRUE(ok1);
  EXPECT_EQ(1, parser.getOptions().size());

  CLArg<string> arg2 = CLArg<string>::Builder{}.setOpt("-b").setLongOpt("--bbb").build();
  bool ok2 = parser.addSafe(arg2);
  EXPECT_TRUE(ok2);
  EXPECT_EQ(2, parser.getOptions().size());

  CLArg<string> arg3 = CLArg<string>::Builder{}.setOpt("-a").setLongOpt("--aaa").build();
  bool ok3 = parser.addSafe(arg3);
  EXPECT_FALSE(ok3);
  EXPECT_EQ(2, parser.getOptions().size());
}

TEST(CLParser, add) {
  CLParser parser;
  CLArg<string> arg1 = CLArg<string>::Builder{}.setOpt("-a").setLongOpt("--aaa").build();
  CLArg<string> arg2 = CLArg<string>::Builder{}.setOpt("-b").setLongOpt("--bbb").build();
  CLArg<string> arg3 = CLArg<string>::Builder{}.setOpt("-a").setLongOpt("--aaa").build();
  parser.add(arg1).add(arg2).add(arg3);
  EXPECT_EQ(2, parser.getOptions().size());
}

#endif //COMMAND_LINE_PARSER_C__UNITTESTS_H
