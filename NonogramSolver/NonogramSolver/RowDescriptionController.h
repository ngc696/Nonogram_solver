#pragma once
#include <vector>

using std::vector;

class RowDescriptionController {
private:
	vector< vector<int> > &rowBlockSizes_;

public:
	RowDescriptionController(vector< vector<int> > &rowBlockSizes);

	const vector< vector<int> > &GetBlockSizes() const;

	void SetBlockSizes(const vector< vector<int> > &blockSizes);

	void SetColumnCount(int columnCount);
};
