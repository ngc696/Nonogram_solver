#include "ColumnDescriptionsWindow.h"
#include "resource.h"
#include <tchar.h>
#include <stdlib.h>

wchar_t* ColumnDescriptionsWindow::className_ = L"ClassColumnDescriptionsWindow";

ColumnDescriptionsWindow::ColumnDescriptionsWindow(ColumnDescriptionController controller)
	: handle_(0)
	, columnCount_(DEFAULT_COLUMN_COUNT)
	, cellSize_(CELL_SIZE)
	, controller_(controller)
{
	backgroundBrush_ = CreateSolidBrush(EDIT_CELL_COLOR);
}

ColumnDescriptionsWindow::~ColumnDescriptionsWindow() {}

bool ColumnDescriptionsWindow::RegisterClass(HINSTANCE hInstance) {
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

	return true;
}

bool ColumnDescriptionsWindow::Create(HINSTANCE hInstance, HWND hWndParent, int columnCount, int positionX, int positionY) {
	hInstance_ = hInstance;
	columnCount_ = columnCount;
	cells_.resize(columnCount);

	handle_ = CreateWindow(className_, NULL, WS_CHILD | WS_VISIBLE,
		positionX, positionY, columnCount_ * cellSize_ + 2, cellSize_ + 2,
		hWndParent, NULL, hInstance, this);

	for (int column = 0; column < columnCount; ++column) {
		HWND cell = CreateWindow(L"EDIT", NULL,
			WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_CENTER,
			column * cellSize_, 0, cellSize_, cellSize_,
			handle_,         // parent window 
			0, hInstance_, NULL);
		
		cells_[column].push_back(cell);
		cellColumn_[cell] = column;
		cellPosition_[cell] = 0;
	}

	return true;
}

void ColumnDescriptionsWindow::Show(int cmdShow) {
	ShowWindow(handle_, cmdShow);
	UpdateWindow(handle_);
}

HWND ColumnDescriptionsWindow::GetHandle() {
	return handle_;
}

void ColumnDescriptionsWindow::UpdateDataFromWindow() {
	vector< vector<int> > blocksSizes(columnCount_);

	for (int column = 0; column < columnCount_; ++column) {
		for (int i = 0; i < cells_[column].size(); ++i) {
			LRESULT length = SendMessage(cells_[column][i], WM_GETTEXTLENGTH, 0, 0);
			WCHAR *buffer = new TCHAR[length + 1];
			SendMessage(cells_[column][i], WM_GETTEXT, (WPARAM)length + 1, (LPARAM)buffer);

			int value = _wtoi(buffer);

			if (length != 0 && value != 0) {
				blocksSizes[column].push_back(value);
			}
			
			delete[] buffer;
		}

		std::reverse(blocksSizes[column].begin(), blocksSizes[column].end());
	}

	controller_.SetBlockSizes(blocksSizes);
}

void ColumnDescriptionsWindow::UpdateWindowFromData() {
	const vector< vector<int> > &blockSizes = controller_.GetBlockSizes();

	int maxSize = 0;
	for (int i = 0; i < blockSizes.size(); ++i) {
		if (blockSizes[i].size() > maxSize) {
			maxSize = blockSizes[i].size();
		}
	}

	for (int i = cells_[0].size(); i < maxSize; ++i) {
		AddRow();
	}

	if (blockSizes.size() >= columnCount_) {
		cells_.resize(blockSizes.size());
		for (int column = columnCount_; column < blockSizes.size(); ++column) {
			for (int i = 0; i < cells_[0].size(); ++i) {
				int yPosition = (cells_[0].size() - i - 1) * cellSize_;
				HWND cell = CreateWindow(L"EDIT", NULL,
					WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_CENTER,
					column * cellSize_, yPosition, cellSize_, cellSize_,
					handle_,         // parent window 
					0, hInstance_, NULL);
		
				cells_[column].push_back(cell);
				cellColumn_[cell] = column;
				cellPosition_[cell] = i;
			}
		}
	} 
	else {
		for (int column = blockSizes.size(); column < columnCount_; ++column) {
			for (int i = 0; i < cells_[0].size(); ++i) {
				cellColumn_.erase(cells_[column][i]);
				cellPosition_.erase(cells_[column][i]);
				DestroyWindow(cells_[column][i]);
			}
		}
		
		cells_.resize(blockSizes.size());
	}
		
	columnCount_ = blockSizes.size();
	
	WCHAR buffer[MAX_PATH];

	for (int column = 0; column < columnCount_; ++column) {
		for (int i = 0; i < cells_[column].size(); ++i) {
			SetWindowText(cells_[column][i], L"");
		}
	}

	for (int column = 0; column < blockSizes.size(); ++column) {
		for (int i = 0; i < blockSizes[column].size(); ++i) {
			_itow_s(blockSizes[column][blockSizes[column].size() - i - 1], buffer, MAX_PATH, 10);
			SetWindowText(cells_[column][i], buffer);
		}
	}
}

