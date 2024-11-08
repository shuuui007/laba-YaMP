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
	if (flag) tree_node.print(out, "");
	else {
		cout << " -Analysis error-";
		out << " -Analysis error-";
	}
}

void Syntaxor::Function()
{
	tree_node.str = "Function:";
	token t = lex.get_next_lexem();
	pos++;
	Begin(t, tree_node);
	if (flag) Descriptions(t, tree_node);
	if (flag) Operators(t, tree_node);
	if (flag) End(t, tree_node);
}

void Syntaxor::Begin(token& t, Node& n)
{
	Node* node = new Node("Begin:");
	n.children.push_back(node);
	Type(t, *node);
	FuncName(t, *node);
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
		if (t.lexema == ";") {
			node->children.push_back(new Node(";"));
			t = lex.get_next_lexem();
			pos++;
			if (t.lexema == "}") {
				node->children.push_back(new Node("}"));
				t = lex.get_next_lexem();
				pos++;

				if (t.lexema == "" && flag) {
					cout << " ~Analysis was completed successfully~"<<endl;
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
	Id(t, *node);
}

void Syntaxor::Descriptions(token& t, Node& n)
{
	Node* node = new Node("Descriptions:");
	n.children.push_back(node);
	Descr(t, *node);
	if (t.lexema == "int" || t.lexema == "float") {
		Descriptions(t, *node);
	}
}

void Syntaxor::Operators(token& t, Node& n)
{
	Node* node = new Node("Operators:");
	n.children.push_back(node);
	Op(t, *node);
	if (t.type_lexema == "ID") {
		Operators(t, *node);
	}
}

void Syntaxor::Descr(token& t, Node& n)
{
	Node* node = new Node("Descr:");
	n.children.push_back(node);
	Type(t, *node);
	VarList(t, *node);
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
	Id(t, *node);
	if (t.lexema == ",") {
		node->children.push_back(new Node(","));
		t = lex.get_next_lexem();
		pos++;
		VarList(t, *node);
	}
}

void Syntaxor::Type(token& t, Node& n)
{
	Node* node = new Node("Type:");
	n.children.push_back(node);
	if (t.lexema == "int") {
		node->children.push_back(new Node("int"));
		t = lex.get_next_lexem();
		pos++;
	}
	else if (t.lexema == "float") {
		node->children.push_back(new Node("float"));
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
	}
	else {
		flag = false;
		cout << "the '=' symbol was expected at position " << pos << ", the lexem '" << t.lexema << "' was found" << endl;
	}
	Expr(t, *node);
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

void Syntaxor::Expr(token& t, Node& n)
{
	Node* node = new Node("Expr:");
	n.children.push_back(node);
	SimpleExpr(t, *node);
	if (t.lexema == "+") {
		node->children.push_back(new Node("+"));
		t = lex.get_next_lexem();
		pos++;
		Expr(t, *node);
	}
	else if (t.lexema == "-") {
		node->children.push_back(new Node("-"));
		t = lex.get_next_lexem();
		pos++;
		Expr(t, *node);
	}
}

void Syntaxor::SimpleExpr(token& t, Node& n)
{
	Node* node = new Node("SimpleExpr:");
	n.children.push_back(node);
	if (t.type_lexema == "ID") {
		Id(t, *node);
	}
	else if (t.type_lexema == "CONST") {
		Const(t, *node);
	}
	else if (t.lexema == "(") {
		node->children.push_back(new Node("("));
		t = lex.get_next_lexem();
		pos++;
		Expr(t, *node);
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
		}
		else {
			flag = false;
			cout << "the '(' symbol was expected at position " << pos << ", the lexem '" << t.lexema << "' was found" << endl;
		}
		Expr(t, *node);
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
	else if (t.lexema == "ftoi") {
		node->children.push_back(new Node("ftoi"));
		t = lex.get_next_lexem();
		pos++;
		if (t.lexema == "(") {
			node->children.push_back(new Node("("));
			t = lex.get_next_lexem();
			pos++;
		}
		else {
			flag = false;
			cout << "the '(' symbol was expected at position " << pos << ", the lexem '" << t.lexema << "' was found" << endl;
		}
		Expr(t, *node);
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
		cout << "the id_name or number or '(' sybmol or 'ftoi', 'itof' keyword was expected at position " << pos << ", the lexem '" << t.lexema << "' was found" << endl;
	}
}

void Syntaxor::Id(token& t, Node& n)
{
	Node* node = new Node("Id:");
	n.children.push_back(node);
	if (t.type_lexema == "ID") {
		node->children.push_back(new Node(t.lexema));
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
		t = lex.get_next_lexem();
		pos++;
	}
	else {
		flag = false;
		cout << "the number was expected at position " << pos << ", the lexem '" << t.lexema << "' was found" << endl;
	}
}