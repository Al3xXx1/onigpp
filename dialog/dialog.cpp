// dialog.cpp --- Oniguruma++ test dialog
// Author: katahiromz
// License: BSD-2-Clause
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <string>
#include "onigpp.h"

namespace rex = onigpp;

#ifdef UNICODE
	using string_type = std::wstring;
	using regex_type = rex::wregex;
#else
	using string_type = std::string;
	using regex_type = rex::regex;
#endif

bool do_find(const string_type& input, DWORD& iStart, DWORD& iEnd, regex_type& re) {
	try {
		// Ensure positions are within the string bounds
		const auto input_len = static_cast<typename string_type::size_type>(input.size());
		if (iStart > input_len) iStart = static_cast<DWORD>(input_len);
		if (iEnd > input_len) iEnd = static_cast<DWORD>(input_len);

		using const_iter = typename string_type::const_iterator;
		rex::match_results<const_iter> m;

		// Search forward from the end of current selection first
		const_iter start_it = input.begin() + static_cast<ptrdiff_t>(iEnd);
		if (rex::regex_search(start_it, input.end(), m, re)) {
			// m.position(0) is relative to start_it
			const auto pos = static_cast<size_t>(iEnd) + static_cast<size_t>(m.position(0));
			const auto len = static_cast<size_t>(m.length(0));
			iStart = static_cast<DWORD>(pos);
			iEnd = static_cast<DWORD>(pos + len);
			return true;
		}

		// If not found, wrap around and search from the beginning
		if (input_len > 0) {
			if (rex::regex_search(input.begin(), input.end(), m, re)) {
				const auto pos = static_cast<size_t>(m.position(0));
				const auto len = static_cast<size_t>(m.length(0));
				iStart = static_cast<DWORD>(pos);
				iEnd = static_cast<DWORD>(pos + len);
				return true;
			}
		}

		return false;
	} catch (const rex::regex_error&) {
		// Any regex-related error: treat as no match / failure
		return false;
	}
};

void OnFindReplace(HWND hwnd, int action) {
	BOOL oniguruma = IsDlgButtonChecked(hwnd, chx1) == BST_CHECKED;
	BOOL ecma = IsDlgButtonChecked(hwnd, chx2) == BST_CHECKED;
	BOOL icase = IsDlgButtonChecked(hwnd, chx3) == BST_CHECKED;

	TCHAR text[4][256];
	GetDlgItemText(hwnd, edt1, text[0], _countof(text[0]));
	GetDlgItemText(hwnd, edt2, text[1], _countof(text[1]));
	GetDlgItemText(hwnd, edt3, text[2], _countof(text[2]));
	GetDlgItemText(hwnd, edt4, text[3], _countof(text[3]));
	string_type input = text[0];
	string_type pattern = text[2];
	string_type replacement = text[3];

	int flags = 0;
	if (oniguruma) flags |= rex::regex::oniguruma;
	if (ecma) flags |= rex::regex::ECMAScript;
	if (icase) flags |= rex::regex::icase;

	regex_type re;
	try {
		re = regex_type(pattern, flags);
	} catch (const rex::regex_error& e) {
		MessageBox(hwnd, TEXT("Failure!"), NULL, MB_ICONERROR);
		return;
	}

	DWORD iStart, iEnd;
	SendDlgItemMessage(hwnd, edt1, EM_GETSEL, (WPARAM)&iStart, (LPARAM)&iEnd);

	switch (action) {
	case 0: // find
		{
			if (!do_find(input, iStart, iEnd, re)) {
				MessageBox(hwnd, TEXT("No more match"), TEXT("dialog"), MB_ICONINFORMATION);
				return;
			}
		}
		break;
	case 1: // replace
		{
			// TODO:
		}
		break;
	case 2: // replace all
		{
			// TODO:
		}
		break;
	default:
		assert(0);
		return;
	}

	SendDlgItemMessage(hwnd, edt1, EM_SETSEL, iStart, iEnd);
}

// WM_INITDIALOG
BOOL OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {
	SetDlgItemText(hwnd, edt1, TEXT("This is a test.\r\n\r\nThis is a test.\r\n"));
	CheckDlgButton(hwnd, chx1, BST_CHECKED); // oniguruma
	return TRUE;
}

// WM_COMMAND
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
	switch (id) {
	case IDOK:
	case IDCANCEL:
		EndDialog(hwnd, id);
		break;
	case psh1: // Replace
		OnFindReplace(hwnd, 1);
		break;
	case psh2: // Find
		OnFindReplace(hwnd, 0);
		break;
	case psh3: // Replace All
		OnFindReplace(hwnd, 2);
		break;
	}
}

INT_PTR CALLBACK
DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		HANDLE_MSG(hwnd, WM_INITDIALOG, OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, OnCommand);
	}
	return 0;
}

INT WINAPI
WinMain(HINSTANCE   hInstance,
        HINSTANCE   hPrevInstance,
        LPSTR       lpCmdLine,
        INT         nCmdShow)
{
	onigpp::auto_init init;

	InitCommonControls();

	DialogBox(hInstance, MAKEINTRESOURCE(1), NULL, DialogProc);
	return 0;
}
