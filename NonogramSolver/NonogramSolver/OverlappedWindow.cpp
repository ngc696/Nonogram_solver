#include "OverlappedWindow.h"
#include "NonogramSolver.h"
#include "resource.h"
#include <string>
#include <cstdlib>
#include <fstream>

wchar_t* OverlappedWindow::className_ = L"ClassOverlappedWindow";

BOOL __stdcall dialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

OverlappedWindow::OverlappedWindow(
	NonogramSolver solver, 
	ColumnDescriptionController columnDescriptionController,
	RowDescriptionController rowDescriptionController,
	NonogramFieldController fieldController
)
	: handle_(0)
	, columnCount_(DEFAULT_COLUMN_COUNT)
	, rowCount_(DEFAULT_ROW_COUNT)
	, columnDescriptions_(columnDescriptionController)
	, rowDescriptions_(rowDescriptionController)
	, fieldWindow_(fieldController)
	, solver_(solver)
	, columnDescriptionController_(columnDescriptionController)
	, rowDescriptionController_(rowDescriptionController)
	, fieldController_(fieldController)
{}

OverlappedWindow::~OverlappedWindow() {}

bool OverlappedWindow::RegisterClass(HINSTANCE hInstance) {
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = windowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON2), IMAGE_ICON, 32, 32, LR_SHARED);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wc.lpszClassName = className_;
	wc.hInstance = hInstance;
	wc.hIconSm = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, LR_SHARED);
	wc.hIconSm = NULL;
	if (!RegisterClassEx(&wc)) {
		LPTSTR lpErrorText = NULL;
		::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			0, GetLastError(), 0, lpErrorText, MAX_PATH, 0);

		MessageBox(NULL, L"Can't register class", L"ERROR!", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	ColumnDescriptionsWindow::RegisterClass(hInstance);
	RowDescriptionsWindow::RegisterClass(hInstance);
	NonogramFieldWindow::RegisterClass(hInstance);

	return true;
}

bool OverlappedWindow::Create(HINSTANCE hInstance, HWND hWndParent) {
	hInstance_ = hInstance;

	handle_ = CreateWindow(className_, NULL, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		hWndParent, NULL, hInstance, this);

	columnDescriptions_.Create(hInstance, handle_, DEFAULT_COLUMN_COUNT, CELL_SIZE, 0);
	rowDescriptions_.Create(hInstance, handle_, DEFAULT_ROW_COUNT, 0, CELL_SIZE);
	fieldWindow_.Create(hInstance, handle_, DEFAULT_COLUMN_COUNT, DEFAULT_ROW_COUNT);

	UpdateChildWindowsSizes();

	return true;
}

//void OverlappedWindow::CreateDialogWindow() {
//	dialog_ = CreateDialog(hInstance_, MAKEINTRESOURCE(IDD_DIALOG1), handle_, dialogProc);
//	if (dialog_ == NULL) {
//		MessageBox(handle_, L"CreateDialog returned NULL", L"Warning!", MB_OK | MB_ICONINFORMATION);
//	}
//
//	if (handle_ == NULL) {
//		MessageBox(NULL, L"Can't got handle", L"ERROR!", MB_OK | MB_ICONEXCLAMATION);
//		return;
//	}
//
//	HWND hColumnCount = GetDlgItem(dialog_, IDC_EDIT1);
//	HWND hRowCount = GetDlgItem(dialog_, IDC_EDIT2);
//
//	std::wstring buf = std::to_wstring(columnCount_);
//	SetWindowText(hColumnCount, buf.c_str());
//
//	buf = std::to_wstring(rowCount_);
//	SetWindowText(hRowCount, buf.c_str());
//
//	ShowWindow(dialog_, SW_SHOWNORMAL);
//}

void OverlappedWindow::Show(int cmdShow) {
	ShowWindow(handle_, cmdShow);
	UpdateWindow(handle_);
}

HWND OverlappedWindow::GetHandle() {
	return handle_;
}

HWND OverlappedWindow::GetDialogHandle() {
	return dialogWindow_.GetHandle();
}

void OverlappedWindow::onDestroy() {
	PostQuitMessage(0);
}

