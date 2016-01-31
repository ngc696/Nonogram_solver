#include "NonogramSolver.h"

//template <typename Line>
//LineProcessor<Line>::LineProcessor(Line line, int lineSize, const vector<int> &blockSizes)
//	: line_(line)
//	, lineSize_(lineSize)
//	, blockSizes_(blockSizes)
//{}

template <typename Line>
bool NonogramSolver::isCorrectBlockBegin(
	Line line,
	int lineSize,
	const vector<int> &blockSizes,
	int blockIndex,
	const vector<int> &firstCorrectBlockBegin,
	const vector< vector<bool> > &hasEmptyCells,
	const vector< vector<bool> > &hasFilledCells) const
{
	int blockBegin = firstCorrectBlockBegin[blockIndex];
	int blockEnd = blockBegin + blockSizes[blockIndex];

	// ��������, ��� ���� ���������� � �������
	if (blockEnd > lineSize) {
		return false;
	}

	// ��������, ��� �������, � ������� �� ����� ��������� ����, �� �����
	if (hasEmptyCells[blockBegin][blockEnd]) {
		return false;
	}

	// ��������, ��� ����� � ������ �� ����� ��� ������ ������
	if (blockBegin > 0 && line[blockBegin - 1] == FILLED ||
		blockEnd < lineSize && line[blockEnd] == FILLED)
	{
		return false;
	}

	// ��������, ��� ���� ���� - ���������, �� �� ��� �� ��������� ����������� ������
	if (blockIndex + 1 == blockSizes.size() &&
		blockEnd < lineSize &&
		hasFilledCells[blockEnd][lineSize])
	{
		return false;
	}

	return true;
}

template <typename Line>
vector< vector<bool> > NonogramSolver::buildHasEmptyCells(
	Line line,
	int lineSize) 
{
	vector< vector<bool> > hasEmptyCells(lineSize, vector<bool>(lineSize + 1, false));

	for (int i = 0; i < lineSize; ++i) {
		for (int j = i; j < lineSize; ++j) {
			if (i == j) {
				hasEmptyCells[i][j + 1] = (line[j] == EMPTY);
			}
			else if (line[j] == EMPTY) {
				hasEmptyCells[i][j + 1] = true;
			}
			else{
				hasEmptyCells[i][j + 1] = hasEmptyCells[i][j];
			}
		}
	}

	return hasEmptyCells;
}

template <typename Line>
vector< vector<bool> > NonogramSolver::buildHasFilledCells(
	Line line,
	int lineSize) {
	vector< vector<bool> > hasFilledCells(lineSize, vector<bool>(lineSize + 1, false));

	for (int i = 0; i < lineSize; ++i) {
		for (int j = i; j < lineSize; ++j) {
			if (i == j) {
				hasFilledCells[i][j + 1] = (line[j] == FILLED);
			}
			else if (line[j] == FILLED) {
				hasFilledCells[i][j + 1] = true;
			}
			else{
				hasFilledCells[i][j + 1] = hasFilledCells[i][j];
			}
		}
	}

	return hasFilledCells;
}

