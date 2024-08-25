#include <Gadget.h>
#include <Data/HashTable.h>
#include <Data/String.h>

#include "_Catch2/catch_amalgamated.hpp"

using namespace Gadget;

//------------------------------------------------------------//
//------------------- Array Accessor -------------------------//
//------------------------------------------------------------//
//Check if all characters in the string are unique
//Naive O(n^2) implementation
static inline bool IsUnique1(const String& str){
	for(int i = 0; i < str.Length(); i++){
		for(int j = i + 1; j < str.Length(); j++){
			if(str[i] == str[j]){
				return false;
			}
		}
	}

	return true;
}

//O(n log n) average, O(n ^ 2) worst-case
static inline bool IsUnique2(String str){ //Intentional copy so we can sort the string in place
	str.QuickSort();
	for(size_t i = 0; i < str.Length() - 1; i++){
		if(str[i] == str[i + 1]){
			return false;
		}
	}

	return true;
}

//O(n), with heavy space complexity
static inline bool IsUnique3(const String& str){
	HashTable<char, size_t> hashTable;

	for(size_t i = 0; i < str.Length(); i++){
		if(hashTable.Contains(str[i])){
			return false;
		}

		hashTable.Add(str[i], i);
	}

	return true;
}

//O(n)
static inline bool IsOneEditAway(const String& str1, const String& str2){
	int64_t lengthDist = str1.Length() - str2.Length();
	if(Math::Abs(lengthDist) > 1){
		return false; //Requires more than one edit no matter what
	}

	bool canAdd = lengthDist == -1;
	bool canRemove = lengthDist == 1;
	bool canSwap = lengthDist == 0;

	size_t i = 0;
	size_t j = 0;
	for(; i < str1.Length() && j < str2.Length(); i++, j++){
		if(str1[i] == str2[j]){
			continue;
		} else{
			if(canAdd){
				canAdd = false;
				i--;
				continue;
			} else if(canRemove){
				canRemove = false;
				j--;
				continue;
			} else if(canSwap){
				canSwap = false;
				continue;
			} else{
				return false;
			}
		}
	}

	return true;
}

TEST_CASE("String Array Accessor" "[string_array_access]"){
	const String str1 = "Hello World";
	const String str2 = String("Helo Wrd");

	REQUIRE(str1.Length() == 11);
	REQUIRE(str2.Length() == 8);

	REQUIRE(str1[0] == str2[0]);
	REQUIRE(str1[1] == str2[1]);
	REQUIRE(str1[2] == str2[2]);
	REQUIRE(str1[3] != str2[3]);
	REQUIRE(str1[4] != str2[4]);

	REQUIRE(str1[0] == 'H');
	REQUIRE(str1[1] == 'e');
	REQUIRE(str1[2] == 'l');
	REQUIRE(str1[3] == 'l');
	REQUIRE(str1[4] == 'o');

	REQUIRE(str2[0] == 'H');
	REQUIRE(str2[1] == 'e');
	REQUIRE(str2[2] == 'l');
	REQUIRE(str2[3] == 'o');
	REQUIRE(str2[4] == ' ');

	REQUIRE(!IsUnique1(str1));
	REQUIRE(!IsUnique2(str1));
	REQUIRE(!IsUnique3(str1));

	REQUIRE(IsUnique1(str2));
	REQUIRE(IsUnique2(str2));
	REQUIRE(IsUnique3(str2));

	REQUIRE(IsOneEditAway("pale", "ple"));
	REQUIRE(IsOneEditAway("pales", "pale"));
	REQUIRE(IsOneEditAway("pale", "bale"));
	REQUIRE(!IsOneEditAway("pale", "bake"));
}

//------------------------------------------------------------//
//------------------- String::Contains -----------------------//
//------------------------------------------------------------//
//Brute Force (Note - we're assuming this should be case sensitive)
//O(n^2)
static inline bool IsPermutation(const String& base, String permutation){ //Intentional copy
	if(permutation.Length() != base.Length()){
		return false;
	}

	for(int i = 0; i < base.Length(); i++){
		if(permutation.Contains(base[i])){
			permutation.Remove(base[i]);
			if(permutation.IsEmpty()){
				return true;
			}
		}else{
			return false;
		}
	}

	return false;
}

