#include "../inc/centercutplus/ui/dialog.h"

#include <boost/foreach.hpp>

namespace ccp
{
namespace ui
{

Dialog::Dialog(int idd)
    : _idd(idd),
      _hDlg(NULL),
      _hParentWnd(NULL),
      _hInstance(NULL)
{
}

Dialog::~Dialog()
{
    Instances().erase(this);
}

void Dialog::Create(HINSTANCE hInst, HWND hParentWnd)
{
    StoreCreateInfo(GHInstance(), hParentWnd); // TODO: TEMPORARY HACK!
    ::CreateDialogParam(_hInstance,
                        MAKEINTRESOURCE(_idd),
                        _hParentWnd,
                        _WindowProc,
                        reinterpret_cast<LPARAM>(this));
}

BOOL Dialog::Destroy()
{
    return ::IsWindow(_hDlg) ? ::DestroyWindow(_hDlg) : FALSE;
}

INT_PTR Dialog::DoModal(HINSTANCE hInst, HWND hParentWnd)
{
    StoreCreateInfo(GHInstance(), hParentWnd); // TODO: TEMPORARY HACK!
    return ::DialogBoxParam(_hInstance,
                            MAKEINTRESOURCE(_idd),
                            _hParentWnd,
                            _WindowProc,
                            reinterpret_cast<LPARAM>(this));
}

BOOL Dialog::End(INT_PTR result)
{
    return ::IsWindow(_hDlg) ? ::EndDialog(_hDlg, result) : FALSE;
}

INT_PTR Dialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    return static_cast<INT_PTR>(FALSE);
}

INT_PTR CALLBACK Dialog::_WindowProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    Dialog* instance = TryGetDialogInstance(hDlg, message, lParam);

    if(instance != NULL)
    {
        INT_PTR result = instance->WindowProc(message, wParam, lParam);
        instance->TryPostProcessDestroy(message);
        return result;
    }

    return static_cast<INT_PTR>(FALSE);
}

Dialog* Dialog::TryGetDialogInstance(HWND hDlg, UINT message, LPARAM lParam)
{
    std::set<Dialog*>& instances = Instances();

    if(message == WM_INITDIALOG)
    {
        Dialog* instance = reinterpret_cast<Dialog*>(lParam);
        instance->_hDlg = hDlg;
        instances.insert(instance);
        return instance;
    }

    BOOST_FOREACH(Dialog* instance, instances)
    {
        if(instance->_hDlg == hDlg)
        {
            return instance;
        }
    }

    return NULL;
}

void Dialog::StoreCreateInfo(HINSTANCE hInst, HWND hParentWnd)
{
    _hInstance = hInst;
    _hParentWnd = hParentWnd;
}

void Dialog::TryPostProcessDestroy(UINT message)
{
    if(message == WM_DESTROY)
    {
        Dialog::Instances().erase(this);
    }
}

}
}