template <typename Line>
bool NonogramSolver::FindFirstCorrectBlockBegin(
	Line line,
	int lineSize,
	const vector<int> &blockSizes,
	vector<int> &firstCorrectBlockBegin)
{
	firstCorrectBlockBegin.assign(blockSizes.size(), UNDEFINED);

	vector< vector<bool> > hasEmptyCells = buildHasEmptyCells(line, lineSize);
	vector< vector<bool> > hasFilledCells = buildHasFilledCells(line, lineSize);

	int blockIndex = 0;

	// ���� �� ����� ���������� ������� ��� ���� ������
	// �� ���� ����� ������ ��������� �� ����� O(n^2) ��������:
	// ������ �� ������ ��������� ������ ������, � isCorrectPosiotion ����������� �� O(1)
	while (blockIndex < blockSizes.size()) {
		if (blockIndex < 0) {
			return 0;
		}

		if (firstCorrectBlockBegin[blockIndex] == UNDEFINED) {
			firstCorrectBlockBegin[blockIndex] = 0;
		}
		
		// ������ �������� ����� �� ����� ���������� ����� ������ �����������
		if (blockIndex > 0) {
			firstCorrectBlockBegin[blockIndex] = max(
				firstCorrectBlockBegin[blockIndex - 1] + blockSizes[blockIndex - 1] + 1,
				firstCorrectBlockBegin[blockIndex]);
		}

		// ���� �� ������ ���������� ��� ����� ������� ��� �� ������� � ����� ������
		while (firstCorrectBlockBegin[blockIndex] + blockSizes[blockIndex] <= lineSize &&
			!isCorrectBlockBegin(
			line,
			lineSize,
			blockSizes,
			blockIndex, 
			firstCorrectBlockBegin,
			hasEmptyCells,
			hasFilledCells)) 
		{
			++firstCorrectBlockBegin[blockIndex];
		}

		// ��� ���������� �������
		if (firstCorrectBlockBegin[blockIndex] + blockSizes[blockIndex] > lineSize) {
			return false;
		}

		int previousBlockEnd = 0;
		if (blockIndex > 0) {
			previousBlockEnd = firstCorrectBlockBegin[blockIndex - 1] + blockSizes[blockIndex - 1];
		}

		// ��������, ��� ��� ����������� ������ �����
		// ������ ����������� ����� � ������� �������� 
		if (hasFilledCells[previousBlockEnd][firstCorrectBlockBegin[blockIndex]]) {
			--blockIndex;
			++firstCorrectBlockBegin[blockIndex];
			continue;
		}

		++blockIndex;
	}

	return true;
}

template <typename ReversedLine>
bool NonogramSolver::FindLastCorrectBlockBegin(
	ReversedLine reversedLine,
	int lineSize,
	vector<int> blockSizes,
	vector<int> &lastCorrectBlockBegin)
{
	std::reverse(blockSizes.begin(), blockSizes.end());

	if (!FindFirstCorrectBlockBegin(reversedLine, lineSize, blockSizes, lastCorrectBlockBegin)) {
		return false;
	}

	std::reverse(lastCorrectBlockBegin.begin(), lastCorrectBlockBegin.end());

	for (int i = 0; i < lastCorrectBlockBegin.size(); ++i) {
		lastCorrectBlockBegin[i] = lineSize - lastCorrectBlockBegin[i] - blockSizes[i];
	}

	return true;
}

template <typename Line>
void NonogramSolver::FindFilledCells(
	Line line,
	int lineSize,
	const vector<int> &blockSizes,
	const vector<int> &firstSuitablePosition, 
	const vector<int> &lastSuitablePosition) 
{
	// ���� ��� ����� ������ ������ ������� ���������� ��������� � ����� ������ ������
	// ������������, �� ������ � ����������� ����������� ����������� ������� �����.
	for (int blockIndex = 0; blockIndex < blockSizes.size(); ++blockIndex) {
		for (int i = lastSuitablePosition[blockIndex];
			i < firstSuitablePosition[blockIndex] + blockSizes[blockIndex]; ++i) {
			line[i] = FILLED;
		}
	}
}

