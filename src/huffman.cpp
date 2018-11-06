#include "huffman.h"
#include <unordered_map>
#include <queue>
#include "stdint.h"

using namespace std;

int code[1000];
int curSize = 0;
vector<int> codes[1000];
int nums[1000];
int theNumberOfAlph = 0, theNumberOfLinks = 0, theNumberOfSymbols = 0;
int subInf = 0, newSize = 0, firstSize = 0;



void HuffTree::calcCodeTree(TreeNode* v) {
	bool leaf = 1;
	if (v != v->l) {
		theNumberOfLinks++;
		leaf = 0;
		code[curSize] = 0;
		curSize++;
		calcCodeTree(v->l);
	}
	if (v != v->r) {
		theNumberOfLinks++;
		leaf = 0;
		code[curSize] = 1;
		curSize++;
		calcCodeTree(v->r);
	}
	if (leaf) {
		for (int i = 0; i < curSize; i++) {
			codes[theNumberOfAlph].push_back(code[i]);
		}
		nums[v->key + 200] = theNumberOfAlph;

		theNumberOfAlph++;
	}
	curSize--;
	theNumberOfLinks++;
}

HuffArchiver::HuffArchiver(ifstream& fin, ofstream& fout) : fin(fin), fout(fout) {}

void HuffArchiver::setInput(const string inp) {
	inputname = inp;
}

void HuffArchiver::code() {
	char c;
	unordered_map<char, int> mymap;

	while (!fin.eof()) {
		fin.read(&c, sizeof(char));
		firstSize += sizeof(c);
		mymap[c]++;
		theNumberOfSymbols++;
	}
	firstSize -= sizeof(c);
	theNumberOfSymbols--;

	priority_queue< pair<int, TreeNode*> > q;

	int cnt = 0;
	for (auto i : mymap) {
		cnt++;
		q.push( make_pair( -i.second, new TreeNode(i.first) ) );	
	}

	for (int i = 0; i < cnt - 1; i++) {
		auto fst = q.top();
		q.pop();
		auto scd = q.top();
		q.pop();
		q.push( make_pair( fst.first + scd.first, new TreeNode(0, fst.second, scd.second) ) );
	}

	
	auto t = q.top();
	HuffTree tree(t.second);

	q.pop();

	tree.calcCodeTree(tree.getRoot());

	fin.clear();
	fin.seekg(0, ios::beg);
	

	fout.write((char*)&theNumberOfAlph, sizeof(theNumberOfAlph));
	subInf += sizeof(theNumberOfAlph);
	fout.write((char*)&theNumberOfLinks, sizeof(theNumberOfLinks));
	subInf += sizeof(theNumberOfLinks);
	fout.write((char*)&theNumberOfSymbols, sizeof(theNumberOfSymbols));
	subInf += sizeof(theNumberOfSymbols);

	tree.printAlphabet(tree.getRoot(), fout);
	tree.printTree(tree.getRoot(), fout);
	char curr = 0;
	int eight = 0;
	while (!fin.eof()) {
		fin.read(&c, sizeof(char));
		int id = nums[c + 200];
		for (int i = 0; i < (int)codes[id].size(); i++) {
			eight++;
			curr = (curr << 1) | (codes[id][i]);
			if (eight == 8*(int)sizeof(char)) {
				newSize += sizeof(curr);
				fout.write(&curr, sizeof(curr));
				curr = 0;
				eight = 0;
			}
		}
	}
	while (eight < 8*(int)sizeof(char)) {
		curr = curr << 1;
		eight++;
	}
	newSize += sizeof(curr);
	fout.write(&curr, sizeof(curr));

	cerr << firstSize << "\n" << newSize << "\n" << subInf << "\n";
}

char alph[1000];
char path[1000000];

void readTree(ifstream& fin, TreeNode* v) {
	if (!theNumberOfLinks) {
		return;
	}
	char c = path[theNumberOfLinks];
	theNumberOfLinks--;
	int leaf = 1;
	if (c == 'L') {
		leaf = 0;
		v->l = new TreeNode(0);
		readTree(fin, v->l);
		if (!theNumberOfLinks) {
			return;
		}
		c = path[theNumberOfLinks];
		theNumberOfLinks--;
	}
	if (c == 'R') {
		leaf = 0;
		v->r = new TreeNode(0);
		readTree(fin, v->r);
		if (!theNumberOfLinks) {
			return;
		}
		c = path[theNumberOfLinks];
		theNumberOfLinks--;
	}
	if ((c == 'U') && (leaf)) {
		v->key = alph[curSize];
		curSize++;
	}
}


