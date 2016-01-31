#include <Windows.h>
#include "resource.h"
#include "Constants.h"
#include "NonogramField.h"
#include "OverlappedWindow.h"
#include "NonogramSolver.h"
#include "ColumnDescriptionController.h"
#include "RowDescriptionController.h"
#include "NonogramFieldController.h"

int _stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR commandLine, int nCmdShow) {
	if (!OverlappedWindow::RegisterClass(hInstance)) {
		return 1;
	}

	NonogramField field(DEFAULT_COLUMN_COUNT, DEFAULT_ROW_COUNT);
	vector< vector<int> > columnBlockSizes(DEFAULT_COLUMN_COUNT);
	vector< vector<int> > rowBlockSizes(DEFAULT_ROW_COUNT);

	NonogramSolver solver(field, columnBlockSizes, rowBlockSizes);
	ColumnDescriptionController columnDescriptionsController(columnBlockSizes);
	RowDescriptionController rowDescriptionsController(rowBlockSizes);
	NonogramFieldController fieldController(field);

	OverlappedWindow window(solver, columnDescriptionsController, rowDescriptionsController, fieldController);

	if (!window.Create(hInstance, NULL)) {
		return 1;
	}

	window.Show(nCmdShow);

	MSG message;
	BOOL getMessageResult = 0;
	
	while ((getMessageResult = GetMessage(&message, NULL, 0, 0)) != 0) {
		if (!IsDialogMessage(window.GetDialogHandle(), &message)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}

	return message.wParam;
}
