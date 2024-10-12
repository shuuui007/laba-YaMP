#pragma once
#include "dfa.h"
#include <vector>
#include <string>
#include <set>
#include <queue>
#include <algorithm>
class nfa
{
private:
	int countStates; // ���������� ��������� ��������
	std::vector<char> alphabet; // ������� �������
	std::vector<int> finalStates; // ��������� ����������� ���������
	std::vector<std::vector<std::vector<int>>> transitFunction; // ������� ���������
	// transitFunction[i][j] = q ��������, ��� �� i-�� ��������� �� j-�� ������� �������� �������� ������� ��������� � ��������� ���������, ������� ����� � ������� q
	std::set<int> getStates(std::vector<int> states, char c);
public:
	nfa(int countStates, std::vector<char> alphabet, std::vector<int> finiteStates, std::vector<std::vector<std::vector<int>>> transitFunction);
	bool isAccept(std::string s);
	dfa createDFA();
};

