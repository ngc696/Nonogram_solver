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

	// Проверка, что блок помещается в строчку
	if (blockEnd > lineSize) {
		return false;
	}

	// Проверка, что позиции, в которые мы хотим поставить блок, не пусты
	if (hasEmptyCells[blockBegin][blockEnd]) {
		return false;
	}

	// Проверка, что слева и справа от блока нет пустых клеток
	if (blockBegin > 0 && line[blockBegin - 1] == FILLED ||
		blockEnd < lineSize && line[blockEnd] == FILLED)
	{
		return false;
	}

	// Проверка, что если блок - последний, то за ним не останется заполненных клеток
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

	// Пока не нашли подходящие позиции для всех блоков
	// На этом этапе должно произойти не более O(n^2) операций:
	// Каждый из блоков сдигается только вперед, а isCorrectPosiotion выполняется за O(1)
	while (blockIndex < blockSizes.size()) {
		if (blockIndex < 0) {
			return 0;
		}

		if (firstCorrectBlockBegin[blockIndex] == UNDEFINED) {
			firstCorrectBlockBegin[blockIndex] = 0;
		}
		
		// начало текущего блока не может находиться перед концом предыдущего
		if (blockIndex > 0) {
			firstCorrectBlockBegin[blockIndex] = max(
				firstCorrectBlockBegin[blockIndex - 1] + blockSizes[blockIndex - 1] + 1,
				firstCorrectBlockBegin[blockIndex]);
		}

		// пока не найдем допустимой для блока позиции или не упремся в конец строки
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

		// нет корректных позиций
		if (firstCorrectBlockBegin[blockIndex] + blockSizes[blockIndex] > lineSize) {
			return false;
		}

		int previousBlockEnd = 0;
		if (blockIndex > 0) {
			previousBlockEnd = firstCorrectBlockBegin[blockIndex - 1] + blockSizes[blockIndex - 1];
		}

		// Проверка, что нет заполненных клеток между
		// концом предыдущего блока и началом текущего 
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
	// Если для блока начало самого правого доступного положения и конец самого левого
	// пересекаются, то клетки в пересечении обязательно принадлежат данному блоку.
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
		// Находим границы отрезка
		if (line[blockBegin] != FILLED) {
			continue;
		}

		int blockEnd = blockBegin;
		while (blockEnd < lineSize && line[blockEnd] == FILLED) {
			++blockEnd;
		}

		// Находим ближайшие к отрезку пустые клетки
		int previousEmptyCell = blockBegin;
		while (previousEmptyCell >= 0 && line[previousEmptyCell] != EMPTY) {
			--previousEmptyCell;
		}

		int nextEmptyCell = blockEnd;
		while (nextEmptyCell < lineSize && line[nextEmptyCell] != EMPTY) {
			++nextEmptyCell;
		}

		// Находим первый блок, которому может принадленжать отрезок, при наиболее левом расположении блоков
		int firstAvailiableBlock = 0;
		while (firstSuitablePosition[firstAvailiableBlock] + blockSizes[firstAvailiableBlock] < blockEnd) {
			++firstAvailiableBlock;
		}

		// Находим первый блок, которому может принадленжать отрезок, при наиболее правом расположении блоков
		int lastAvialiableBlock = 0;
		while (lastSuitablePosition[lastAvialiableBlock] + blockSizes[lastAvialiableBlock] < blockEnd) {
			++lastAvialiableBlock;
		}

		int NOT_SET = -2;
		int firstEmptyCell = NOT_SET;
		int lastEmptyCell = NOT_SET;

		int rangeToBeFilledBegin = 0;
		int rangeToBeFilledEnd = lineSize;

		// Для каждого блока, которому может принадленжать отрезок
		for (int block = lastAvialiableBlock; block <= firstAvailiableBlock; ++block) {
			// найдем самое левое корректное начало, такое, что отрезок принадлежит блоку
			int firstAvailableBegin = max(firstSuitablePosition[block], blockEnd - blockSizes[block]);
			
			// пропускаем некорректные позиции
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

			// найдем самое левое корректное начало, такое, что отрезок принадлежит блоку
			int lastAvailableBegin = min(lastSuitablePosition[block], blockBegin);
			
			// пропускаем некорректные позиции
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
		// здесь конец блока - последняя клетка блока
		int blockEnd = blockBegin;

		while (blockBegin <= lastSuitablePosition[blockIndex] && blockEnd < lineSize) {
			// Если еще не нашли место, куда можно поставить блок нужной длины
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

			// Если нашли, но:

			// Последняя клетка оказалась пустой
			if (line[blockEnd] == EMPTY) {
				blockBegin = blockEnd + 1;
				blockEnd = blockBegin;
				continue;
			}

			// Клетка, перед началом оказалсь заполненной
			if (blockBegin > 0 && line[blockBegin - 1] == FILLED) {
				++blockBegin;
				++blockEnd;
				continue;
			}

			// Клетка, за концом оказалсь заполненной
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

		// Помечаем те клетки, в которые может заполнить этот блок
		// Сейчас (похоже) это делается за O(n^2), можно оптимизировать до O(n)
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
//	// i > 0 - клетка может быть занята i-м блоком
//	vector< vector<int> > avaliableStates;
//
//	for (int block = 0; block < blockSizes.size(); ++block) {
//		// Добавим пустые сосотояния для клеток, расположенных между клеткой 
//		// за концом предыдущего блока и клеткой перед началом текущего
//		for (int i = previousBlockEnd + 1; i + 1 < firstSuitablePosition[block]; ++i) {
//			avaliableStates[i].insert(-1);
//		}
//
//		for (int blockBegin = firstSuitablePosition[block];
//			blockBegin < lastSuitablePosition[block];
//			++blockBegin)
//		{
//			if (isCorrectPosition(block, blockBegin, -1, Line, blockSizes, cellBlockNumber) {
//				// Клетка перед началом блока может быть пустой
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
//// если нашли заполненную клетку другого блока
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
//	// первая в строке клетка, принадлежащая блоку
//	vector<int> firstBlockCell(blockSizes.size(), UNDEFINED);
//
//	int block = 0;
//	int i = 0;
//
//	while (block < blockSizes.size()) {
//		// пропускаем пустые клетки
//		while (i < Line.size() && Line[i] == EMPTY) {
//			++i;
//		}
//
//		// не найдено позиции для блока block
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
//				// найдем этот блок
//				while (block >= 0 && block != cellBlockNumber[i]) {
//					--block;
//				}
//
//				// если такого блока нет
//				if (block == -1) {
//					return false;
//				}
//
//				// конец найденного блока поместим в эту клетке
//				i = std::max(0, i - blockSizes[block] + 1);
//
//				// если пропусли клетку блока
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
