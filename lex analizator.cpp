#include "lex analizator.h"


token::token(string lex, string type_lex) : lexema(lex), type_lexema(type_lex) {}
token::token(){
	lexema = "";
	type_lexema = "";
};

lexor::lexor(int _cur_pos, string _text) : cur_pos(cur_pos), text(_text) {
	automat = create_my_dfa();
}

token lexor::get_next_lexem(){
	int state_automat = 0;
	string lexem="", type_lex;

	while (text[cur_pos] == ' ' && cur_pos != text.size() - 1) {
		cur_pos++;
	}

	for (int i=cur_pos; i<text.size(); i++){
		if (text[i] == ' ') {
			lexem = text.substr(cur_pos, i - cur_pos);
			type_lex=type_lexem[state_automat];
			cur_pos = i + 1;
			break;
		}

		state_automat = automat.getState(state_automat, text[i]);

		if (state_automat == -1) {
			type_lex = "ERROR";
			while (text[i] != ' ' && i != text.size()-1) {
				i++;
			}
			lexem = text.substr(cur_pos, i - cur_pos);
			cur_pos = i+1;
			break;
		}
	}

	if (lexem == "") {
		if (state_automat == -1) {
			type_lex = "ERROR";
		}
		else {
			type_lex = type_lexem[state_automat];
		}
		int i = text.size() - 1;
		lexem = text.substr(cur_pos, i + 1 - cur_pos);
		cur_pos = i + 1;
	}

	if (type_lex == "ERROR") {
		error_token.push_back(token(lexem, type_lex));
	}

	return token(lexem, type_lex);
}

vector<token> lexor::get_all_lexem(){
	vector<token> v;
	while (cur_pos != text.size()) {
		v.push_back(get_next_lexem());
	}
	return v;
}

dfa lexor::create_my_dfa(){
	string alphavit_str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789(){},;+=-.";
	vector<char> alphavit(alphavit_str.size());
	for (int i = 0; i < alphavit_str.size(); i++) {
		alphavit[i] = alphavit_str[i];
	}
	int cnt_states = 27;
	vector<int> final_states = { 1,2,3,4,5,24 };
	// transitFunction[i][j] = q  из i-го состояния по j-му символу входного алфавита автомат переходит 
	// в множество состояний, которые лежат в векторе q
	vector<vector<vector<int>>> transit_func(cnt_states, vector<vector<int>>(alphavit.size(), vector<int>(1, -1)));
	for (int i = 0; i <= 51; i++) { //буквы
		transit_func[0][i] = { 4 };
		transit_func[4][i] = { 4 };
	}
	for (int i = 53; i <= 61; i++) { //цифры с 1
		transit_func[0][i] = { 3 };
		transit_func[25][i] = { 3 };
	}
	for (int i = 52; i <= 61; i++) { //цифры с 0
		transit_func[3][i] = { 3 };
		//transit_func[24][i] = { 3 };
		transit_func[26][i] = { 3 };
	}
	for (int i = 62; i <= 67; i++) { // (){},;
		transit_func[0][i] = { 1 };
	}
	transit_func[0][68] = { 2 }; //+
	transit_func[0][69] = { 2 }; //=
	transit_func[0][70] = { 2, 23, 25 }; //-
	transit_func[0][17].push_back(6); //r
	transit_func[0][8].push_back(11); //i
	transit_func[0][5].push_back(13); //f
	transit_func[0][8].push_back(17);
	transit_func[0][5].push_back(20);
	transit_func[23][52] = { 24 }; //0
	transit_func[0][52] = { 24 }; //0
	transit_func[3][71] = { 26 }; //.
	transit_func[24][71] = { 26 }; //.


	transit_func[6][4] = { 7 };
	transit_func[7][19] = { 8 };
	transit_func[8][20] = { 9 };
	transit_func[9][17] = { 10 };
	transit_func[10][13] = { 5 };

	transit_func[11][13] = { 12 };
	transit_func[12][19] = { 5 };

	transit_func[13][11] = { 14 };
	transit_func[14][14] = { 15 };
	transit_func[15][0] = { 16 };
	transit_func[16][19] = { 5 };

	transit_func[17][19] = { 18 };
	transit_func[18][14] = { 19 };
	transit_func[19][5] = { 5 };

	transit_func[20][19] = { 21 };
	transit_func[21][14] = { 22 };
	transit_func[22][8] = { 5 };

	nfa my_nfa(cnt_states, alphavit, final_states, transit_func);
	dfa my_dfa = my_nfa.createDFA();

	//                           0       1      2     3     4      5         6      7     8      9           
	vector<string> type_lex = { "ERROR", "ID", "ID", "ID", "ID", "CONST", "CONST", "SEP", "OP", "OP" };
	for (int i = 10; i <= 23; i++) {
		type_lex.push_back("ID");
	}
	type_lex[15] = "ERROR";
	type_lex[18] = "KEYWORD";
	type_lexem=type_lex;
	return my_dfa;
}

