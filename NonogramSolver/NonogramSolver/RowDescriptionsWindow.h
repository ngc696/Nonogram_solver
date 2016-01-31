#pragma once
#include <Windows.h>
#include <vector>
#include <deque>
#include <map>
#include "RowDescriptionController.h"
#include "Constants.h"

using std::vector;
using std::deque;
using std::map;

class RowDescriptionsWindow {
public:
	RowDescriptionsWindow(RowDescriptionController controller);
	~RowDescriptionsWindow();

	// Зарегистрировать класс окна
	static bool RegisterClass(HINSTANCE hInstance);

	// Создать экземпляр окна
	bool Create(HINSTANCE hInstance, HWND hWndParent, int rowCount, int positionX, int positionY);

	// Показать окно
	void Show(int cmdShow);

	void AddColumn();
	void SetRowCount(int rowCount);

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
	static wchar_t* className;

	HWND handle; // хэндл окна
	HINSTANCE hInstance_;

	int rowCount_;
	int cellSize_;
	HBRUSH backgroundBrush_;

	map<HWND, int> cellRow_;
	map<HWND, int> cellPosition_;
	vector< vector<HWND> > cells_;

	RowDescriptionController controller_;
	
	void MoveCellsLeft();
	void MoveCellsRight();

	//vector< vector<int> > GetBlocksSizes();
	//void SetBlocksSizes(const vector< vector<int> > &blockSizes);

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};
