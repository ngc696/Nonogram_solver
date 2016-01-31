#pragma once
#include <Windows.h>
#include "Constants.h"
#include "resource.h"

class DialogWindow {
public:
	// Зарегистрировать класс окна
	static bool RegisterClass(HINSTANCE hInstance);

	// Создать экземпляр окна
	bool Create(HINSTANCE hInstance, HWND hWndParent, int columnCount, int rowCount);

	// Показать окно
	void Show(int cmdShow);

	HWND GetHandle();
	
protected:
	void onDestroy();
	void onResize();
	void onSave();
	void onOpen();
	void onClose();
	void onCommand(WPARAM);

private:
	static wchar_t* className_;

	HWND handle_; // хэндл окна
	
	HINSTANCE hInstance_;

	int columnCount_;
	int rowCount_;

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};
