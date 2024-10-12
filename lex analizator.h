#pragma once
#include "dfa.h"
#include "nfa.h"

using namespace std;

class token {
public:
	string lexema;
	string type_lexema;
	token(string lex, string type_lex);
	token();
};


class lexor {
public:
	string text;
	dfa automat;
	int cur_pos;
	vector<token> error_token;
	lexor(int _cur_pos, string _text);
	token get_next_lexem();
	vector<token> get_all_lexem();
	dfa create_my_dfa();
	vector<string> type_lexem;
};