template <typename Line>
void NonogramSolver::DetermineBlocksAndFillNearCells(
	Line line,
	int lineSize,
	const vector<int> &blockSizes,
	const vector<int> &firstSuitablePosition, 
	const vector<int> &lastSuitablePosition) 
{
	for (int blockBegin = 0; blockBegin < lineSize; ++blockBegin) {
		// ������� ������� �������
		if (line[blockBegin] != FILLED) {
			continue;
		}

		int blockEnd = blockBegin;
		while (blockEnd < lineSize && line[blockEnd] == FILLED) {
			++blockEnd;
		}

		// ������� ��������� � ������� ������ ������
		int previousEmptyCell = blockBegin;
		while (previousEmptyCell >= 0 && line[previousEmptyCell] != EMPTY) {
			--previousEmptyCell;
		}

		int nextEmptyCell = blockEnd;
		while (nextEmptyCell < lineSize && line[nextEmptyCell] != EMPTY) {
			++nextEmptyCell;
		}

		// ������� ������ ����, �������� ����� ������������� �������, ��� �������� ����� ������������ ������
		int firstAvailiableBlock = 0;
		while (firstSuitablePosition[firstAvailiableBlock] + blockSizes[firstAvailiableBlock] < blockEnd) {
			++firstAvailiableBlock;
		}

		// ������� ������ ����, �������� ����� ������������� �������, ��� �������� ������ ������������ ������
		int lastAvialiableBlock = 0;
		while (lastSuitablePosition[lastAvialiableBlock] + blockSizes[lastAvialiableBlock] < blockEnd) {
			++lastAvialiableBlock;
		}

		int NOT_SET = -2;
		int firstEmptyCell = NOT_SET;
		int lastEmptyCell = NOT_SET;

		int rangeToBeFilledBegin = 0;
		int rangeToBeFilledEnd = lineSize;

		// ��� ������� �����, �������� ����� ������������� �������
		for (int block = lastAvialiableBlock; block <= firstAvailiableBlock; ++block) {
			// ������ ����� ����� ���������� ������, �����, ��� ������� ����������� �����
			int firstAvailableBegin = max(firstSuitablePosition[block], blockEnd - blockSizes[block]);
			
			// ���������� ������������ �������
			for (int i = firstAvailableBegin; i < blockBegin; ++i) {
				if (line[i] == EMPTY) {
					firstAvailableBegin = i + 1;
				}
			}

			while (firstAvailableBegin < blockBegin && 
				firstAvailableBegin > 0 && 
				line[firstAvailableBegin - 1] == FILLED) 
			{
				++firstAvailableBegin;
			}

			// ������ ����� ����� ���������� ������, �����, ��� ������� ����������� �����
			int lastAvailableBegin = min(lastSuitablePosition[block], blockBegin);
			
			// ���������� ������������ �������
			for (int i = lastAvailableBegin; i < max(0, blockEnd - blockSizes[block]); ++i) {
				if (line[i] == EMPTY) {
					lastAvailableBegin = i + 1;
				}
			}

			while (lastAvailableBegin + blockSizes[block] < lineSize &&
				line[lastAvailableBegin + blockSizes[block]] == FILLED) {
				--lastAvailableBegin;
			}

			firstAvailableBegin = max(firstAvailableBegin, previousEmptyCell + 1);
			lastAvailableBegin = min(lastAvailableBegin, nextEmptyCell - blockSizes[block]);
			if (lastAvailableBegin < 0) {
				lastAvailableBegin = 0;
			}

			rangeToBeFilledBegin = max(rangeToBeFilledBegin, lastAvailableBegin);
			rangeToBeFilledEnd = min(rangeToBeFilledEnd, firstAvailableBegin + blockSizes[block]);
		}

		for (int i = rangeToBeFilledBegin; i < rangeToBeFilledEnd; ++i) {
			line[i] = FILLED;
		}
	}
}


template <typename Line>
void NonogramSolver::FindEmptyCells(
	Line line,
	int lineSize,
	const vector<int> &blockSizes,
	const vector<int> &firstSuitablePosition,
	const vector<int> &lastSuitablePosition) 
{
	vector<bool> couldBeFilled(lineSize, false);

	for (int blockIndex = 0; blockIndex < blockSizes.size(); ++blockIndex) {
		vector<bool> availableBegin(lineSize, false);

		int blockBegin = firstSuitablePosition[blockIndex];
		// ����� ����� ����� - ��������� ������ �����
		int blockEnd = blockBegin;

		while (blockBegin <= lastSuitablePosition[blockIndex] && blockEnd < lineSize) {
			// ���� ��� �� ����� �����, ���� ����� ��������� ���� ������ �����
			if (blockEnd - blockBegin + 1 < blockSizes[blockIndex]) {
				if (line[blockEnd] == EMPTY) {
					blockBegin = blockEnd + 1;
					blockEnd = blockBegin;
					continue;
				}
				else {
					++blockEnd;
					continue;
				}
			}

			// ���� �����, ��:

			// ��������� ������ ��������� ������
			if (line[blockEnd] == EMPTY) {
				blockBegin = blockEnd + 1;
				blockEnd = blockBegin;
				continue;
			}

			// ������, ����� ������� �������� �����������
			if (blockBegin > 0 && line[blockBegin - 1] == FILLED) {
				++blockBegin;
				++blockEnd;
				continue;
			}

			// ������, �� ������ �������� �����������
			if (blockEnd + 1 < lineSize && line[blockEnd + 1] == FILLED) {
				++blockBegin;
				++blockEnd;
				continue;
			}

			availableBegin[blockBegin] = true;
			++blockBegin;
			++blockEnd;
		}

		int lastFilled = -1;

		// �������� �� ������, � ������� ����� ��������� ���� ����
		// ������ (������) ��� �������� �� O(n^2), ����� �������������� �� O(n)
		for (int i = 0; i < lineSize; ++i) {
			if (availableBegin[i]) {
				for (int j = max(i, lastFilled + 1); j < i + blockSizes[blockIndex]; ++j) {
					couldBeFilled[j] = true;
					lastFilled = j;
				}
			}
		}
	}

	for (int i = 0; i < lineSize; ++i) {
		if (!couldBeFilled[i]) {
			line[i] = EMPTY;
		}
	}
}

