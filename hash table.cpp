#include "hash table.h"

hash_table::hash_table(int N) : N(N) {};

int hash_table::hash_function(string lexem){
    int sum = 0;
    for (int i = 0; i < lexem.size(); i++) {
        sum += (i + 1) * int(lexem[i]);
        sum %= N;
    }
    return sum;
}

void hash_table::append(token tok){
    if (tok.type_lexema != "ERROR") {
        int id = this->hash_function(tok.lexema);
        while (table[id].lexema != "" && table[id].lexema != tok.lexema) {
            id++;
            if (id == N) id = 0;
        }
        if (table[id].lexema == "") {
            table[id] = tok;
        }
    }
}

string hash_table::print_table_as_string(){
    string str = "";
    for (int i = 0; i < N; i++) {
        if (table[i].lexema != "") {
            str+= table[i].type_lexema + " | " + table[i].lexema + " | " + to_string(i) + "\n";
        }
    }
    return str;
}


void hash_table::get_all_lexem_to_hashtable(lexor& lex_analizator){
    while (lex_analizator.cur_pos != lex_analizator.text.size()) {
        this->append(lex_analizator.get_next_lexem());
    }
}
