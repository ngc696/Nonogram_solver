#include "NonogramFieldController.h"


NonogramFieldController::NonogramFieldController(NonogramField &field)
	: field_(field)
{}

void NonogramFieldController::SetFieldSize(int rowCount, int columnCount) {
	field_.SetFieldSize(rowCount, columnCount);
}

NonogramCell NonogramFieldController::GetCell(int row, int column) const {
	return field_.GetCell(row, column);
}

void NonogramFieldController::SetCell(int row, int column, NonogramCell value) {
	field_.SetCell(row, column, value);
}

int NonogramFieldController::GetColumnCount() const {
	return field_.GetColumnCount();
}
int NonogramFieldController::GetRowCount() const {
	return field_.GetRowCount();
}