int ColumnDescriptionsWindow::Size() {
	return cells_[0].size();
}

void ColumnDescriptionsWindow::AddRow() {
	LiftCellsDown();

	for (int column = 0; column < cells_.size(); ++column) {
		HWND cell = CreateWindow(L"EDIT", NULL,
			WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_CENTER,
			column * cellSize_, 0, cellSize_, cellSize_,
			handle_,         // parent window 
			0, hInstance_, NULL);

		cells_[column].push_back(cell);
		cellColumn_[cell] = column;
		cellPosition_[cell] = 0;
	}
}

void ColumnDescriptionsWindow::LiftCellsDown() {
	for (int column = 0; column < cells_.size(); ++column) {
		for (int i = 0; i < cells_[column].size(); ++i) {
			int yPosition = (cells_[column].size() - i) * cellSize_;
			SetWindowPos(cells_[column][i], NULL, column * cellSize_, yPosition, cellSize_, cellSize_, NULL);
		}
	}
}

void ColumnDescriptionsWindow::onDestroy() {}

void ColumnDescriptionsWindow::onEdit(WPARAM wParam, LPARAM lParam) {
	if (HIWORD(wParam) == EN_KILLFOCUS) {
		HWND editHandle = (HWND)lParam;

		if (cellPosition_.find(editHandle) == cellPosition_.end()) {
			return;
		}

		LRESULT length = SendMessage(editHandle, WM_GETTEXTLENGTH, 0, 0);
		WCHAR *buffer = new TCHAR[length + 1];
		SendMessage(editHandle, WM_GETTEXT, (WPARAM)length + 1, (LPARAM)buffer);

		int newValue = _wtoi(buffer);

		if (length == 0 || newValue == 0) {
			SetWindowText(editHandle, L"");
		}
		else {
			_itow_s(newValue, buffer, length + 1, 10);
			SetWindowText(editHandle, buffer);
		}

		delete[] buffer;

		UpdateDataFromWindow();

		return;
	}
}

//void ColumnDescriptionsWindow::SetColumnCount(int columnCount) {
//	if (columnCount >= columnCount_) {
//		cells_.resize(columnCount);
//		for (int column = columnCount_; column < columnCount; ++column) {
//			for (int i = 0; i < cells_[0].size(); ++i) {
//				int yPosition = (cells_[0].size() - i - 1) * cellSize_;
//				HWND cell = CreateWindow(L"EDIT", NULL,
//					WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_CENTER,
//					column * cellSize_, yPosition, cellSize_, cellSize_,
//					handle_,         // parent window 
//					0, hInstance_, NULL);
//
//				cells_[column].push_back(cell);
//				cellColumn_[cell] = column;
//				cellPosition_[cell] = i;
//			}
//		}
//	} 
//	else {
//		for (int column = columnCount; column < columnCount_; ++column) {
//			for (int i = 0; i < cells_[0].size(); ++i) {
//				cellColumn_.erase(cells_[column][i]);
//				cellPosition_.erase(cells_[column][i]);
//				DestroyWindow(cells_[column][i]);
//			}
//		}
//
//		cells_.resize(columnCount);
//	}
//
//	columnCount_ = columnCount;
//}

LRESULT __stdcall ColumnDescriptionsWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
	ColumnDescriptionsWindow* that = reinterpret_cast< ColumnDescriptionsWindow* >(GetWindowLong(handle, GWL_USERDATA));

	if (message == WM_NCCREATE) {
		ColumnDescriptionsWindow* that = reinterpret_cast< ColumnDescriptionsWindow* >(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams);
		SetWindowLong(handle, GWL_USERDATA, reinterpret_cast<LONG>(that));

		return DefWindowProc(handle, message, wParam, lParam);
	}

	switch (message) {
	case WM_DESTROY:
		that->onDestroy();
		return 0;
	case WM_COMMAND:
		that->onEdit(wParam, lParam);
		return 0;
	case WM_CTLCOLOREDIT:
		SetBkColor((HDC)wParam, EDIT_CELL_COLOR);
		return (LRESULT)that->backgroundBrush_;
	}

	return DefWindowProc(handle, message, wParam, lParam);
}
