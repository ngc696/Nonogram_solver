#include "DialogWindow.h"
#include "OverlappedWindow.h"
#include <string>
#include <cstdlib>
#include <fstream>

BOOL __stdcall dialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

bool DialogWindow::Create(HINSTANCE hInstance, HWND parentHandle, int columnCount, int rowCount) {
	handle_ = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), parentHandle, dialogProc);
	if (handle_ == NULL) {
		return false;
	}

	HWND hColumnCount = GetDlgItem(handle_, IDC_EDIT1);
	HWND hRowCount = GetDlgItem(handle_, IDC_EDIT2);

	std::wstring buf = std::to_wstring(columnCount);
	SetWindowText(hColumnCount, buf.c_str());

	buf = std::to_wstring(rowCount);
	SetWindowText(hRowCount, buf.c_str());

	ShowWindow(handle_, SW_SHOWNORMAL);

	return true;
}

HWND DialogWindow::GetHandle() {
	return handle_;
}

BOOL __stdcall dialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	OverlappedWindow* that = reinterpret_cast< OverlappedWindow* >(GetWindowLong(GetParent(hwndDlg), GWL_USERDATA));

	if (message == WM_COMMAND) {
		if (LOWORD(wParam) == IDOK) {
			HWND hColumnCount = GetDlgItem(hwndDlg, IDC_EDIT1);
			HWND hRowCount = GetDlgItem(hwndDlg, IDC_EDIT2);

			LRESULT length = SendMessage(hColumnCount, WM_GETTEXTLENGTH, 0, 0);
			WCHAR *buffer = new TCHAR[length + 1];
			SendMessage(hColumnCount, WM_GETTEXT, (WPARAM)length + 1, (LPARAM)buffer);

			int columnCount = _wtoi(buffer);

			delete[] buffer;

			length = SendMessage(hRowCount, WM_GETTEXTLENGTH, 0, 0);
			buffer = new TCHAR[length + 1];
			SendMessage(hRowCount, WM_GETTEXT, (WPARAM)length + 1, (LPARAM)buffer);

			int rowCount = _wtoi(buffer);

			delete[] buffer;

			if (columnCount <= 0 || columnCount > 1000) {
				MessageBox(hwndDlg, L"Incorrect column count", L"Error", MB_OK);
			}
			else if (rowCount <= 0 || rowCount > 1000) {
				MessageBox(hwndDlg, L"Incorrect row count", L"Error", MB_OK);
			}
			else {
				that->SetFieldSizes(columnCount, rowCount);
				that->UpdateChildWindowsSizes();
				DestroyWindow(hwndDlg);
			}

			return TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL) {
			DestroyWindow(hwndDlg);
			return TRUE;
		}
	}

	return FALSE;
}
