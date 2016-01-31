#pragma once
#include <Windows.h>
#include <vector>
#include "NonogramCellWindow.h"
#include "NonogramFieldController.h"

class NonogramFieldWindow {
public:
	NonogramFieldWindow(NonogramFieldController fieldController);
	~NonogramFieldWindow();

	// Зарегистрировать класс окна
	static bool RegisterClass(HINSTANCE hInstance);

	// Создать экземпляр окна
	bool Create(HINSTANCE hInstance, HWND hWndParent, int columnCount, int rowCount);

	void NonogramFieldWindow::UpdateWindowFromData();

	// Показать окно
	void Show(int cmdShow);

	HWND GetHandle();

	// vector< vector<NonogramCellWindow*> > *GetCells();

protected:
	void onDestroy();
	/*void onResize();
	void onLeftClick();
	void onRightClick();
	void onPaint();

	void drawBlackSquare(HDC dc);
	void drawWhiteSquare(HDC dc);
	void drawBlackCross(HDC dc);*/

private:
	static wchar_t* className_;

	HWND handle_; // хэндл окна
	HINSTANCE hInstance_;

	int columnCount_;
	int rowCount_;
	int cellSize_;

	vector< vector<NonogramCellWindow> > cellWindows_;
	NonogramFieldController fieldController_;

	void SetFieldSize(int columnCount, int rowCount);

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};
