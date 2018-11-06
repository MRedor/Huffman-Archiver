#include <iostream>
#include <fstream>
#include <string>
#include "huffman.h"

using namespace std;

int main(int argc, char* argv[]) {
	if (argc != 6) {
		cout << "Wrong parameters\n";
		return 0;
	}

	int code = -1;
	string inp, outp;
	bool last = 0;
	for (int i = 1; i < argc; i++) {
		string curArg = string(argv[i]); 
		if (curArg == "-c") {
			if (code == 1) {
				cout << "You can't use both -c and -u?\n";
				return 0;
			}
			code = 0;
			continue;
		}
		if (curArg == "-u") {
			if (code == 0) {
				cout << "You can't use both -c and -u?\n";
				return 0;
			}
			code = 1;
			continue;
		}
		if ((curArg == "-f") || (curArg == "--file")) {
			if (i == argc - 1) {
				cout << "Wrong -f parameter\n";
				return 0;
			}
			inp = argv[i + 1];
			last = 1;
		}
		if ((curArg == "-o") || (curArg == "--output")) {
			if (i == argc - 1) {
				cout << "Wrong -o parameter\n";
				return 0;
			}
			outp = argv[i + 1];
			last = 1;
		}
		if (!last) {
			cout << "Wrong parameters\n";
		}
	}


	ifstream fin;
    fin.open(inp.c_str(), ios::binary);
    if (!fin) {
    	cout << "Wrong input file\n";
    	return 0;
    }

    ofstream fout;
    fout.open(outp.c_str(), ios::binary);

    HuffArchiver arch(fin, fout);

    if (!code) {
    	arch.code();
    } else {
    	arch.setInput(inp);// = inp;
    	arch.decode();
    }
}
