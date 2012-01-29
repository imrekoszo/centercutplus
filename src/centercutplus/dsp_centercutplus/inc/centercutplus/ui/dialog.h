#pragma once

#include <set>

#include <Windows.h>

#include "dll.h"

namespace ccp
{
namespace ui
{

class DSP_CENTERCUTPLUS_API Dialog
{
    // ctor
  public:
    Dialog(int idd);
    virtual ~Dialog();

    // methods
  public:
    void Create(HINSTANCE hInst, HWND hParentWnd);
    BOOL Destroy();
    INT_PTR DoModal(HINSTANCE hInst, HWND hParentWnd);
    BOOL End(INT_PTR result);
    HWND HDlg() const { return _hDlg; }
    HWND HParentWnd() const { return _hParentWnd; }
    HINSTANCE HInstance() const { return _hInstance; }
    bool IsWindow() const { return ::IsWindow(HDlg()) == TRUE; }

  protected:
    virtual INT_PTR DlgProc(UINT message, WPARAM wParam, LPARAM lParam);
    void EnableControl(UINT idControl, bool enable);

  private:
    static std::set<Dialog*>& Instances()
    {
        static std::set<Dialog*> instances;
        return instances;
    }
    static INT_PTR CALLBACK _DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
    static Dialog* TryGetDialogInstance(HWND hDlg, UINT message, LPARAM lParam);
    void StoreCreateInfo(HINSTANCE hInst, HWND hParentWnd);
    void TryPostProcessDestroy(UINT message);


    // data
  public:
    static HINSTANCE& GHInstance() // TODO: TEMPORARY HACK!
    {
        static HINSTANCE i = NULL;
        return i;
    }

  private:
    HWND _hDlg;
    HWND _hParentWnd;
    HINSTANCE _hInstance;
    int _idd;

  private:
    NONCOPYABLE(Dialog);
};

}
}
