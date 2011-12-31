// proba.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <string>
#include <iostream>

#include <centercutplus/ui/configdialog.h>
#include <centercutplus/wrapper/centercutwrapper.h>

int _tmain(int argc, _TCHAR* argv[])
{
    // TODO: this is a totally temporary test application

    ccp::wrapper::CenterCutWrapper wrapper;
    ccp::ui::ConfigDialog dialog1;
    ccp::ui::ConfigDialog dialog2;
    ccp::ui::ConfigDialog dialog3;

    dialog1.Create(NULL, NULL);
    wrapper.AddView(dialog1);
    wrapper.AddView(dialog2);
    wrapper.AddView(dialog3);
    dialog3.Create(NULL, NULL);

    dialog2.DoModal(NULL, NULL);

    dialog1.Destroy();
    dialog3.Destroy();


	return 0;
}

