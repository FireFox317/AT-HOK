#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#	include <wx/wx.h>
#endif

#include "main.h"

#include <thread>

#include "Socket/Sender.h"
#include "Socket/Receiver.h"
#include "Message.h"

IMPLEMENT_APP(MainApp);

bool MainApp::OnInit(){
	send_thread = new std::thread(Sender::loop);
	receive_thread = new std::thread(Receiver::loop);

	MainFrame* mainFrame = new MainFrame(wxT("AD-HOC Chatting"), wxDefaultPosition, wxSize(300,200));
	mainFrame->Show(true);
	SetTopWindow(mainFrame);
	return true;
}

int MainApp::OnExit(){
		Sender::closeSocket();
		Receiver::closeSocket();

		send_thread->join();
		receive_thread->detach();

		return 0;
}

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_BUTTON(BUTTON_send, MainFrame::OnClick)
	EVT_TEXT_ENTER(INPUT, MainFrame::OnClick)
END_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame((wxFrame *) NULL, -1, title, pos, size)
{

	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *verticalBox = new wxBoxSizer(wxVERTICAL);

	input = new wxTextCtrl(this, INPUT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	button = new wxButton(this, BUTTON_send, wxT("Send"));

	box = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	Receiver::setBox(box);

	sizer->Add(input, 1);
	sizer->Add(button, 0);
	verticalBox->Add(sizer,0, wxEXPAND);
	verticalBox->Add(box, 1, wxEXPAND);
	SetSizer(verticalBox);

	input->SetFocus();
}

void MainFrame::OnClick(wxCommandEvent &event){
	if(!input->GetLineText(0).IsEmpty()){

		wxString item[1] = {"You > " + input->GetLineText(0)};
		box->InsertItems(1, item,0);
		box->EnsureVisible(0);

		Message mes("192.168.5.2",input->GetLineText(0).ToStdString());
		Sender::sendMessage(mes);

		input->Clear();
	}
}
