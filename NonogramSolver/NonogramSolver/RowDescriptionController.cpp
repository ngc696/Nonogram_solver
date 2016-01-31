#include "RowDescriptionController.h"

RowDescriptionController::RowDescriptionController(vector< vector<int> > &rowBlockSizes)
	: rowBlockSizes_(rowBlockSizes)
{}

const vector< vector<int> > &RowDescriptionController::GetBlockSizes() const {
	return rowBlockSizes_;
}

void RowDescriptionController::SetBlockSizes(const vector< vector<int> > &blockSizes) {
	rowBlockSizes_ = blockSizes;
}

void RowDescriptionController::SetColumnCount(int columnCount) {
	rowBlockSizes_.resize(columnCount);
}