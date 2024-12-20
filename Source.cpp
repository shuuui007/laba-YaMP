#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "dfa.h"
#include "nfa.h"
#include "lex analizator.h"
#include "hash table.h"
#include "syntax an.h"

using namespace std;


int main() {
	ifstream in("text.txt");
	string str = "", text="";
	in >> str;
	while (!(in.fail())) {
		text += str+" ";
		in >> str;
	}
	ofstream out("tree.txt");
	ofstream out2("semantic_an.txt");
	lexor my_lexor(0, text);
	Syntaxor A(my_lexor, out, out2);
	A.analyse();


	/*int N = 1000;
	hash_table my_ht(N);
	my_ht.get_all_lexem_to_hashtable(my_lexor);

	string file_output="tokens.txt";
	ofstream out(file_output);
	out << my_ht.print_table_as_string() << endl << endl;
	for (int i = 0; i < my_lexor.error_token.size(); i++) {
		out << my_lexor.error_token[i].lexema << " | unrecognized lexeme" << endl;
	}*/
}