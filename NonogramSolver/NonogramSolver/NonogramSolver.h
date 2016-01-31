// Решатель японских крссвордов.

#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include "NonogramField.h"

using std::vector;
using std::min;
using std::max;

//template <typename Line>
//class LineProcessor {
//private:
//	Line line_;
//	int lineSize_;
//
//	const vector<int> &blockSizes_;
//
//public:
//	LineProcessor(Line line, int lineSize, const vector<int> &blockSizes);
//
//	
//};


class NonogramSolver {
private:
	const vector< vector<int> > &columnBlocksSizes_;
	const vector< vector<int> > &rowBlocksSizes_;

	NonogramField &field_;

	int columnCount_, rowCount_;

public:
	NonogramSolver(
		NonogramField &field,
		const vector< vector<int> > &columnBlocksSizes,
		const vector< vector<int> > &rowBlocksSizes);

	// Проверка, что блок с номером blockIndex может быть поставлен
	// в строку string начиная с позиции firstCorrectBlockBegin[blockIndex];
	template <typename Line>
	bool isCorrectBlockBegin(
		Line line,
		int lineSize,
		const vector<int> &blockSizes,
		int blockIndex,
		const vector<int> &firstCorrectBlockBegin,
		const vector< vector<bool> > &hasEmptyCells,
		const vector< vector<bool> > &hasFilledCells) const;

	// Строит таблицу, в которой элемент [begin][end] истенен,
	// если на полуинтервале [begin, end) в строке string
	// есть хотябы одна пустая клетка, иначе - ложен.
	template <typename Line>
	vector< vector<bool> > buildHasEmptyCells(Line line, int lineSize);

	// Строит таблицу, в которой элемент [begin][end] истенен,
	// если на полуинтервале [begin, end) в строке string
	// есть хотябы одна заполненная клетка, иначе - ложен.
	template <typename Line>
	vector< vector<bool> > buildHasFilledCells(Line line, int lineSize);

	// Для каждого блока находит первые (самые левые) возможные позиции для 
	// каждого блока, такие, что они не противоречат данным в строке string
	template <typename Line>
	bool FindFirstCorrectBlockBegin(
		Line line,
		int lineSize,
		const vector<int> &blockSizes,
		vector<int> &firstCorrectBlockBegin);

	// Для каждого блока находит последние (самые правые) возможные позиции для 
	// каждого блока, такие, что они не противоречат данным в строке string.
	template <typename ReversedLine>
	bool FindLastCorrectBlockBegin(
		ReversedLine reversedLine,
		int lineSize,
		vector<int> blockSizes,
		vector<int> &lastCorrectBlockBegin);

	// Находит клетки, точно принадлежащие некоторому блоку, и помечает их как заполненные
	template <typename Line>
	void FindFilledCells(
		Line line,
		int lineSize,
		const vector<int> &blockSizes,
		const vector<int> &firstSuitablePosition,
		const vector<int> &lastSuitablePosition);

	// Для каждой последовательности заполненных клеток (отрезков) находит 
	// блоки из условия, к которым она может принадлежать.
	// Далее, по блоку минимальной длины из найденных, пытается заполнить рядом стоящие клетки.
	template <typename Line>
	void DetermineBlocksAndFillNearCells(
		Line line,
		int lineSize,
		const vector<int> &blockSizes,
		const vector<int> &firstSuitablePosition,
		const vector<int> &lastSuitablePosition);

	// Находит клетки, точно не принадлежащие ни одному из блоков, и помечает их как пустые
	template <typename Line>
	void FindEmptyCells(
		Line line,
		int lineSize,
		const vector<int> &blockSizes,
		const vector<int> &firstSuitablePosition,
		const vector<int> &lastSuitablePosition);

	// Пытается определить новые заполненные и пустые клетки.
	// Возвращает true, если были опредены новые клетки.
	// Иначе - false
	template <typename Line>
	bool ProcessLine(
		Line line,
		int lineSize,
		const vector<int> &blockSizes);

	void Solve();

	void PrintField();
};
