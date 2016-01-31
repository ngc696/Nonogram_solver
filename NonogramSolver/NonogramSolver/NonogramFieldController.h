#pragma once

#include "NonogramField.h"
#include "NonogramCell.h"


class NonogramFieldController {
private:
	NonogramField& field_;

public:
	NonogramFieldController(NonogramField &field);

	void SetFieldSize(int rowCount, int columnCount);

	NonogramCell GetCell(int row, int column) const;

	void SetCell(int row, int column, NonogramCell value);

	int GetColumnCount() const;
	int GetRowCount() const;
};
