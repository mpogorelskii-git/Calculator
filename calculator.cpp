#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Node
{
	Node* left = nullptr;
	Node* right = nullptr;

	int num = 0; // for leaves (integers)
	char op = 0; // for operators (for integers - 0)

	Node(int val) : num(val), op(0) {}
	Node(char oper, Node* l, Node* r) : op(oper), left(l), right(r), num(0) {}
};

// the thing that turns a string into a tree
class Parser {
public:
	Parser() {}
	Node* parse() { return parse_expression(); }

	// turns strings into tokens
	void tokenize(string s)
	{
		tokens.clear();
		int cur = 0;
		for (int i = 0; i < s.size(); i++)
		{
			char c = s[i];

			if (c == ' ')
			{
				continue;
			}

			else if (c == '(' || c == ')' || c == '+' || c == '-' || c == '*' || c == '/')
			{
				if (cur != 0)
				{
					tokens.push_back(Token(cur));
					cur = 0;
				}
				tokens.push_back(Token(c));
			}

			else if (c - '0' >= 0 && c - '0' <= 9)
			{
				cur = cur * 10 + (c - '0');
			}

			else {
				cerr << endl << endl << endl;
				cerr << "Unexpected symbol:" << endl;
				cerr << s << endl;
				for (int q = 0; q < i; q++) cerr << ' ';
				cerr << '^' << endl;
			}
		}

		tokens.push_back(Token(cur));
		return;
	}

private:
	struct Token
	{
		bool is_num;
		int val;
		char op;

		Token(int v) : is_num(true), val(v), op('0') {}
		Token(char o) : is_num(false), val(0), op(o) {}
	};

	vector<Token> tokens;
	int pos = 0;

	// these 3 functions build a tree based on the priority of the operations
	// higher priority => lower branch
	Node* parse_factor()
	{
		Token t = tokens[pos];
		if (t.is_num)
		{
			pos++;
			return new Node(t.val);
		}

		if (t.op == '(')
		{
			pos++;
			Node* node = parse_expression();
			if (tokens[pos].op == ')')
			{
				pos++;
				return node;
			}
		}
		cerr << "Syntax error at token " << pos << endl;
		return nullptr;
	}

	Node* parse_term()
	{
		Node* l = parse_factor();

		while (pos < tokens.size() && (tokens[pos].op == '*' || tokens[pos].op == '/'))
		{
			char op = tokens[pos].op;
			pos++;
			Node* r = parse_factor();
			l = new Node(op, l, r);
		}
		return l;
	}

	Node* parse_expression()
	{
		Node* l = parse_term();

		while (pos < tokens.size() && (tokens[pos].op == '+' || tokens[pos].op == '-'))
		{
			char operand = tokens[pos].op;
			pos++;
			Node* r = parse_term();
			l = new Node(operand, l, r);
		}
		return l;
	}
};

// recursively traverses the tree, extracting values from the leaves and calculating values
int solve(Node* node)
{
	if (node->op == 0)
	{
		return node->num;
	}
	else
	{
		int l = solve(node->left);
		int r = solve(node->right);
		char oper = node->op;

		if (oper == '+') return l + r;
		if (oper == '-') return l - r;
		if (oper == '*') return l * r;
		if (oper == '/') {
			if (r == 0) {
				cerr << "Error: division by zero: " << l << '/' << r << endl;
				return 0;
			}
			return l / r;
		}

		cerr << "Error while calculating" << l << " and " << r << endl;
		cerr << "Operator " << oper << " not recognised" << endl;
		return 0;
	}
}

// for memory management: deleting the whole tree from its root
void deleteTree(Node* node)
{
	if (node == nullptr) return;
	deleteTree(node->left);
	deleteTree(node->right);
	delete node;
}

// for visualising: building a graph based on our expression tree
void printTree(Node* node, int depth = 0, bool isRight = false)
{
	if (!node) return;

	printTree(node->right, depth + 1, true);

	for (int i = 0; i < depth; i++) {
		cout << "    ";
	}

	if (depth > 0) {
		cout << (isRight ? "R-- " : "L-- ");
	}

	if (node->op) {
		cout << node->op << endl;
	}
	else {
		cout << node->num << endl;
	}

	printTree(node->left, depth + 1, false);
}

int main()
{
	bool treeExists = false; Node* globalRoot = nullptr;
	string s;

	cout << "Calcualtor v. 1.1." << endl;
	cout << "Available commands:" << endl;

	while (true)
	{
		cout << "-------------------" << endl;
		cout << "1. Calculate the expression." << endl;
		cout << "2. Visualise the expression." << endl;
		cout << "0. Exit." << endl;
		cout << endl;
		cout << "Enter your choice: ";
		cout.flush();

		int choice; cin >> choice;
		cin.ignore();

		if (choice == 1)
		{
			if (treeExists)
			{
				deleteTree(globalRoot);
				treeExists = false; globalRoot = nullptr;
			}

			cout << "Expression: ";
			getline(cin, s);


			Parser parser;
			parser.tokenize(s);

			Node* root = parser.parse();

			treeExists = true; globalRoot = root;

			cout << solve(root) << endl;
		}

		else if (choice == 2)
		{
			if (!treeExists)
			{
				cout << "Expression: ";
				getline(cin, s);

				Parser parser;
				parser.tokenize(s);

				Node* root = parser.parse();

				treeExists = true; globalRoot = root;

				cout << "Visualising: " << s << endl;
				printTree(globalRoot);
			}

			else
			{
				cout << "Visualising: " << s << endl;
				printTree(globalRoot);
			}
		}

		else if (choice == 0) return 0;

		else
		{
			cerr << "Unknown command" << endl;
		}
	}
}