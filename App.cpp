#include "App.h"

#include <wx/app.h>

#include "MainFrame.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    MainFrame* mainFrame { new MainFrame("Calculator") };
    mainFrame->SetClientSize(200, 400);
    mainFrame->Centre();
    mainFrame->Show();
    return true;
}
