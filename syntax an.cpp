#include "syntax an.h"

void Node::print(ofstream& out_, string s) 
{
	out_ << s << str << endl;
	for (int i = 0; i < children.size(); i++) {
		children[i]->print(out_, s + "     ");
	}
}

void Syntaxor::analyse()
{
	Function();
	if (flag) {
		tree_node.print(out, "");
		out_seman_an << endl << tree_node.tr;
	}
	else {
		cout << " -Sintax analysis error-";
		out << " -Sintax analysis error-";
		out_seman_an << " -Sintax analysis error-";
	}
}

void Syntaxor::Function()
{
	tree_node.str = "Function:";
	token t = lex.get_next_lexem();
	pos++;
	Begin(t, tree_node);
	if (flag) Descriptions(t, tree_node);
	if (flag) {
		descr_passed = true;
		Operators(t, tree_node);
	}
	if (flag) End(t, tree_node);

	for (int i = 0; i < tree_node.children.size(); i++) {
		tree_node.tr += tree_node.children[i]->tr+"\n";
	}
}

void Syntaxor::Begin(token& t, Node& n)
{
	Node* node = new Node("Begin:");
	n.children.push_back(node);
	Type(t, *node);
	FuncName(t, *node);

	for (int i = 0; i < 2; i++) {
		node->tr += node->children[i]->tr + " ";
	}
	node->tr +="2 FUNC_DECL";

	if (t.lexema == "(") {
		node->children.push_back(new Node("("));
		t = lex.get_next_lexem();
		pos++;
		if (t.lexema == ")") {
			node->children.push_back(new Node(")"));
			t = lex.get_next_lexem();
			pos++;
			if (t.lexema == "{") {
				node->children.push_back(new Node("{"));
				t = lex.get_next_lexem();
				pos++;
			}
			else {
				flag = false;
				cout << "the '{' symbol was expected at position " << pos << ", the symbol '" << t.lexema << "' was found" << endl;
			}
		}
		else {
			flag = false;
			cout << "the ')' symbol was expected at position " << pos << ", the lexem '" << t.lexema << "' was found" << endl;
		}
	}
	else {
		flag = false;
		cout << "the '(' symbol was expected at position "<< pos<< ", the lexem '"<<t.lexema<<"' was found"<<endl;
	}
}

void Syntaxor::End(token& t, Node& n)
{
	Node* node = new Node("End:");
	n.children.push_back(node);
	if (t.lexema == "return") {
		node->children.push_back(new Node("return"));
		t = lex.get_next_lexem();
		pos++;
		Id(t, *node);

		node->tr += node->children[1]->tr + " return";
		if (node->children[1]->type != type_decl_id[declared_id[0]]) {
			out_seman_an << "after 'return' at position " << pos << " is expected type " + type_decl_id[declared_id[0]] << endl;
		}

		if (t.lexema == ";") {
			node->children.push_back(new Node(";"));
			t = lex.get_next_lexem();
			pos++;
			if (t.lexema == "}") {
				node->children.push_back(new Node("}"));
				t = lex.get_next_lexem();
				pos++;

				if (t.lexema == "" && flag) {
					cout << " ~Sintax analysis was completed successfully~"<<endl;
				}
				else if (t.lexema != "") {
					flag = false;
					cout << "unwanted lexem '" << t.lexema << "' in the position " << pos << endl;
				}

			}
			else {
				flag = false;
				cout << "the '}' symbol was expected at position " << pos << ", the lexem '" << t.lexema << "' was found" << endl;
			}
		}
		else {
			flag = false;
			cout << "the ';' symbol was expected at position " << pos << ", the lexem '" << t.lexema << "' was found" << endl;
		}
	}
	else {
		flag = false;
		cout << "the 'return' keyword was expected at position " << pos << ", the lexem '" << t.lexema << "' was found" << endl;
	}
}

void Syntaxor::FuncName(token& t, Node& n)
{
	Node* node = new Node("FunctionName:");
	n.children.push_back(node);

	node->type = n.children[0]->type;

	Id(t, *node);

	node->tr += node->children[0]->tr;
}

void Syntaxor::Descriptions(token& t, Node& n)
{
	Node* node = new Node("Descriptions:");
	n.children.push_back(node);
	Descr(t, *node);

	node->tr += node->children[0]->tr;

	if (t.lexema == "int" || t.lexema == "float") {
		Descriptions(t, *node);

		node->tr += "\n" + node->children[1]->tr;
	}
}

