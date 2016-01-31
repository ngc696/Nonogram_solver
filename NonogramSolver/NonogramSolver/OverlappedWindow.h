#pragma once
#include <Windows.h>
#include "DialogWindow.h"
#include "ColumnDescriptionsWindow.h"
#include "RowDescriptionsWindow.h"
#include "NonogramFieldWindow.h"
#include "NonogramFieldController.h"
#include "ColumnDescriptionController.h"
#include "RowDescriptionController.h"
#include "NonogramSolver.h"
#include "Constants.h"

class OverlappedWindow {
public:
	OverlappedWindow(
		NonogramSolver solver,
		ColumnDescriptionController columnDescriptionController,
		RowDescriptionController rowDescriptionController,
		NonogramFieldController fieldController
	);
	~OverlappedWindow();

	// Зарегистрировать класс окна
	static bool RegisterClass(HINSTANCE hInstance);

	// Создать экземпляр окна
	bool Create(HINSTANCE hInstance, HWND hWndParent);

	// Показать окно
	void Show(int cmdShow);

	HWND GetHandle();
	HWND GetDialogHandle();
	
	void UpdateChildWindowsSizes();

	void SetFieldSizes(int coulumnCount, int rowCount);

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

	DialogWindow dialogWindow_;
	ColumnDescriptionsWindow columnDescriptions_;
	RowDescriptionsWindow rowDescriptions_;
	NonogramFieldWindow fieldWindow_;

	NonogramSolver solver_;
	ColumnDescriptionController columnDescriptionController_;
	RowDescriptionController rowDescriptionController_;
	NonogramFieldController fieldController_;

	int columnCount_;
	int rowCount_;

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};
