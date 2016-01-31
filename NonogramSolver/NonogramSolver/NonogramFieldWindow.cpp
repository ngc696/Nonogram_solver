#include "NonogramFieldWindow.h"
#include "NonogramCellWindow.h"
#include "resource.h"
#include "Constants.h"

wchar_t* NonogramFieldWindow::className_ = L"ClassNonogramFieldWindow";


BOOL __stdcall dialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);


NonogramFieldWindow::NonogramFieldWindow(NonogramFieldController fieldController)
	: handle_(0)
	, cellSize_(CELL_SIZE)
	, fieldController_(fieldController)
{}

NonogramFieldWindow::~NonogramFieldWindow() {}

bool NonogramFieldWindow::RegisterClass(HINSTANCE hInstance) {
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = windowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = className_;
	wc.hInstance = hInstance;
	wc.hIconSm = NULL;
	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, L"Can't register class", L"ERROR!", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	NonogramCellWindow::RegisterClass(hInstance);

	return true;
}

bool NonogramFieldWindow::Create(HINSTANCE hInstance, HWND hWndParent, int columnCount, int rowCount) {
	hInstance_ = hInstance;
	columnCount_ = columnCount;
	rowCount_ = rowCount;
	
	handle_ = CreateWindow(className_, NULL, WS_CHILD | WS_VISIBLE,
		0, 0, columnCount_ * cellSize_, rowCount_ * cellSize_,
		hWndParent, NULL, hInstance, this);

	cellWindows_.resize(rowCount_);

	for (int row = 0; row < rowCount_; ++row) {
		for (int column = 0; column < columnCount_; ++column) {
			cellWindows_[row].push_back(NonogramCellWindow());
			cellWindows_[row][column].Create(hInstance_, handle_, column * cellSize_, row * cellSize_, cellSize_);
		}
	}

	return true;
}

void NonogramFieldWindow::SetFieldSize(int rowCount, int columnCount) {
	if (rowCount >= rowCount_) {
		cellWindows_.resize(rowCount);

		for (int row = rowCount_; row < rowCount; ++row) {
			for (int column = 0; column < columnCount_; ++column) {
				cellWindows_[row].push_back(NonogramCellWindow());
				cellWindows_[row][column].Create(hInstance_, handle_, column * cellSize_, row * cellSize_, cellSize_);
			}
		}
	}
	else {
		for (int row = rowCount; row < rowCount_; ++row) {
			for (int column = 0; column < columnCount_; ++column) {
				//SendMessage(cells_[row][column]->GetHandle(), WM_CLOSE, NULL, NULL);
				if (cellWindows_[row][column].GetHandle() != 0) {
					DestroyWindow(cellWindows_[row][column].GetHandle());
				}
			}
		}

		cellWindows_.resize(rowCount);
	}

	rowCount_ = rowCount;

	if (columnCount >= columnCount_) {
		for (int row = 0; row < rowCount_; ++row) {
			for (int column = columnCount_; column < columnCount; ++column) {
				cellWindows_[row].push_back(NonogramCellWindow());
				cellWindows_[row][column].Create(hInstance_, handle_, column * cellSize_, row * cellSize_, cellSize_);
			}
		}
	}
	else {
		for (int row = 0; row < rowCount_; ++row) {
			for (int column = columnCount; column < columnCount_; ++column) {
				//SendMessage(cells_[row][column]->GetHandle(), WM_CLOSE, NULL, NULL);
				if (cellWindows_[row][column].GetHandle() != 0) {
					DestroyWindow(cellWindows_[row][column].GetHandle());
				}
			}
		}

		for (int row = 0; row < rowCount_; ++row) {
			// Размер вектора уменьшается. Второй аргумент не используется.
			cellWindows_[row].resize(columnCount, NonogramCellWindow());
		}
	}

	columnCount_ = columnCount;
}

void NonogramFieldWindow::UpdateWindowFromData() {
	SetFieldSize(fieldController_.GetColumnCount(), fieldController_.GetRowCount());
}

void NonogramFieldWindow::Show(int cmdShow) {
	ShowWindow(handle_, cmdShow);
	UpdateWindow(handle_);
}

HWND NonogramFieldWindow::GetHandle() {
	return handle_;
}

void NonogramFieldWindow::onDestroy() {
	// PostQuitMessage(0);
}

LRESULT __stdcall NonogramFieldWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
	NonogramFieldWindow* that = reinterpret_cast< NonogramFieldWindow* >(GetWindowLong(handle, GWL_USERDATA));

	if (message == WM_NCCREATE) {
		NonogramFieldWindow* that = reinterpret_cast< NonogramFieldWindow* >(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams);
		SetWindowLong(handle, GWL_USERDATA, reinterpret_cast<LONG>(that));

		return DefWindowProc(handle, message, wParam, lParam);
	}

	switch (message) {
	case WM_DESTROY:
		that->onDestroy();
		return 0;
	}

	return DefWindowProc(handle, message, wParam, lParam);
}