void Syntaxor::Operators(token& t, Node& n)
{
	Node* node = new Node("Operators:");
	n.children.push_back(node);
	Op(t, *node);

	node->tr += node->children[0]->tr;

	if (t.type_lexema == "ID") {
		Operators(t, *node);

		node->tr +="\n"+node->children[1]->tr;
	}
}

void Syntaxor::Descr(token& t, Node& n)
{
	Node* node = new Node("Descr:");
	n.children.push_back(node);
	Type(t, *node);
	VarList(t, *node);

	node->tr += node->children[0]->tr + " " + node->children[1]->tr + " " + to_string(node->children[1]->cnt + 1) + " DECL";

	if (t.lexema == ";") {
		node->children.push_back(new Node(";"));
		t = lex.get_next_lexem();
		pos++;
	}
	else {
		flag = false;
		cout << "the ';' symbol was expected at position " << pos << ", the lexem '" << t.lexema << "' was found" << endl;
	}
}

void Syntaxor::VarList(token& t, Node& n)
{
	Node* node = new Node("VarList:");
	n.children.push_back(node);
	if (n.str == "Descr:") {
		node->type = n.children[0]->type;
	}
	else {
		node->type = n.type;
	}

	Id(t, *node);

	node->tr += node->children[0]->tr;
	node->cnt += node->children[0]->cnt;
	node->children[0]->type = node->type;

	if (t.lexema == ",") {
		node->children.push_back(new Node(","));
		t = lex.get_next_lexem();
		pos++;
		VarList(t, *node);

		node->tr += " "+node->children[2]->tr;
		node->cnt += node->children[2]->cnt;
		node->children[2]->type = node->type;
	}
}

void Syntaxor::Type(token& t, Node& n)
{
	Node* node = new Node("Type:");
	n.children.push_back(node);
	if (t.lexema == "int") {
		node->children.push_back(new Node("int"));

		node->type = "int";
		node->tr = "int";

		t = lex.get_next_lexem();
		pos++;
	}
	else if (t.lexema == "float") {
		node->children.push_back(new Node("float"));

		node->type = "float";
		node->tr = "float";

		t = lex.get_next_lexem();
		pos++;
	}
	else {
		flag = false;
		cout << "the 'int' or 'float' type was expected at position " << pos << ", the lexem '" << t.lexema << "' was found" << endl;
	}
}

void Syntaxor::Op(token& t, Node& n)
{
	Node* node = new Node("Op:");
	n.children.push_back(node);
	Id(t, *node);
	if (t.lexema == "=") {
		node->children.push_back(new Node("="));
		t = lex.get_next_lexem();
		pos++;

		Expr(t, *node);

		node->tr += node->children[0]->tr + " " + node->children[2]->tr + " =";
		if (node->children[0]->type != node->children[2]->type) {
			out_seman_an << "to the right of '=' at position " << pos << " is expected type " + node->children[0]->type << endl;
		}

		if (t.lexema == ";") {
			node->children.push_back(new Node(";"));
			t = lex.get_next_lexem();
			pos++;
		}
		else {
			flag = false;
			cout << "the ';' symbol was expected at position " << pos << ", the lexem '" << t.lexema << "' was found" << endl;
		}
	}
	else {
		flag = false;
		cout << "the '=' symbol was expected at position " << pos << ", the lexem '" << t.lexema << "' was found" << endl;
	}
}

void Syntaxor::Expr(token& t, Node& n)
{
	Node* node = new Node("Expr:");
	n.children.push_back(node);
	SimpleExpr(t, *node);

	node->tr += node->children[0]->tr;
	node->type = node->children[0]->type;

	if (t.lexema == "+") {
		node->children.push_back(new Node("+"));
		t = lex.get_next_lexem();
		pos++;
		Expr(t, *node);

		node->tr += " " + node->children[2]->tr + " +";
		if (node->children[0]->type != node->children[2]->type) {
			out_seman_an << "the left and right operands of the '+' sign are of different types (at position " << pos << ")" << endl;
		}
	}
	else if (t.lexema == "-") {
		node->children.push_back(new Node("-"));
		t = lex.get_next_lexem();
		pos++;
		Expr(t, *node);

		node->tr += " " + node->children[2]->tr + " -";
		if (node->children[0]->type != node->children[2]->type) {
			out_seman_an << "the left and right operands of the '-' sign are of different types (at position " << pos << ")" << endl;
		}
	}
}

