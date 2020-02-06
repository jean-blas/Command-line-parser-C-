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

TEST(UtilsTest, contains) {
  std::vector<std::string> v = split("a -bb ++ccc ", ' ');
  EXPECT_TRUE(contains<string>("a", v));
  EXPECT_FALSE(contains<string>("b", v));

  CLArg<string> arg1 = CLArg<string>::Builder{}.setOpt("-a").setLongOpt("--aaa").build();
  CLArg<string> arg2 = CLArg<string>::Builder{}.setOpt("-b").setLongOpt("--bbb").build();
  CLArg<string> arg3 = CLArg<string>::Builder{}.setOpt("-c").setLongOpt("--ccc").build();
  std::vector<CLArgBase> v1{arg1, arg2, arg3};
  EXPECT_TRUE(contains<CLArgBase>(arg1, v1));
  CLArg<string> arg4 = CLArg<string>::Builder{}.setOpt("-d").setLongOpt("--ddd").build();
  EXPECT_FALSE(contains<CLArgBase>(arg4, v1));
}

TEST(UtilsTest, locate) {
  CLArg<string> arg1 = CLArg<string>::Builder{}.setOpt("-a").setLongOpt("--aaa").build();
  CLArg<string> arg2 = CLArg<string>::Builder{}.setOpt("-b").setLongOpt("--bbb").build();
  CLArg<string> arg3 = CLArg<string>::Builder{}.setOpt("-c").setLongOpt("--ccc").build();
  std::vector<CLArgBase> v{arg1, arg2, arg3};
  // Test the short option field
  auto loc = locate<string, CLArgBase>("-b", v, [](const CLArgBase& arg) { return arg.getOption(); });
  ASSERT_TRUE(loc != v.cend());
  EXPECT_EQ(arg2, *loc);
  loc = locate<string, CLArgBase>("-e", v, [](const CLArgBase& arg) { return arg.getOption(); });
  EXPECT_TRUE(loc == v.cend());
  // Test the long option field
  loc = locate<string, CLArgBase>("--bbb", v, [](const CLArgBase& arg) { return arg.getLongOption(); });
  ASSERT_TRUE(loc != v.cend());
  EXPECT_EQ(arg2, *loc);
  loc = locate<string, CLArgBase>("--eee", v, [](const CLArgBase& arg) { return arg.getLongOption(); });
  EXPECT_TRUE(loc == v.cend());
}

//TEST(UtilsTest, locateOR) {
//  CLArg<string> arg1 = CLArg<string>::Builder{}.setOpt("-a").setLongOpt("--aaa").build();
//  CLArg<string> arg2 = CLArg<string>::Builder{}.setOpt("-b").setLongOpt("--bbb").build();
//  CLArg<string> arg3 = CLArg<string>::Builder{}.setOpt("-c").setLongOpt("--ccc").build();
//  std::vector<CLArgBase> v{arg1, arg2, arg3};
//  vector<string> o1{"-a", "--aaa"};
//  auto loc = locateOR<string, CLArgBase, vector<string>::iterator>(o1.begin(), o1.end(), v, [](const CLArgBase &arg) {
//    return arg.getOption();
//  });
//  ASSERT_TRUE(loc != v.cend());
//  EXPECT_EQ(arg1, *loc);
//  vector<string> o2{"-e", "--eee"};
//  loc = locateOR<string, CLArgBase, vector<string>::iterator>(o2.begin(), o2.end(), v, [](const CLArgBase &arg) {
//    return arg.getOption();
//  });
//  EXPECT_TRUE(loc == v.cend());
//}

TEST(UtilsTest, filloptions) {
	// Test string
	CLArg<string> args = CLArg<string>::Builder{}.setCltype(CLTYPE::STRING).build();
	bool b = fillOption("value1", args);
	EXPECT_TRUE(b);
	EXPECT_EQ("value1", args.getValue());
	EXPECT_TRUE(args.isPresent());
	// Test int
	CLArg<int> argi = CLArg<int>::Builder{}.setCltype(CLTYPE::INT).build();
	b = fillOption("5", argi);
	EXPECT_TRUE(b);
	EXPECT_EQ(5, argi.getValue());
	EXPECT_TRUE(argi.isPresent());
	// Test float
	CLArg<float> argf = CLArg<float>::Builder{}.setCltype(CLTYPE::FLOAT).build();
	b = fillOption("3.14", argf);
	EXPECT_TRUE(b);
	EXPECT_FLOAT_EQ(3.14, argf.getValue());
	EXPECT_TRUE(argf.isPresent());
	// Test bool
	CLArg<bool> argb = CLArg<bool>::Builder{}.setCltype(CLTYPE::BOOL).build();
	b = fillOption("true", argb);
	EXPECT_TRUE(b);
	EXPECT_TRUE(argb.getValue());
	EXPECT_TRUE(argb.isPresent());
	b = fillOption("false", argb);
	EXPECT_TRUE(b);
	EXPECT_FALSE(argb.getValue());
	EXPECT_TRUE(argb.isPresent());
    // Test quoted
	CLArg<string> argq = CLArg<string>::Builder{}.setCltype(CLTYPE::STRING).build();
	b = fillOption("value1 quoted", argq);
	EXPECT_TRUE(b);
	EXPECT_EQ("value1 quoted", argq.getValue());
	EXPECT_TRUE(argq.isPresent());
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
