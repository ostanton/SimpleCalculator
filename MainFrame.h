#pragma once

#include <wx/frame.h>

class wxGridSizer;
class wxStaticText;
class wxPanel;
class wxBoxSizer;
class wxGridBagSizer;

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);

    wxBoxSizer* rootSizer {nullptr};
    wxPanel* rootPanel {nullptr};

    wxStaticText* outputText {nullptr};

    wxGridBagSizer* buttonSizer {nullptr};
    wxButton* buttons[17];

    void OnButtonClick(const wxCommandEvent& _event);
    void OnKeyDown(wxKeyEvent& _event);

    void buttonPressed(char input);

    float firstNum {0.f};
    float secondNum {0.f};

    enum operations
    {
        none,
        add,
        subtract,
        multiply,
        divide
    };

    operations selectedOperator {none};
    bool operationSelected {false};

    void numberPressed(int num) const;
    void operatorPressed(operations op);
    void equalsPressed();
    void decimalPressed();
};