void OverlappedWindow::onResize() {
	RECT rect;

	if (GetClientRect(handle_, &rect)) {
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		//SetWindowPos(textEdit, NULL, 0, 0, width, height, 0);
	}
}

void constructOfn(HWND handle, OPENFILENAME &ofn, LPWSTR fileName) {
	ZeroMemory(&ofn, sizeof(ofn));
	ZeroMemory(fileName, MAX_PATH);

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = handle;
	ofn.lpstrFilter = (LPCWSTR)L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = (LPCWSTR)L"txt";

	
}

void OverlappedWindow::onSave() {
	OPENFILENAME ofn;
	wchar_t fileName[MAX_PATH];
	constructOfn(handle_, ofn, fileName);

	if (GetSaveFileName(&ofn)) {
		std::ofstream out;
		out.open(ofn.lpstrFile);
		
		vector< vector<int> > columnDescriptions = columnDescriptionController_.GetBlockSizes();
		out << columnDescriptions.size() << std::endl;
		for (int column = 0; column < columnDescriptions.size(); ++column) {
			out << columnDescriptions[column].size() << ' ';
			for (int i = 0; i < columnDescriptions[column].size(); ++i) {
				out << columnDescriptions[column][i] << ' ';
			}
			out << std::endl;
		}

		vector< vector<int> > rowDescriptions = rowDescriptionController_.GetBlockSizes();
		out << rowDescriptions.size() << std::endl;
		for (int row = 0; row < rowDescriptions.size(); ++row) {
			out << rowDescriptions[row].size() << ' ';
			for (int i = 0; i < rowDescriptions[row].size(); ++i) {
				out << rowDescriptions[row][i] << ' ';
			}
			out << std::endl;
		}
	}
}

void OverlappedWindow::onOpen() {
	OPENFILENAME ofn;
	wchar_t fileName[MAX_PATH];
	constructOfn(handle_, ofn, fileName);

	if (GetOpenFileName(&ofn)) {
		std::ifstream in;
		in.open(ofn.lpstrFile);

		int columnCount;
		in >> columnCount;
		vector< vector<int> > columnDescriptions(columnCount);
		
		for (int column = 0; column < columnDescriptions.size(); ++column) {
			int blockCount;
			in >> blockCount;

			columnDescriptions[column].resize(blockCount);
			for (int i = 0; i < columnDescriptions[column].size(); ++i) {
				in >> columnDescriptions[column][i];
			}
		}

		int rowCount;
		in >> rowCount;
		vector< vector<int> > rowDescriptions(rowCount);

		for (int row = 0; row < rowDescriptions.size(); ++row) {
			int blockCount;
			in >> blockCount;

			rowDescriptions[row].resize(blockCount);
			for (int i = 0; i < rowDescriptions[row].size(); ++i) {
				in >> rowDescriptions[row][i];
			}
		}

		SetFieldSizes(columnDescriptions.size(), rowDescriptions.size());

		columnDescriptionController_.SetBlockSizes(columnDescriptions);
		rowDescriptionController_.SetBlockSizes(rowDescriptions);

		columnDescriptions_.UpdateWindowFromData();
		rowDescriptions_.UpdateWindowFromData();
		UpdateChildWindowsSizes();
	}
}

void OverlappedWindow::onClose() {
	::DestroyWindow(handle_);
}

void OverlappedWindow::UpdateChildWindowsSizes() {
	SetWindowPos(rowDescriptions_.GetHandle(), NULL, 0, columnDescriptions_.Size() * CELL_SIZE,
		rowDescriptions_.Size() * CELL_SIZE, rowCount_ * CELL_SIZE, NULL);

	SetWindowPos(columnDescriptions_.GetHandle(), NULL, rowDescriptions_.Size() * CELL_SIZE, 0,
		columnCount_ * CELL_SIZE, columnDescriptions_.Size() * CELL_SIZE, NULL);

	SetWindowPos(fieldWindow_.GetHandle(), NULL, rowDescriptions_.Size() * CELL_SIZE,
		columnDescriptions_.Size() * CELL_SIZE, columnCount_ * CELL_SIZE, rowCount_ * CELL_SIZE, NULL);
}

