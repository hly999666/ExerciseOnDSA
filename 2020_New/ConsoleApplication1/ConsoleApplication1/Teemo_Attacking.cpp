//LeetCode 495. Teemo Attacking


#include "mainHeader.h"
 
using namespace std;
int findPoisonedDuration(vector<int>& timeSeries, int duration) {
    int ans = 0;
    for (int i = 1; i < timeSeries.size(); i++)ans += min(timeSeries[i] - timeSeries[i - 1], duration);
    ans += duration;
    return ans;
}