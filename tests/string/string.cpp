/*
 * readline.c
 *
 *  Created on: 26.06.2013
 *      Author: moric
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtest/gtest.h>
extern "C" {
	#include "../../src/rw_mesh.h"
}

TEST(Test, in_charset){

	ASSERT_LT(in_charset('a',CHARSET_SPACE), 0);
	ASSERT_LT(in_charset('0',CHARSET_SPACE), 0);
    ASSERT_LT(in_charset('9',CHARSET_SPACE), 0);
    ASSERT_GE(in_charset(' ',CHARSET_SPACE), 0);
    ASSERT_GE(in_charset('\t',CHARSET_SPACE), 0);
    ASSERT_GE(in_charset('\r',CHARSET_SPACE), 0);
    ASSERT_GE(in_charset('\n',CHARSET_SPACE), 0);
}

#define _TRIMC_TEST(FROM,TO,FUNCTION,CHARSET) 	strcpy(string1,FROM); \
	strcpy(string2,TO); \
	FUNCTION(string1,CHARSET); \
	ASSERT_STREQ(string1,string2);

#define _TRIM_TEST(FROM,TO,FUNCTION) 	strcpy(string1,FROM); \
	strcpy(string2,TO); \
	FUNCTION(string1); \
	ASSERT_STREQ(string1,string2);

TEST(Test, string_ltrim_charset){
	char string1[256],string2[256];

	_TRIMC_TEST("samplestring","samplestring",string_ltrim_charset,CHARSET_SPACE)
	_TRIMC_TEST("sample  string","sample  string",string_ltrim_charset,CHARSET_SPACE)
	_TRIMC_TEST("samplestring  ","samplestring  ",string_ltrim_charset,CHARSET_SPACE)
	_TRIMC_TEST("  samplestring","samplestring",string_ltrim_charset,CHARSET_SPACE)
	_TRIMC_TEST("\t   samplestring","samplestring",string_ltrim_charset,CHARSET_SPACE)
	_TRIMC_TEST("\t sample  string  ","sample  string  ",string_ltrim_charset,CHARSET_SPACE)

}

TEST(Test, string_rtrim_charset){
	char string1[256],string2[256];

	_TRIMC_TEST("samplestring","samplestring",string_rtrim_charset,CHARSET_SPACE)
	_TRIMC_TEST("sample  string","sample  string",string_rtrim_charset,CHARSET_SPACE)
	_TRIMC_TEST("samplestring  ","samplestring",string_rtrim_charset,CHARSET_SPACE)
	_TRIMC_TEST("  samplestring","  samplestring",string_rtrim_charset,CHARSET_SPACE)
	_TRIMC_TEST("\t   samplestring\t   ","\t   samplestring",string_rtrim_charset,CHARSET_SPACE)
	_TRIMC_TEST("\t sample  string \t ","\t sample  string",string_rtrim_charset,CHARSET_SPACE)

}

TEST(Test, string_trim_charset){
	char string1[256],string2[256];

	_TRIMC_TEST("samplestring","samplestring",string_trim_charset,CHARSET_SPACE)
	_TRIMC_TEST("sample  string","sample  string",string_trim_charset,CHARSET_SPACE)
	_TRIMC_TEST("samplestring  ","samplestring",string_trim_charset,CHARSET_SPACE)
	_TRIMC_TEST("  samplestring","samplestring",string_trim_charset,CHARSET_SPACE)
	_TRIMC_TEST("\t   samplestring\t   ","samplestring",string_trim_charset,CHARSET_SPACE)
	_TRIMC_TEST("\t sample  string \t ","sample  string",string_trim_charset,CHARSET_SPACE)

}


TEST(Test, string_trim){
	char string1[256],string2[256];

	_TRIM_TEST("samplestring","samplestring",string_trim)
	_TRIM_TEST("sample  string","sample  string",string_trim)
	_TRIM_TEST("samplestring  ","samplestring",string_trim)
	_TRIM_TEST("  samplestring","samplestring",string_trim)
	_TRIM_TEST("\t   samplestring\t   ","samplestring",string_trim)
	_TRIM_TEST("\t sample  string \t ","sample  string",string_trim)

}


TEST(Test, string_is_integer){
    ASSERT_EQ(string_is_integer("a"), 0);
    ASSERT_EQ(string_is_integer("aasd"), 0);
    ASSERT_EQ(string_is_integer("aa2sd"), 0);
    ASSERT_EQ(string_is_integer("3aa2sd"), 0);
    ASSERT_EQ(string_is_integer("321a"), 0);
    ASSERT_EQ(string_is_integer("321 "), 0);
    ASSERT_EQ(string_is_integer("321"), 1);
    ASSERT_EQ(string_is_integer("+321"), 1);
    ASSERT_EQ(string_is_integer("-321"), 1);
    ASSERT_EQ(string_is_integer("-321+"), 0);
    ASSERT_EQ(string_is_integer("-32-1"), 0);
    ASSERT_EQ(string_is_integer("-32.1"), 0);
}


TEST(Test, string_count_of_words){
    ASSERT_EQ(string_count_of_words(""), 0);
    ASSERT_EQ(string_count_of_words("a"), 1);
    ASSERT_EQ(string_count_of_words("aasd"), 1);
    ASSERT_EQ(string_count_of_words("aasd  asdads"), 2);
    ASSERT_EQ(string_count_of_words(" aasd  asdads "), 2);
    ASSERT_EQ(string_count_of_words(" aasd \t  \n asdads "), 2);
    ASSERT_EQ(string_count_of_words(" aasd  asdads asdasd "), 3);
    ASSERT_EQ(string_count_of_words("a a s d  a s d a d s asdasd "), 11);
}

TEST(Test, string_cut_word){
	char string[256];
	char word[256];

#define _TEST_string_cut_word(F,R,T,W) 	strcpy(string,F); \
	ASSERT_EQ(string_cut_word(string,word),R); \
	ASSERT_STREQ(string,T); \
	ASSERT_STREQ(word,W);

	_TEST_string_cut_word("",0,"","")

	_TEST_string_cut_word("   ",0,"","")
	_TEST_string_cut_word("  \t ",0,"","")

	_TEST_string_cut_word("a",1,"","a")
	_TEST_string_cut_word("aasdzxc",1,"","aasdzxc")
	_TEST_string_cut_word("  aasdzxc",1,"","aasdzxc")
	_TEST_string_cut_word("aas-dzxc  ",1,"","aas-dzxc")
	_TEST_string_cut_word("  aa123sdzxc   ",1,"","aa123sdzxc")

	_TEST_string_cut_word("aasdzxc vvdaa",1,"vvdaa","aasdzxc")
	_TEST_string_cut_word(" aasdzxc vvdaa asdasfgasf",1,"vvdaa asdasfgasf","aasdzxc")
	_TEST_string_cut_word(" adasd123asdzxc vvdaa asdasfgasf   asd1asd a",1,"vvdaa asdasfgasf   asd1asd a","adasd123asdzxc")

	strcpy(string," adasd123asdzxc vvdaa asdasfgasf   asd1asd a  ");
	ASSERT_EQ(string_cut_word(string,word),1);
	ASSERT_STREQ(string,"vvdaa asdasfgasf   asd1asd a  ");
	ASSERT_STREQ(word,"adasd123asdzxc");

	ASSERT_EQ(string_cut_word(string,word),1);
	ASSERT_STREQ(string,"asdasfgasf   asd1asd a  ");
	ASSERT_STREQ(word,"vvdaa");

	ASSERT_EQ(string_cut_word(string,word),1);
	ASSERT_STREQ(string,"asd1asd a  ");
	ASSERT_STREQ(word,"asdasfgasf");

	ASSERT_EQ(string_cut_word(string,word),1);
	ASSERT_STREQ(string,"a  ");
	ASSERT_STREQ(word,"asd1asd");

	ASSERT_EQ(string_cut_word(string,word),1);
	ASSERT_STREQ(string,"");
	ASSERT_STREQ(word,"a");

	ASSERT_EQ(string_cut_word(string,word),0);
	ASSERT_STREQ(string,"");
	ASSERT_STREQ(word,"");
}




int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