void HuffArchiver::decode() {
	fin.close();
	fin.open(inputname.c_str(), ios::binary);
	
	fin.read((char*)&theNumberOfAlph, sizeof(int));
	fin.read((char*)&theNumberOfLinks, sizeof(int));
	fin.read((char*)&theNumberOfSymbols, sizeof(int));

	subInf += 3 * sizeof(int);

	char x;
	for (int i = 0; i < theNumberOfAlph; i++) {
		fin.read(&x, sizeof(char));
		alph[i] = x;
		//cerr << x << ' ';
	}
	subInf += theNumberOfAlph * sizeof(char);

	for (int i = theNumberOfLinks; i > 0; i--) {
		fin.read(&x, sizeof(char));
		path[i] = x;
	}
	subInf += theNumberOfLinks * sizeof(char);

	TreeNode* t = new TreeNode(0);
	readTree(fin, t);
	curSize = 0;
	HuffTree tree(t);
	tree.calcCodeTree(tree.getRoot());
	newSize = theNumberOfSymbols * sizeof(char);
	firstSize -= sizeof(char);
	while (!fin.eof()) {
		fin.read(&x, sizeof(char));
		firstSize += sizeof(char);
		for (int i = 8 * sizeof(char) - 1; i > -1; i--) {
			int d = (x >> i) & (1);
			if (d == 0) {
				if (t->l != t) {
					t = t->l;
				} else {
					if (!theNumberOfSymbols) {
						break;
					}
					theNumberOfSymbols--;
					fout.write(&(t->key), sizeof(t->key));
					t = tree.getRoot();
					t = t->l;
				}
			} else {
				if (t->r != t) {
					t = t->r;
				} else {
					if (!theNumberOfSymbols) {
						break;
					}
					theNumberOfSymbols--;
					fout.write(&(t->key), sizeof(t->key));
					t = tree.getRoot();
					t = t->r;
				}
			}
		}
	}
	if (theNumberOfSymbols) {
		fout.write(&(t->key), sizeof(t->key));
	}
	cerr << firstSize << "\n" << newSize << "\n" << subInf;
}

TreeNode::TreeNode(char key) : key(key) {
	l = r = this;
}

TreeNode* HuffTree::getRoot() {
	return root;
}

TreeNode::TreeNode(char key, TreeNode* l, TreeNode* r) : key(key), l(l), r(r) {
}

void HuffTree::deleteTree(TreeNode* v) {
	if (v->l != v) {
		deleteTree(v->l);
	}
	if (v->r != v) {
		deleteTree(v->r);
	}
	delete v;
}

HuffTree::~HuffTree() {
	deleteTree(root);
}

HuffTree::HuffTree() {
	root = 0;
}

HuffTree::HuffTree(TreeNode* v) {
	root = v;
}

char ll='L', rr='R', uu='U';
void HuffTree::printTree(TreeNode* v, ofstream& fout) {
	if (v != v->l) {
		fout.write(&ll, sizeof(ll));
		subInf += sizeof(ll);
		printTree(v->l, fout);
	}
	if (v != v->r) {
		subInf += sizeof(rr);
		fout.write(&rr, sizeof(rr));
		printTree(v->r, fout);
	}
	subInf += sizeof(uu);
	fout.write(&uu, sizeof(uu));
}

void HuffTree::printAlphabet(TreeNode* v, ofstream& fout) {
	int leaf = 1;
	if (v != v->l) {
		leaf = 0;
		printAlphabet(v->l, fout);
	}
	if (v != v->r) {
		leaf = 0;
		printAlphabet(v->r, fout);
	}
	if (!leaf) {
		return;
	}
	int k = v->key;
	subInf += sizeof(v->key);
	fout.write((char*)&k, sizeof(v->key));
}

