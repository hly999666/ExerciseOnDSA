// LeetCode.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "myHeader.h"
#include <vector>
using namespace std;
int main()
{ 
	std::vector<int>nums({1,2,3,4,5});
	auto head = buildList(nums);
	auto ans= isValidSerialization("9,3,4,#,#,1,#,#,2,#,6,#,#");
    return 0;
}