void Syntaxor::SimpleExpr(token& t, Node& n)
{
	Node* node = new Node("SimpleExpr:");
	n.children.push_back(node);
	if (t.type_lexema == "ID") {
		Id(t, *node);

		node->tr += node->children[0]->tr;
		node->type = node->children[0]->type;
	}
	else if (t.type_lexema == "CONST") {
		Const(t, *node);

		node->tr += node->children[0]->tr;
		node->type = node->children[0]->type;
	}
	else if (t.lexema == "(") {
		node->children.push_back(new Node("("));
		t = lex.get_next_lexem();
		pos++;
		Expr(t, *node);

		node->tr += node->children[1]->tr;
		node->type = node->children[1]->type;

		if (t.lexema == ")") {
			node->children.push_back(new Node(")"));
			t = lex.get_next_lexem();
			pos++;
		}
		else {
			flag = false;
			cout << "the ')' symbol was expected at position " << pos << ", the lexem '" << t.lexema << "' was found" << endl;
		}
	}
	else if (t.lexema == "itof") {
		node->children.push_back(new Node("itof"));
		t = lex.get_next_lexem();
		pos++;
		if (t.lexema == "(") {
			node->children.push_back(new Node("("));
			t = lex.get_next_lexem();
			pos++;

			Expr(t, *node);

			node->type = "float";
			if (node->children[2]->type != "int") {
				out_seman_an << "at position " << pos <<" (function 'itof' argument) expected type 'int'"<< endl;
			}
			node->tr += "itof " + node->children[2]->tr + " CALL";

			if (t.lexema == ")") {
				node->children.push_back(new Node(")"));
				t = lex.get_next_lexem();
				pos++;
			}
			else {
				flag = false;
				cout << "the ')' symbol was expected at position " << pos << ", the lexem '" << t.lexema << "' was found" << endl;
			}
		}
		else {
			flag = false;
			cout << "the '(' symbol was expected at position " << pos << ", the lexem '" << t.lexema << "' was found" << endl;
		}
	}
	else if (t.lexema == "ftoi") {
		node->children.push_back(new Node("ftoi"));
		t = lex.get_next_lexem();
		pos++;
		if (t.lexema == "(") {
			node->children.push_back(new Node("("));
			t = lex.get_next_lexem();
			pos++;

			Expr(t, *node);

			node->type = "int";
			if (node->children[2]->type != "float") {
				out_seman_an << "at position " << pos << " (function 'ftoi' argument) expected type 'float'" << endl;
			}
			node->tr += "ftoi " + node->children[2]->tr + " CALL";

			if (t.lexema == ")") {
				node->children.push_back(new Node(")"));
				t = lex.get_next_lexem();
				pos++;
			}
			else {
				flag = false;
				cout << "the ')' symbol was expected at position " << pos << ", the lexem '" << t.lexema << "' was found" << endl;
			}
		}
		else {
			flag = false;
			cout << "the '(' symbol was expected at position " << pos << ", the lexem '" << t.lexema << "' was found" << endl;
		}
	}
	else {
		flag = false;
		cout << "the id_name or number or '(' sybmol or 'ftoi', 'itof' keyword was expected at position " << pos << ", the lexem '" << t.lexema << "' was found" << endl;
	}
}

void Syntaxor::Id(token& t, Node& n)
{
	Node* node = new Node("Id:");
	n.children.push_back(node);

	if (t.type_lexema == "ID") {
		node->children.push_back(new Node(t.lexema));

		node->tr += t.lexema;
		node->cnt = 1;
		if (!descr_passed) {
			if (find(declared_id.begin(), declared_id.end(), t.lexema) != declared_id.end()) {
				out_seman_an << "repeat declaration variable '" + t.lexema + "' at position " << pos << endl;
			}
			else {
				declared_id.push_back(t.lexema);
				type_decl_id[t.lexema] = n.type;
				node->type = n.type;
			}
		}
		else {
			if (find(declared_id.begin(), declared_id.end(), t.lexema) == declared_id.end()) {
				out_seman_an << "variable '" + t.lexema + "' at position " << pos << " has not been declared"<<endl;
				node->type = "error_type";
			}
			else {
				node->type = type_decl_id[t.lexema];
			}
		}

		t = lex.get_next_lexem();
		pos++;
	}
	else {
		flag = false;
		cout << "the id_name was expected at position " << pos << ", the lexem '" << t.lexema << "' was found" << endl;
	}
}

void Syntaxor::Const(token& t, Node& n)
{
	Node* node = new Node("Const:");
	n.children.push_back(node);
	if (t.type_lexema == "CONST") {
		node->children.push_back(new Node(t.lexema));

		node->tr += t.lexema;
		if (t.lexema.find(".") != -1) {
			node->type = "float";
		}
		else {
			node->type = "int";
		}

		t = lex.get_next_lexem();
		pos++;
	}
	else {
		flag = false;
		cout << "the number was expected at position " << pos << ", the lexem '" << t.lexema << "' was found" << endl;
	}
}