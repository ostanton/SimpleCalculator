#include "MainFrame.h"

#include <valarray>
#include <wx/wx.h>
#include <wx/gbsizer.h>

MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize)
{
    // Calculator layout
    // Output label
    // 7 8 9 /
    // 4 5 6 *
    // 1 2 3 -
    // . 0 C +
    //    =

    // Set rootSizer
    rootSizer = new wxBoxSizer(wxVERTICAL);

    // Set rootPanel
    rootPanel = new wxPanel(this);
    // Keyboard input
    rootPanel->Bind(wxEVT_CHAR_HOOK, &MainFrame::OnKeyDown, this);

    // Add rootPanel to rootSizer
    rootSizer->Add(rootPanel);

    // Set outputText
    outputText = new wxStaticText(this, wxID_ANY, "0");
    const wxFont outputFont(20, outputText->GetFont().GetFamily(), outputText->GetFont().GetStyle(), wxFONTWEIGHT_BOLD);
    outputText->SetFont(outputFont);

    const wxFont buttonFont(12, outputText->GetFont().GetFamily(), outputText->GetFont().GetStyle(), wxFONTWEIGHT_NORMAL);

    // Add outputText to rootSizer
    rootSizer->Add(outputText, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 8);

    // Set buttonSizer
    buttonSizer = new wxGridBagSizer(0, 0);
    buttonSizer->SetCols(4);
    buttonSizer->SetRows(5);
    buttonSizer->AddGrowableCol(0, 1);
    buttonSizer->AddGrowableCol(1, 1);
    buttonSizer->AddGrowableCol(2, 1);
    buttonSizer->AddGrowableCol(3, 1);
    buttonSizer->AddGrowableRow(0, 1);
    buttonSizer->AddGrowableRow(1, 1);
    buttonSizer->AddGrowableRow(2, 1);
    buttonSizer->AddGrowableRow(3, 1);
    buttonSizer->AddGrowableRow(4, 1);

    // Add button grid sizer to rootSizer
    rootSizer->Add(buttonSizer, 1, wxEXPAND | wxALL, 6);

    // Create and set buttons, I would do this in the for loop below but man, how do I even do that, it's crazy!
    buttons[0] = new wxButton(this, wxID_ANY, "7");
    buttons[1] = new wxButton(this, wxID_ANY, "8");
    buttons[2] = new wxButton(this, wxID_ANY, "9");
    buttons[3] = new wxButton(this, wxID_ANY, "/");

    buttons[4] = new wxButton(this, wxID_ANY, "4");
    buttons[5] = new wxButton(this, wxID_ANY, "5");
    buttons[6] = new wxButton(this, wxID_ANY, "6");
    buttons[7] = new wxButton(this, wxID_ANY, "*");

    buttons[8] = new wxButton(this, wxID_ANY, "1");
    buttons[9] = new wxButton(this, wxID_ANY, "2");
    buttons[10] = new wxButton(this, wxID_ANY, "3");
    buttons[11] = new wxButton(this, wxID_ANY, "-");

    buttons[12] = new wxButton(this, wxID_ANY, ".");
    buttons[13] = new wxButton(this, wxID_ANY, "0");
    buttons[14] = new wxButton(this, wxID_ANY, "C");
    buttons[15] = new wxButton(this, wxID_ANY, "+");

    buttons[16] = new wxButton(this, wxID_ANY, "=");

    // For each button, bind the event to it, and add it to the buttonSizer grid
    for (int i {0}; i < std::end(buttons) - std::begin(buttons); i++)
    {
        if (buttons[i])
        {
            buttons[i]->SetFont(buttonFont);
            buttons[i]->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClick, this);
            buttonSizer->Add(buttons[i], 1, wxEXPAND | wxALL, 2);

            const int column = i % 4;
            const int row = i / 4;
            
            buttonSizer->SetItemPosition(i, wxGBPosition(row, column));
            buttonSizer->SetItemSpan(i, wxGBSpan(1, i != 16 ? 1 : 4));
        }
    }

    // Set rootSizer to this frame
    SetSizerAndFit(rootSizer);
}

void MainFrame::OnButtonClick(const wxCommandEvent& _event)
{
    const char buttonChar { dynamic_cast<wxButton*>(_event.GetEventObject())->GetLabel()[0] };

    buttonPressed(buttonChar);
}

void MainFrame::OnKeyDown(wxKeyEvent& _event)
{
    buttonPressed(_event.m_uniChar);
}

void MainFrame::buttonPressed(char input)
{
    // If this input is not a digit, switch on what it could be
    if (!isdigit(input))
    {
        switch (input)
        {
        default:
            break;
        case '+':
            operatorPressed(add);
            break;
        case '-':
            operatorPressed(subtract);
            break;
        case '*':
            operatorPressed(multiply);
            break;
        case '/':
            operatorPressed(divide);
            break;
        case '.':
            decimalPressed();
            break;
        case '=':
            equalsPressed();
            break;
        case 'C':
            outputText->SetLabelText("0");
            firstNum = 0.f;
            break;
        }

        return;
    }

    // Otherwise get the number value of input and send it through
    numberPressed(input - '0');
}

void MainFrame::numberPressed(int num) const
{
    if (outputText->GetLabelText() != "0")
        outputText->SetLabelText(outputText->GetLabelText() + std::to_string(num));
    else
        outputText->SetLabelText(std::to_string(num));
}

void MainFrame::operatorPressed(operations op)
{
    firstNum = std::stof(outputText->GetLabelText().utf8_string());
    outputText->SetLabelText("0");
    operationSelected = true;
    selectedOperator = op;
}

void MainFrame::equalsPressed()
{
    // If we have an operator on the go
    if (operationSelected)
    {
        secondNum = std::stof(outputText->GetLabelText().utf8_string());

        // The actual maths behind the calculator!!
        // Literally the same as what you'd do in a CLI calculator lol, nothing exciting.
        switch (selectedOperator)
        {
        case add:
            outputText->SetLabelText(std::to_string(firstNum + secondNum));
            break;
        case subtract:
            outputText->SetLabelText(std::to_string(firstNum - secondNum));
            break;
        case multiply:
            outputText->SetLabelText(std::to_string(firstNum * secondNum));
            break;
        case divide:
            outputText->SetLabelText(std::to_string(firstNum / secondNum));
            break;
        case none:
            outputText->SetLabelText("None");
            break;
        }

        operationSelected = false;
        selectedOperator = none;
    }
}

void MainFrame::decimalPressed()
{
    if (!outputText->GetLabelText().Contains("."))
        outputText->SetLabelText(outputText->GetLabelText() + ".");
}