//O(n log n) average, O(n ^ 2) worst-case
static inline bool IsPermutation2(String base, String permutation){ //Intentional copies
	if(permutation.Length() != base.Length()){
		return false;
	}

	base.QuickSort();
	permutation.QuickSort();
	return base == permutation;
}

TEST_CASE("String::Contains", "[string_contains]"){
	const String str1a = "Hello";		const String str1b = "lHe";
	const String str2a = "aaaaaaaaaa";	const String str2b = "aab";
	const String str3a = "God";			const String str3b = "doG";
	String str4 = "Mr John Smith      ";
	String str5 = "  _   .  ";
	String str6 = "...";

	REQUIRE(str1a.Contains("H"));
	REQUIRE(str1a.Contains("e"));
	REQUIRE(str1a.Contains("l"));
	REQUIRE(str1a.Contains("o"));
	REQUIRE(!str1a.Contains("W"));
	REQUIRE(!str1a.Contains("X"));
	REQUIRE(!str1a.Contains("Y"));
	REQUIRE(!str1a.Contains("Z"));
	REQUIRE(!str1a.Contains(-1));
	REQUIRE(str1a.Contains("Hell"));
	REQUIRE(str1a.Contains("Hello"));
	REQUIRE(!str1a.Contains("Hell0"));
	REQUIRE(!str1a.Contains("Helloo"));

	REQUIRE(str2a.Contains("aaa"));
	REQUIRE(!str2a.Contains("aba"));

	REQUIRE(!IsPermutation(str1a, str1b));
	REQUIRE(!IsPermutation2(str1a, str1b));

	REQUIRE(!IsPermutation(str2a, str2b));
	REQUIRE(!IsPermutation2(str2a, str2b));

	REQUIRE(IsPermutation(str3a, str3b));
	REQUIRE(IsPermutation2(str3a, str3b));

	size_t oldLength4 = str4.Length();
	str4.FindAndReplace(' ', "%20");
	REQUIRE(str4.Length() >= oldLength4);
	REQUIRE(!str4.Contains(' '));

	size_t oldLength5 = str5.Length();
	str5.FindAndReplace(' ', "%20");
	REQUIRE(str5.Length() >= oldLength5);
	REQUIRE(!str5.Contains(' '));

	size_t oldLength6 = str6.Length();
	str6.FindAndReplace(' ', "%20");
	REQUIRE(str6.Length() >= oldLength6);
	REQUIRE(!str6.Contains(' '));
}

//------------------------------------------------------------//
//--------------------- String::Trim -------------------------//
//------------------------------------------------------------//
TEST_CASE("String::Trim", "[string_trim]"){
	String str1 = "Mr John Smith      ";
	String str2 = "  _   .  ";
	String str3 = "...";

	str1.Trim();
	REQUIRE(str1.Length() == 13);
	REQUIRE(str1[0] == 'M');
	REQUIRE(str1[12] == 'h');

	str2.Trim();
	REQUIRE(str2.Length() == 5);
	REQUIRE(str2[0] == '_');
	REQUIRE(str2[4] == '.');

	size_t oldLength = str3.Length();
	str3.Trim();
	REQUIRE(str3.Length() == oldLength);
	REQUIRE(str3[0] == '.');
	REQUIRE(str3[1] == '.');
	REQUIRE(str3[2] == '.');
}

//------------------------------------------------------------//
//------------------- String::ToLower ------------------------//
//------------------------------------------------------------//
//O(n log n)
static inline bool IsPalindromePermutation(String str){ //Intentional copy
	str.RemoveAll(' ');

	if(str.Length() <= 1){
		return true;
	}

	str.ToLower(); //O(n)
	str.QuickSort(); //O(n log n)

	bool hasMiddle = str.Length() % 2 == 0; //True if even

	for(size_t i = 0; i < str.Length(); i++){
		size_t numOfThisChar = 1;
		for(size_t j = i + 1; j < str.Length(); j++){
			if(str[j] != str[i]){
				break;
			}

			numOfThisChar++;
		}

		if(numOfThisChar % 2 != 0){
			if(hasMiddle){
				return false;
			}

			hasMiddle = true;
		}

		i += numOfThisChar - 1;
	}

	return true;
}

TEST_CASE("String::ToLower", "[string_tolower]"){
	REQUIRE(IsPalindromePermutation("Tact Coa"));
	REQUIRE(!IsPalindromePermutation("aaaaab"));
}

