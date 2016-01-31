#pragma once
#include <Windows.h>
#include "Constants.h"
#include "resource.h"

class DialogWindow {
public:
	// ���������������� ����� ����
	static bool RegisterClass(HINSTANCE hInstance);

	// ������� ��������� ����
	bool Create(HINSTANCE hInstance, HWND hWndParent, int columnCount, int rowCount);

	// �������� ����
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

	HWND handle_; // ����� ����
	
	HINSTANCE hInstance_;

	int columnCount_;
	int rowCount_;

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};
