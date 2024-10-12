#pragma once
#include<string>
#include<fstream>
#include "lex analizator.h"

using namespace std;

class hash_table {
public:
	int N;
	token* table = new token[N];
	hash_table(int N);
	int hash_function(string lexem);
	void append(token tok);
	string print_table_as_string();
	void get_all_lexem_to_hashtable(lexor& lex_analizator);
};