//------------------------------------------------------------//
//-------------------- String::Append ------------------------//
//------------------------------------------------------------//

//O(n)
static inline String CompressString(const String& str){
	String compressed = String(str.Length());

	for(size_t i = 0; i < str.Length(); i++){
		int charCount = 1;
		for(size_t j = i + 1; j <= str.Length(); j++){
			if(j == str.Length() || str[i] != str[j]){
				if(compressed.Length() + 2 == str.Length()){
					return str;
				}

				compressed.Append(str[i]);
				compressed.Append(charCount);
				i = j - 1;
				break;
			}

			charCount++;
		}
	}

	if(compressed.Length() >= str.Length()){
		return str;
	}

	return compressed;
}

TEST_CASE("String::Append", "[string_append]"){
	REQUIRE(CompressString("aabcccccaaa") == "a2b1c5a3");
	REQUIRE(CompressString("aabbccddee") == "aabbccddee");
	REQUIRE(CompressString("abcde") == "abcde");
}

//------------------------------------------------------------//
//-------------- String Rotation (CTCI 1.9) ------------------//
//------------------------------------------------------------//
//O(n)
static inline bool IsRotation(const String& s1, const String& s2){
	if(s1.Length() != s2.Length()){
		return false;
	}

	const int64_t start = s2.Find(s1[0]);
	if(start < 0){
		return false;
	}

	String unrotated;
	for(size_t i = start; i < s2.Length(); i++){
		unrotated.Append(s2[i]);
	}

	for(int64_t i = 0; i < start; i++){
		unrotated.Append(s2[i]);
	}

	return unrotated == s1;
}

TEST_CASE("String Rotation", "[string_rotation]"){
	String s1 = "waterbottle";
	String s2 = "erbottlewat";

	REQUIRE(s1.Length() == s2.Length());
	REQUIRE(IsRotation(s1, s2));
}

//------------------------------------------------------------//
//Longest Substring Without Repeating Characters (Leetcode 3) //
//------------------------------------------------------------//

static inline size_t LengthOfLongestSubstring(const String& s){
	if(s.Length() <= 1){
		return s.Length();
	}

	String initialString = s;
	size_t longestSubStr = 0;

	for(size_t i = 0; i < initialString.Length(); i++){
		bool endOfString = true;

		String substr = initialString.SubString(i, i + 1);

		for(size_t j = i + 1; j < initialString.Length(); j++){
			if(substr.Find(initialString[j]) < 0){
				substr += initialString[j];
			}else{
				if(substr.Length() > longestSubStr){
					longestSubStr = substr.Length();
				}

				auto nextIndex = initialString.Find(initialString[j], i);
				if(nextIndex >= 0){
					i = nextIndex;
					endOfString = false;
					break;
				}
			}
		}

		if(endOfString){
			auto remainingSubStr = initialString.SubString(i, initialString.Length()).Length();
			if(remainingSubStr > longestSubStr){
				longestSubStr = remainingSubStr;
				break;
			}
		}
	}

	return longestSubStr;
}

TEST_CASE("String Longest Substring", "[string_longest_substring]"){
	String s1 = "abcabcbb";
	String s2 = "bbbbb";
	String s3 = "pwwkew";

	REQUIRE(LengthOfLongestSubstring(s1) == 3);
	REQUIRE(LengthOfLongestSubstring(s2) == 1);
	REQUIRE(LengthOfLongestSubstring(s3) == 3);
}

//------------------------------------------------------------//
//------------ Score of a String (Leetcode 3110) -------------//
//------------------------------------------------------------//

static inline size_t GetStringScore(const String& str){
	if(str.Length() < 2){
		return 0;
	}

	size_t score = 0;

	for(int64_t i = 0; i < str.Length() - 1; i++){
		int64_t aVal = str[i];
		int64_t bVal = str[i + 1];

		score += static_cast<size_t>(Math::Abs(aVal - bVal));
	}

	return score;
}

TEST_CASE("String Get String Score", "[string_get_string_score]"){
	REQUIRE(GetStringScore("hello") == 13);
	REQUIRE(GetStringScore("zaz") == 50);
	REQUIRE(GetStringScore("") == 0);
	REQUIRE(GetStringScore("a") == 0);
	REQUIRE(GetStringScore("aaaaaaaaaaaa") == 0);
}