#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::ifstream;
using std::ofstream;
using std::endl;
using std::string;
using std::vector;

vector< vector<int> > getRowDescription(const vector< vector<int> > &field) {
	vector< vector<int> > rowDescription(field.size());

	for (int i = 0; i < field.size(); ++i) {
		int j = 0;

		while (j < field[i].size()) {
			while (j < field[i].size() && field[i][j] == 0) {
				++j;
			}

			int blockSize = 0;
			while (j < field[i].size() && field[i][j] == 1) {
				++blockSize;
				++j;
			}

			if (blockSize > 0) {
				rowDescription[i].push_back(blockSize);
			}
		}
	}
	
	return rowDescription;
}

vector< vector<int> > getColumnDescription(const vector< vector<int> > &field) {
	vector< vector<int> > columnDescription(field[0].size());

	for (int j = 0; j < field[0].size(); ++j) {
		int i = 0;

		while (i < field.size()) {
			while (i < field.size() && field[i][j] == 0) {
				++i;
			}

			int blockSize = 0;
			while (i < field.size() && field[i][j] == 1) {
				++blockSize;
				++i;
			}

			if (blockSize > 0) {
				columnDescription[j].push_back(blockSize);
			}
		}
	}

	return columnDescription;
}

vector< vector<int> > readField() {
	vector< vector<int> > field;

	ifstream file("fromSite.txt");
	if (file.is_open()) {
		int n = 0;
		int m = 0;
		file >> m >> n;

		field.assign(n, vector<int>(m, 0));

		char currentSymbol;

		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m; ++j) {
				file >> currentSymbol;
				if (currentSymbol == '1') {
					field[i][j] = 1;
				}
			}
		}
	}
	file.close();

	return field;
}

void printBlockDescription(ofstream &file, const vector< vector<int> > &description) {
	file << description.size() << std::endl;

	for (int i = 0; i < description.size(); ++i) {
		file << description[i].size() << ' ';

		for (int j = 0; j < description[i].size(); ++j) {
			file << description[i][j] << ' ';
		}

		file << std::endl;
	}
}

void printNonogram(const vector< vector<int> > &field) {
	ofstream file("nonogram.txt");
	if (file.is_open()) {
		printBlockDescription(file, getColumnDescription(field));
		printBlockDescription(file, getRowDescription(field));
	}
	file.close();
}

int main() {
	vector< vector<int> > field = readField();
	printNonogram(field);
	return 0;
}
