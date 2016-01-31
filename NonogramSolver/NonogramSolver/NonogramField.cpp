#include "NonogramField.h"

NonogramField::NonogramField(int columnCount, int rowCount)
	: columnCount_(columnCount)
	, rowCount_(rowCount)
{
	field_.assign(rowCount_, vector<NonogramCell>(columnCount_, UNDEFINED));
}

bool NonogramField::HasUndefinedCells() const {
	for (int i = 0; i < rowCount_; ++i) {
		for (int j = 0; j < columnCount_; ++j) {
			if (field_[i][j] == UNDEFINED) {
				return true;
			}
		}
	}

	return false;
}

NonogramField::const_column_iterator NonogramField::ColumnCBegin(int column) const {
	return const_column_iterator(&field_, column, 0);
}

NonogramField::column_iterator NonogramField::ColumnBegin(int column) {
	return column_iterator(&field_, column, 0);
}

NonogramField::const_column_iterator NonogramField::ColumnCEnd(int column) const {
	return const_column_iterator(&field_, column, rowCount_);
}

NonogramField::column_iterator NonogramField::ColumnEnd(int column) {
	return column_iterator(&field_, column, rowCount_);
}


NonogramField::const_row_iterator NonogramField::RowBegin(int row) const {
	return field_[row].cbegin();
}

NonogramField::row_iterator NonogramField::RowBegin(int row) {
	return field_[row].begin();
}

NonogramField::const_row_iterator NonogramField::RowEnd(int row) const {
	return field_[row].cend();
}

NonogramField::row_iterator NonogramField::RowEnd(int row) {
	return field_[row].end();
}

NonogramCell NonogramField::GetCell(int row, int column) const {
	return field_[row][column];
}

void NonogramField::SetCell(int row, int column, NonogramCell value) {
	field_[row][column] = value;
}

void NonogramField::SetFieldSize(int rowCount, int columnCount) {
	field_.resize(rowCount);

	for (int i = 0; i < rowCount; ++i) {
		field_[i].resize(columnCount, UNDEFINED);
	}

	rowCount_ = rowCount;
	columnCount_ = columnCount;
}

int NonogramField::GetColumnCount() const {
	if (field_.size() == 0) {
		return 0;
	}

	return field_[0].size();
}

int NonogramField::GetRowCount() const {
	return field_.size();
}

//void NonogramField::UpdatePreviousField() {
//	for (int row = 0; row < rowCount_; ++row) {
//		for (int column = 0; column < columnCount_; ++column) {
//			previousField_[row][column] = field_->at(row)[column]->GetCell();
//		}
//	}
//}
//
//bool NonogramField::WasChanged() {
//	for (int row = 0; row < rowCount_; ++row) {
//		for (int column = 0; column < columnCount_; ++column) {
//			if (field_->at(row)[column]->GetCell() != previousField_[row][column]) {
//				UpdatePreviousField();
//				return true;
//			}
//		}
//	}
//
//	return false;
//}

void NonogramField::Print() const {
	for (int i = 0; i < rowCount_; ++i) {
		for (int j = 0; j < columnCount_; ++j) {
			if (field_[i][j] == FILLED) {
				printf("%c", 219);
			}
			if (field_[i][j] == EMPTY) {
				printf("%c", 32);
			}
			if (field_[i][j] == UNDEFINED) {
				printf("%c", 176);
			}
		}
		printf("\n");
	}

	printf("\n");
}
