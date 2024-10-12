#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <set>
class dfa
{
public :
	int countStates; // ���������� ��������� ��������
	std::vector<char> alphabet; // ������� �������
	std::vector<int> finalStates; // ��������� ����������� ���������
	std::vector<std::vector<int>> transitFunction; // ������� ��������� 
	// transitFunction[i][j] = k ��������, ��� �� i-�� ��������� �� j-�� ������� �������� �������� ������� ��������� � ��������� k
	int getState(int state, char c);
	std::set<int> inverse(int state, int k);
public:
	dfa(int countStates, std::vector<char> alphabet, std::vector<int> finalStates, std::vector<std::vector<int>> transitFunction);
	dfa();
	bool isAccept(std::string s);
	dfa minimize();
	std::vector<std::vector<int>> quotientSet();
};