template <typename Line>
bool NonogramSolver::ProcessLine(
	Line line,
	int lineSize,
	const vector<int> &blockSizes) 
{
	vector<int> firstCorrectBlockBegin;
	if (!FindFirstCorrectBlockBegin(line, lineSize, blockSizes, firstCorrectBlockBegin)) 
	{
		std::cerr << "Something terrible happend" << std::endl;
	}

	vector<int> lastCorrectBlockBegin;
	if (!FindLastCorrectBlockBegin(line, lineSize, blockSizes, lastCorrectBlockBegin))
	{
		std::cerr << "Something terrible happend" << std::endl;
	}

	FindFilledCells(line, lineSize, blockSizes, firstCorrectBlockBegin, lastCorrectBlockBegin);
	FindEmptyCells(line, lineSize, blockSizes, firstCorrectBlockBegin, lastCorrectBlockBegin);
	DetermineBlocksAndFillNearCells(line, lineSize, blockSizes, firstCorrectBlockBegin, lastCorrectBlockBegin);

	return true;
}

NonogramSolver::NonogramSolver(
	NonogramField &field,
	const vector< vector<int> > &columnBlocksSizes,
	const vector< vector<int> > &rowBlocksSizes)
	: columnBlocksSizes_(columnBlocksSizes)
	, rowBlocksSizes_(rowBlocksSizes)
	, columnCount_(columnBlocksSizes.size())
	, rowCount_(rowBlocksSizes.size())
	, field_(field)
{
	//for (int i = 0; i < columnCount_; ++i) {
	//	columns_.push_back(LineProcessor<Column>(nonogramField_.GetColumn(i), columnBlocksSizes_[i]));
	//}

	//for (int i = 0; i < rowCount_; ++i) {
	//	rows_.push_back(LineProcessor<Row>(nonogramField_.GetRow(i), rowBlocksSizes_[i]));
	//}
}

void NonogramSolver::Solve() {
	bool isFieldChanged;
	do {
		isFieldChanged = false;

		for (int i = 0; i < columnCount_; ++i) {
			if (ProcessLine(field_.ColumnBegin(i), rowCount_, columnBlocksSizes_[i])) {
				isFieldChanged = true;
			}
		}

		for (int i = 0; i < rowCount_; ++i) {
			if (ProcessLine(field_.RowBegin(i), columnCount_, rowBlocksSizes_[i])) {
				isFieldChanged = true;
			}
		}

	} while (isFieldChanged);
}


//template <typename Line>
//void findAcceptableBlocks(
//	Line *Line, 
//	const vector<int> &blockSizes,
//	const vector<int> &firstSuitablePosition,
//	const vector<int> &lastSuitablePosition,
//	const vector<int> &cellBlockNumber)
//{
//	// i > 0 - ������ ����� ���� ������ i-� ������
//	vector< vector<int> > avaliableStates;
//
//	for (int block = 0; block < blockSizes.size(); ++block) {
//		// ������� ������ ���������� ��� ������, ������������� ����� ������� 
//		// �� ������ ����������� ����� � ������� ����� ������� ��������
//		for (int i = previousBlockEnd + 1; i + 1 < firstSuitablePosition[block]; ++i) {
//			avaliableStates[i].insert(-1);
//		}
//
//		for (int blockBegin = firstSuitablePosition[block];
//			blockBegin < lastSuitablePosition[block];
//			++blockBegin)
//		{
//			if (isCorrectPosition(block, blockBegin, -1, Line, blockSizes, cellBlockNumber) {
//				// ������ ����� ������� ����� ����� ���� ������
//				if (blockBegin > 0) {
//					avaliableStates[blockBegin - 1].insert(-1);
//				}
//
//				for (int i = blockBegin; i < blockBegin + blockSizes[block]; ++i) {
//					avaliableStates[i].insert(block);
//				}
//
//				if (blockBegin + blockSizes[block] < Line->size()) {
//					avaliableStates[blockBegin + blockSizes[block]].insert(-1);
//				}
//			}
//			else {
//
//			}
//		}
//	}
//}

