#include "NonogramCellWindow.h"
#include "resource.h"

wchar_t* NonogramCellWindow::className = L"ClassNonogramCellWindow";


BOOL __stdcall dialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);


NonogramCellWindow::NonogramCellWindow()
	: handle(0)
	, cell_(UNDEFINED)
{}

NonogramCellWindow::~NonogramCellWindow() {}

bool NonogramCellWindow::RegisterClass(HINSTANCE hInstance) {
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
	wc.lpszClassName = className;
	wc.hInstance = hInstance;
	wc.hIconSm = NULL;
	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, L"Can't register class", L"ERROR!", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	return true;
}

bool NonogramCellWindow::Create(HINSTANCE hInstance, HWND hWndParent, int positionX, int positionY, int size) {
	hInstance_ = hInstance;

	handle = CreateWindow(className, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_EX_NOPARENTNOTIFY,
		positionX, positionY, size, size,
		hWndParent, NULL, hInstance, this);

	UpdateWindow(handle);

	return true;
}

void NonogramCellWindow::Show(int cmdShow) {
	ShowWindow(handle, cmdShow);
	UpdateWindow(handle);
}

HWND NonogramCellWindow::GetHandle() {
	return handle;
}

NonogramCell &NonogramCellWindow::GetCell() {
	return cell_;
}

void NonogramCellWindow::onDestroy() {
}

void NonogramCellWindow::onLeftClick() {
	if (cell_ == FILLED) {
		cell_ = UNDEFINED;
	}
	else {
		cell_ = FILLED;
	}

	RECT rect;
	if (GetClientRect(handle, &rect)) {
		InvalidateRect(handle, &rect, false);
	}
}

void NonogramCellWindow::onRightClick() {
	if (cell_ == EMPTY) {
		cell_ = UNDEFINED;
	} else {
		cell_ = EMPTY;
	}

	RECT rect;
	if (GetClientRect(handle, &rect)) {
		InvalidateRect(handle, &rect, false);
	}
}

void NonogramCellWindow::onPaint() {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(handle, &ps);
	HDC newHdc = CreateCompatibleDC(hdc);
	RECT rect;
	GetClientRect(handle, &rect);

	HBITMAP bitmap = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
	HGDIOBJ oldbitmap = SelectObject(newHdc, bitmap);

	switch (cell_) {
	case FILLED:
		drawBlackSquare(newHdc);
		break;
	case EMPTY:
		drawBlackCross(newHdc);
		break;
	case UNDEFINED:
		drawWhiteSquare(newHdc);
	}

	BitBlt(hdc, 0, 0, rect.right, rect.bottom, newHdc, 0, 0, SRCCOPY);

	SelectObject(newHdc, oldbitmap);
	DeleteObject(bitmap);
	DeleteDC(newHdc);

	EndPaint(handle, &ps);
}

void NonogramCellWindow::drawBlackSquare(HDC hdc) {
	RECT rect;

	GetClientRect(handle, &rect);

	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
	FillRect(hdc, &rect, brush);
	DeleteObject(brush);
}

void NonogramCellWindow::drawWhiteSquare(HDC hdc) {
	RECT rect;

	GetClientRect(handle, &rect);

	HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
	FillRect(hdc, &rect, brush);
	DeleteObject(brush);
}

void NonogramCellWindow::drawBlackCross(HDC hdc) {
	RECT rect;

	GetClientRect(handle, &rect);

	HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
	FillRect(hdc, &rect, whiteBrush);
	DeleteObject(whiteBrush);

	MoveToEx(hdc, rect.left, rect.top, (LPPOINT)NULL);
	LineTo(hdc, rect.right, rect.bottom);
	MoveToEx(hdc, rect.left, rect.bottom, (LPPOINT)NULL);
	LineTo(hdc, rect.right, rect.top);
}

LRESULT __stdcall NonogramCellWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
	NonogramCellWindow* that = reinterpret_cast< NonogramCellWindow* >(GetWindowLong(handle, GWL_USERDATA));

	if (message == WM_NCCREATE) {
		NonogramCellWindow* that = reinterpret_cast< NonogramCellWindow* >(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams);
		SetWindowLong(handle, GWL_USERDATA, reinterpret_cast<LONG>(that));

		return DefWindowProc(handle, message, wParam, lParam);
	}

	switch (message) {
	case WM_DESTROY:
		that->onDestroy();
		return 0;
	case WM_PAINT:
		that->onPaint();
		return 0;
	case WM_LBUTTONDOWN:
		that->onLeftClick();
		return 0;
	case WM_RBUTTONDOWN:
		that->onRightClick();
		return 0;
	case WM_CLOSE:
		DestroyWindow(that->handle);
		return 0;
	}

	return DefWindowProc(handle, message, wParam, lParam);
}
