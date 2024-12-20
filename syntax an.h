#pragma once
#include "lex analizator.h"
#include <iostream>
#include <fstream>
#include <map>

using namespace std;

struct Node {
	string str = "";
	vector<Node*> children;
	Node(string t) : str(t) {}
	Node() = default;
	void print(ofstream& out, string s = "");

	string tr = "";
	string type = "";
	int cnt = 0;
	vector<Node*> tr_depends;
	vector<Node*> type_depends;

};

class Syntaxor {
public:
	lexor lex;
	Node tree_node;
	bool flag = true;
	bool descr_passed = false;
	int pos = 0;
	ofstream& out;
	ofstream& out_seman_an;
	vector<string> declared_id;
	map<string, string> type_decl_id;
	Syntaxor(lexor lex_, ofstream& out, ofstream& out2) : lex(lex_), out(out), out_seman_an(out2) {}
	void analyse();
	void Function();
	void Begin(token&t, Node& n);
	void End(token&t, Node& n);
	void FuncName(token&t, Node& n);
	void Descriptions(token&t, Node& n);
	void Operators(token&t, Node& n);
	void Descr(token&t, Node& n);
	void VarList(token&t, Node& n);
	void Type(token&t, Node& n);
	void Op(token&t, Node& n);
	void SimpleExpr(token&t, Node& n);
	void Expr(token&t, Node& n);
	void Id(token&t, Node& n);
	void Const(token&t, Node& n);
};