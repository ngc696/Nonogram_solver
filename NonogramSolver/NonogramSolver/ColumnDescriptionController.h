#pragma once
#include <vector>

using std::vector;

class ColumnDescriptionController {
private:
	vector< vector<int> > &columnBlockSizes_;

public:
	ColumnDescriptionController(vector< vector<int> > &columnBlockSizes);

	const vector< vector<int> > &GetBlockSizes() const;

	void SetBlockSizes(const vector< vector<int> > &blockSizes);

	void SetRowCount(int rowCount);
};
