#pragma once
#include <Windows.h>
#include "NonogramCell.h"

class NonogramCellWindow {
public:
	NonogramCellWindow();
	~NonogramCellWindow();

	// Зарегистрировать класс окна
	static bool RegisterClass(HINSTANCE hInstance);

	// Создать экземпляр окна
	bool Create(HINSTANCE hInstance, HWND hWndParent, int positionX, int positionY, int size);

	// Показать окно
	void Show(int cmdShow);

	HWND GetHandle();

	NonogramCell &GetCell();

protected:
	void onDestroy();
	void onLeftClick();
	void onRightClick();
	void onPaint();

	void drawBlackSquare(HDC dc);
	void drawWhiteSquare(HDC dc);
	void drawBlackCross(HDC dc);

private:
	static wchar_t* className;

	HWND handle; // хэндл окна
	HINSTANCE hInstance_;

	NonogramCell cell_;

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};