void OverlappedWindow::SetFieldSizes(int columnCount, int rowCount) {
	columnCount_ = columnCount;
	rowCount_ = rowCount;

	columnDescriptionController_.SetRowCount(rowCount);
	rowDescriptionController_.SetColumnCount(columnCount);
	fieldController_.SetFieldSize(rowCount, columnCount);

	columnDescriptions_.UpdateWindowFromData();
	rowDescriptions_.UpdateWindowFromData();
	fieldWindow_.UpdateWindowFromData();
}

void OverlappedWindow::onCommand(WPARAM wParam) {
	switch (LOWORD(wParam)) {
	case ID_FILE_SAVE:
		onSave();
		break;
	case ID_FILE_OPEN:
		onOpen();
		break;
	case ID_FILE_EXIT:
		onClose();
		break;
	case ID_FIELD_SETTINGS:
		dialogWindow_.Create(hInstance_, handle_, columnCount_, rowCount_);
		break;
	case ID_FIELD_ADDCOLUMN:
		columnDescriptions_.AddRow();
		UpdateChildWindowsSizes();
		break;
	case ID_FIELD_ADDROW:
		rowDescriptions_.AddColumn();
		UpdateChildWindowsSizes();
		break;
	case ID_SOLVER_SOLVE:
		solver_.Solve();
		
		RECT rect;
		if (GetClientRect(fieldWindow_.GetHandle(), &rect)) {
			InvalidateRect(fieldWindow_.GetHandle(), &rect, false);
		}
		break;
	}
}

LRESULT __stdcall OverlappedWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
	if (message == WM_NCCREATE) {
		OverlappedWindow* that = reinterpret_cast< OverlappedWindow* >(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams);
		SetWindowLong(handle, GWL_USERDATA, reinterpret_cast<LONG>(that));

		return DefWindowProc(handle, message, wParam, lParam);
	}

	OverlappedWindow* that = reinterpret_cast< OverlappedWindow* >(GetWindowLong(handle, GWL_USERDATA));

	HWND editHandle;

	switch (message) {
	case WM_DESTROY:
		that->onDestroy();
		return 0;
	case WM_SIZE:
		that->onResize();
		return 0;
	case WM_CLOSE:
		that->onClose();
		return 0;
	case WM_COMMAND:
		that->onCommand(wParam);
		return 0;
	}

	return DefWindowProc(handle, message, wParam, lParam);
}

//BOOL __stdcall dialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	OverlappedWindow* that = reinterpret_cast< OverlappedWindow* >(GetWindowLong(GetParent(hwndDlg), GWL_USERDATA));
//
//	if (message == WM_COMMAND) {
//		if (LOWORD(wParam) == IDOK) {
//			HWND hColumnCount = GetDlgItem(that->GetDialogHandle(), IDC_EDIT1);
//			HWND hRowCount = GetDlgItem(that->GetDialogHandle(), IDC_EDIT2);
//
//			LRESULT length = SendMessage(hColumnCount, WM_GETTEXTLENGTH, 0, 0);
//			WCHAR *buffer = new TCHAR[length + 1];
//			SendMessage(hColumnCount, WM_GETTEXT, (WPARAM)length + 1, (LPARAM)buffer);
//
//			int columnCount = _wtoi(buffer);
//			
//			delete[] buffer;
//
//			length = SendMessage(hRowCount, WM_GETTEXTLENGTH, 0, 0);
//			buffer = new TCHAR[length + 1];
//			SendMessage(hRowCount, WM_GETTEXT, (WPARAM)length + 1, (LPARAM)buffer);
//
//			int rowCount = _wtoi(buffer);
//
//			delete[] buffer;
//
//			if (columnCount == 0) {
//				MessageBox(that->GetDialogHandle(), L"Incorrect column count", L"Error", MB_OK);
//			}
//			else if (rowCount == 0) {
//				MessageBox(that->GetDialogHandle(), L"Incorrect row count", L"Error", MB_OK);
//			}
//			else {
//				that->SetFieldSizes(columnCount, rowCount);
//				that->UpdateChildWindowsSizes();
//				DestroyWindow(hwndDlg);
//			}
//
//			return TRUE;
//		}
//		else if (LOWORD(wParam) == IDCANCEL) {
//			DestroyWindow(hwndDlg);
//			return TRUE;
//		}
//	}
//
//	return FALSE;
//}