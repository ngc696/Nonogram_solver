#include "ColumnDescriptionController.h"

ColumnDescriptionController::ColumnDescriptionController(vector< vector<int> > &columnBlockSizes)
	: columnBlockSizes_(columnBlockSizes)
{}

const vector< vector<int> > &ColumnDescriptionController::GetBlockSizes() const {
	return columnBlockSizes_;
}

void ColumnDescriptionController::SetBlockSizes(const vector< vector<int> > &blockSizes) {
	columnBlockSizes_ = blockSizes;
}

void ColumnDescriptionController::SetRowCount(int rowCount) {
	columnBlockSizes_.resize(rowCount);
}
