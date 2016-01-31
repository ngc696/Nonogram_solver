// �������� �������� ����������.

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

	// ��������, ��� ���� � ������� blockIndex ����� ���� ���������
	// � ������ string ������� � ������� firstCorrectBlockBegin[blockIndex];
	template <typename Line>
	bool isCorrectBlockBegin(
		Line line,
		int lineSize,
		const vector<int> &blockSizes,
		int blockIndex,
		const vector<int> &firstCorrectBlockBegin,
		const vector< vector<bool> > &hasEmptyCells,
		const vector< vector<bool> > &hasFilledCells) const;

	// ������ �������, � ������� ������� [begin][end] �������,
	// ���� �� ������������� [begin, end) � ������ string
	// ���� ������ ���� ������ ������, ����� - �����.
	template <typename Line>
	vector< vector<bool> > buildHasEmptyCells(Line line, int lineSize);

	// ������ �������, � ������� ������� [begin][end] �������,
	// ���� �� ������������� [begin, end) � ������ string
	// ���� ������ ���� ����������� ������, ����� - �����.
	template <typename Line>
	vector< vector<bool> > buildHasFilledCells(Line line, int lineSize);

	// ��� ������� ����� ������� ������ (����� �����) ��������� ������� ��� 
	// ������� �����, �����, ��� ��� �� ������������ ������ � ������ string
	template <typename Line>
	bool FindFirstCorrectBlockBegin(
		Line line,
		int lineSize,
		const vector<int> &blockSizes,
		vector<int> &firstCorrectBlockBegin);

	// ��� ������� ����� ������� ��������� (����� ������) ��������� ������� ��� 
	// ������� �����, �����, ��� ��� �� ������������ ������ � ������ string.
	template <typename ReversedLine>
	bool FindLastCorrectBlockBegin(
		ReversedLine reversedLine,
		int lineSize,
		vector<int> blockSizes,
		vector<int> &lastCorrectBlockBegin);

	// ������� ������, ����� ������������� ���������� �����, � �������� �� ��� �����������
	template <typename Line>
	void FindFilledCells(
		Line line,
		int lineSize,
		const vector<int> &blockSizes,
		const vector<int> &firstSuitablePosition,
		const vector<int> &lastSuitablePosition);

	// ��� ������ ������������������ ����������� ������ (��������) ������� 
	// ����� �� �������, � ������� ��� ����� ������������.
	// �����, �� ����� ����������� ����� �� ���������, �������� ��������� ����� ������� ������.
	template <typename Line>
	void DetermineBlocksAndFillNearCells(
		Line line,
		int lineSize,
		const vector<int> &blockSizes,
		const vector<int> &firstSuitablePosition,
		const vector<int> &lastSuitablePosition);

	// ������� ������, ����� �� ������������� �� ������ �� ������, � �������� �� ��� ������
	template <typename Line>
	void FindEmptyCells(
		Line line,
		int lineSize,
		const vector<int> &blockSizes,
		const vector<int> &firstSuitablePosition,
		const vector<int> &lastSuitablePosition);

	// �������� ���������� ����� ����������� � ������ ������.
	// ���������� true, ���� ���� �������� ����� ������.
	// ����� - false
	template <typename Line>
	bool ProcessLine(
		Line line,
		int lineSize,
		const vector<int> &blockSizes);

	void Solve();

	void PrintField();
};
