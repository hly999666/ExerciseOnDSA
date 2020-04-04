//LeetCode 488. Zuma Game
#include "mainHeader.h"
#include <queue> 
#include <vector>
using namespace std;

class balls {

public:
	char color;//red(R) 0, yellow(Y) 1, blue(B) 2, green(G) 3, white(W) 4
	char counts;
	balls(char cr = 5, char ct = 0) : color(cr), counts(ct) {};
};


inline char colorToId(char c) {
	if (c == 'R')return 0;
	if (c == 'Y')return 1;
	if (c == 'B')return 2;
	if (c == 'G')return 3;
	if (c == 'W')return 4;
	return -1;
}

inline char idToColor(char c) {
	if (c ==0)return  'R';
	if (c ==1)return  'Y';
	if (c ==2)return  'B';
	if (c == 3)return 'G';
	if (c == 4)return 'W';
	return -1;
}
class state {
private:
	string str;
	vector<balls> table;
	int hands[5];//red(R) 0, yellow(Y) 1, blue(B) 2, green(G) 3, white(W) 4
	int boards[5];//red(R) 0, yellow(Y) 1, blue(B) 2, green(G) 3, white(W) 4
	int depth;
	int remain;

public:
	state() :depth(0), remain(0) {
		for (int& i : hands) i = 0;
	}
	state(string& board, string& hand) {
		depth = 0; remain = board.size();
		for (int& i : hands) i = 0;
		for (int& i : boards) i = 0;
		table.push_back(balls(colorToId(board[0]), 1));
		boards[colorToId(board[0])] = 1;
		for (int i = 1; i < board.size(); i++) {
			boards[colorToId(board[i])]++;
			if (board[i] == board[i - 1]) {
				table[table.size() - 1].counts++;
			}
			else {
				table.push_back(balls(colorToId(board[i]), 1));
			}
		}

		for (char c : hand)hands[colorToId(c)]++;
		//setStr();
	};
	bool operator<(const state& b)const {
		if (depth > b.depth)return true;
		if (depth < b.depth)return false;
		if (remain > b.remain)return true;
		if (remain < b.remain)return false;
		return false;
	}
	int produceNextAndTestEmpty(priority_queue<state>& pq, int ans) {
		vector<int>pos_1; vector<int>pos_2;
		for (int i = 0; i < table.size(); i++) {
			char cr = table[i].color;
			char ct = table[i].counts;
			if (ct == 1 && hands[cr] >= 2)pos_1.push_back(i);
			if (ct == 2 && hands[cr] >= 1)pos_2.push_back(i);
		}
		for (int i : pos_2) {
			state neo;
			if (removeAndMergeAndTestEmpty(neo, i, 1))return depth + 1;
			if (ans != -1 && neo.getDepth() >= ans)continue;
			if (neo.isPossible())pq.push(neo);
		}

		for (int i : pos_1) {
			state neo;
			if (removeAndMergeAndTestEmpty(neo, i, 2))return depth + 2;
			if (ans != -1 && neo.getDepth() >= ans)continue;
			if (neo.isPossible())pq.push(neo);
		}
		return 0;
	}
	void produceNextWithOutRemoving(priority_queue<state>& pq, int ans) {
		if (ans != -1 && depth + 1 >= ans)return;
		for (int pos = 0; pos < table.size(); pos++) {
			for (int color = 0; color < 5; color++) {
				if (hands[color] >= 1) {
					state before;
					if (tryInsertWithOutRemoving(pos, 'B', color, 1, before)) {
						if (before.isPossible())pq.push(before);
					}
					state between;
					if (tryInsertWithOutRemoving(pos, 'I', color, 1, between)) {
						if (between.isPossible())pq.push(between);
					}
					state after;
					if (tryInsertWithOutRemoving(pos, 'A', color, 1, after)) {
						if (after.isPossible())pq.push(after);
					}
 
				}
				if (hands[color] >= 2) {
					if (ans != -1 && depth +2>= ans)continue;
					state before;
					if (tryInsertWithOutRemoving(pos, 'B', color, 2, before)) {
						if (before.isPossible())pq.push(before);
					}
					state between;
					if (tryInsertWithOutRemoving(pos, 'I', color, 2, between)) {
						if (between.isPossible())pq.push(between);
					}
					state after;
					if (tryInsertWithOutRemoving(pos, 'A', color, 2, after)) {
						if (after.isPossible())pq.push(after);
					}
				}
			}
		}
	}
	int getDepth()const {
		return depth;
	}
	bool isPossible()const {
		for (int i = 0; i < 5; i++) {
			if (boards[i] != 0 && hands[i] + boards[i] < 3)return false;
		}
		return true;
	}
private:
	bool removeAndMergeAndTestEmpty(state& out, int id, char type)const {
		char cr = table[id].color; int sum = table[id].counts;
		int begin = id; int end = id;
		while (true) {
			//test over-range
			if (begin - 1 < 0)break;
			if (end + 1 >= table.size())break;
			//test count is zero
			auto& b = table[begin - 1];
			if (b.counts == 0) { begin--; continue; }
			auto& e = table[end + 1];
			if (e.counts == 0) { end++; continue; }
			//test color and count
			if (b.color != e.color)break;
			if (b.counts + e.counts < 3)break;

			sum += b.counts + e.counts;
			begin--; end++;
		}
		if (remain - sum == 0)return true;
		out.table = table;
		out.depth = depth + type;
		out.remain = remain - sum;
		for (int i = 0; i < 5; i++) {
			out.hands[i] = hands[i];
			out.boards[i] = boards[i];
		}
		out.hands[cr] -= type;
		for (int i = begin; i <= end; i++) {
			out.boards[out.table[i].color] -= out.table[i].counts;
			out.table[i].counts = 0;
		}
		if (begin - 1 >= 0 && end + 1 < out.table.size()) {
			if (out.table[begin - 1].color == out.table[end + 1].color) {
				if (out.table[begin - 1].counts == 1 && out.table[end + 1].counts == 1) {
					out.table[begin - 1].counts = 2;
					out.table[end + 1].counts = 0;
				}
			}
		}
		//out.setStr();
		return false;
	}
	void setStr() {
		for (auto& b : table) {
			for (int i = 0; i < b.counts; i++) {
				str.push_back(idToColor(b.color));
			}
		}
	}
	bool tryInsertWithOutRemoving(int pos, char type,int color, int count, state& out) {
		return false;
	}
};



int findMinStep(string board, string hand) {
	//if (board == "RRWWRRBBRR" && hand == "WB")return 2;//problem!! If insert are two step,this case shouldn't be solvable
	priority_queue<state> pq;
	state root(board, hand); 
	if (!root.isPossible())return -1;
	pq.push(root);
	int ans = -1;
	while (!pq.empty()) {
		auto st = pq.top(); pq.pop();
		if (ans != -1 && st.getDepth() >= ans)continue;
		int rst = st.produceNextAndTestEmpty(pq,ans);
		if (rst != 0) {
			if (ans == -1) {
				ans = rst;
			}else ans = min(rst, ans);
		}
		st.produceNextWithOutRemoving(pq, ans);
	}
	return ans;
}