//bool isAcceptableMiddle(
//	const vector<NonogramCell> &Line,
//	const vector<int> &cellBlockNumber,
//	int position,
//	int block)
//{
//	return Line[position] != EMPTY &&
//		(Line[position] != FILLED || cellBlockNumber[position] == block);
//}
//
//bool isAcceptableBegin(
//	const vector<NonogramCell> &Line,
//	const vector<int> &cellBlockNumber,
//	int position,
//	int block)
//{
//	return isAcceptableMiddle(Line, cellBlockNumber, position, block) &&
//		(position == 0 || Line[position - 1] != FILLED);
//}
//
//bool isAcceptableEnd(
//	const vector<NonogramCell> &Line,
//	const vector<int> &cellBlockNumber,
//	int position,
//	int block)
//{
//	return isAcceptableMiddle(Line, cellBlockNumber, position, block) &&
//		(position + 1 == Line.size() || Line[position + 1] != FILLED);
//}
//
//// ���� ����� ����������� ������ ������� �����
//void processAnotherBlockCell(
//	const vector<int> &cellBlockNumber,
//	const vector<int> &cellBlockNumber, 
//	int &block,
//	int &i) 
//{
//	
//}
//
//template <typename Line>
//bool LineProcessor<Line>::FindFirstSutablePosition(
//	const vector<NonogramCell> &Line,
//	const vector<int> &blockSizes,
//	vector<int> &firstSuitablePosition)
//{
//	vector<int> 
//}
//
//template <typename Line>
//bool LineProcessor<Line>::FindFirstSutablePosition(
//	const vector<NonogramCell> &Line,
//	const vector<int> &blockSizes,
//	const vector<int> &cellBlockNumber,
//	vector<int> &firstSuitablePosition)
//{
//	firstSuitablePosition.assign(blockSizes.size(), UNDEFINED);
//	// ������ � ������ ������, ������������� �����
//	vector<int> firstBlockCell(blockSizes.size(), UNDEFINED);
//
//	int block = 0;
//	int i = 0;
//
//	while (block < blockSizes.size()) {
//		// ���������� ������ ������
//		while (i < Line.size() && Line[i] == EMPTY) {
//			++i;
//		}
//
//		// �� ������� ������� ��� ����� block
//		if (i == Line.size()) {
//			return false;
//		}
//
//		int avaliableBlockSize = 0;
//		bool breakFlag = false;
//
//		while (avaliableBlockSize < blockSizes[block] && i < Line.size())
//		{
//			if (cellBlockNumber[i] != UNDEFINED && cellBlockNumber[i] != block) {
//				// ������ ���� ����
//				while (block >= 0 && block != cellBlockNumber[i]) {
//					--block;
//				}
//
//				// ���� ������ ����� ���
//				if (block == -1) {
//					return false;
//				}
//
//				// ����� ���������� ����� �������� � ��� ������
//				i = std::max(0, i - blockSizes[block] + 1);
//
//				// ���� �������� ������ �����
//				if (i > firstBlockCell[block]) {
//					return false;
//				}
//
//				breakFlag = true;
//				break;
//			}
//
//			if (Line[i] == EMPTY) {
//				if (firstBlockCell[block] == UNDEFINED) {
//					breakFlag = true;
//					break;
//				}
//				else {
//					return false;
//				}
//			}
//
//			if (cellBlockNumber[i] == cellBlockNumber[block]) {
//				firstBlockCell[block] = i;
//			}
//
//			++avaliableBlockSize;
//			++i;
//		}
//
//		if (breakFlag) {
//			break;
//		}
//
//		if (avaliableBlockSize < blockSizes[block]) {
//			return false;
//		}
//
//
//		while (isAcceptableMiddle(Line, cellBlockNumber, i, block)) {
//			if (isAcceptableBegin(Line, cellBlockNumber, i - blockSizes[block] + 1, block) &&
//				isAcceptableEnd(Line, cellBlockNumber, i, block))
//			{
//				// save position
//				++block;
//				i += 2;
//				break;
//			}
//		}
//	}
//}
