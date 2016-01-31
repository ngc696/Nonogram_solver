#pragma once
#include <Windows.h>
#include <vector>
#include <deque>
#include <map>
#include "ColumnDescriptionController.h"
#include "Constants.h"

using std::vector;
using std::deque;
using std::map;

class ColumnDescriptionsWindow {
public:
	ColumnDescriptionsWindow(ColumnDescriptionController controller);
	~ColumnDescriptionsWindow();

	// Зарегистрировать класс окна
	static bool RegisterClass(HINSTANCE hInstance);

	// Создать экземпляр окна
	bool Create(HINSTANCE hInstance, HWND hWndParent, int columnCount, int positionX, int positionY);

	// Показать окно
	void Show(int cmdShow);

	void AddRow();
	//void SetColumnCount(int columnCount);

	int Size();

	HWND GetHandle();

	void UpdateWindowFromData();
	void UpdateDataFromWindow();

protected:
	void onDestroy();
	void onResize();
	void onPaint();
	void onEdit(WPARAM wParam, LPARAM lParam);

private:
	static wchar_t* className_;

	HWND handle_; // хэндл окна
	HINSTANCE hInstance_;

	int columnCount_;
	int cellSize_;
	HBRUSH backgroundBrush_;
	
	map<HWND, int> cellColumn_;
	map<HWND, int> cellPosition_;
	vector< vector<HWND> > cells_;

	ColumnDescriptionController controller_;

	void LiftCellsDown();
	void LiftCellsUp();
	
	//vector< vector<int> > GetBlocksSizes();
	//void SetBlocksSizes(const vector< vector<int> > &blockSizes);

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};
