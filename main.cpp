#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#	include <wx/wx.h>
#endif

#include <wx/notifmsg.h>

#include "main.h"

#include <thread>

#include "Socket/Sender.h"
#include "Socket/Receiver.h"
#include "Message.h"
#include "Storage.h"



IMPLEMENT_APP(MainApp);

wxDEFINE_EVENT(MY_EVENT, MessageEvent);

bool MainApp::OnInit(){
	send_thread = new std::thread(Sender::loop);
	receive_thread = new std::thread(Receiver::loop);

	Sender::sendMessage(Message(MULTIGROUP,"Joined the groupchat."));

	mainFrame = new MainFrame(wxT("AD-HOC Chatting"), wxDefaultPosition, wxSize(500,400));
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
	EVT_TEXT_ENTER(INPUT_box, MainFrame::OnClick)
	EVT_MENU(GROUPCHAT, MainFrame::setGroupchat)
	EVT_MENU(ONETOONE, MainFrame::setOneToOne)
	MY_EVT_MESSAGE(wxID_ANY, MainFrame::showNotification)
END_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame((wxFrame *) NULL, -1, title, pos, size)
{

	wxMenuBar* menubar = new wxMenuBar();
	wxMenu* mode = new wxMenu();
	mode->Append(GROUPCHAT, wxT("&Groupchat"));
	mode->Append(ONETOONE, wxT("&One-to-One"));
	menubar->Append(mode, wxT("&Mode"));
	SetMenuBar(menubar);

	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *verticalBox = new wxBoxSizer(wxVERTICAL);

	input = new wxTextCtrl(this, INPUT_box, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	button = new wxButton(this, BUTTON_send, wxT("Send"));

	box = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	test = new wxNotificationMessage("AD-HOC Message");

	storage.setBox(box);
	storage.setMode("GroupChat");


	sizer->Add(input, 1);
	sizer->Add(button, 0);
	verticalBox->Add(sizer,0, wxEXPAND);
	verticalBox->Add(box, 1, wxEXPAND);
	SetSizer(verticalBox);

    CreateStatusBar();
    SetStatusText("Mode: Groupchat");

	input->SetFocus();
}

void MainFrame::OnClick(wxCommandEvent &event){
	if(!input->GetLineText(0).IsEmpty()){
		if(ip == MULTIGROUP){
			storage.addGroupChatMessage("You > " + input->GetLineText(0).ToStdString());
		} else {
			storage.addOneToOneMessage(ip,"You > " + input->GetLineText(0).ToStdString());
		}

		Message mes(ip,input->GetLineText(0).ToStdString());
		Sender::sendMessage(mes);

		input->Clear();
	}
}

void MainFrame::setGroupchat(wxCommandEvent &event){
	ip = MULTIGROUP;
	wxMessageBox("Set to groupchatting!", "GroupChat", wxICON_NONE);
	SetStatusText("Mode: Groupchat");
	Sender::sendMessage(Message(MULTIGROUP,"Joined the groupchat."));
	storage.setMode("GroupChat");

}

void MainFrame::setOneToOne(wxCommandEvent &event){
	wxTextEntryDialog* test = new wxTextEntryDialog(this, "Fill in the Computer number to chat with.", "One-to-One");
	if(test->ShowModal() == wxID_OK){
		ip = "192.168.5." + test->GetValue();
		SetStatusText("Mode: One to one -> Computer number: " + test->GetValue());
		Sender::sendMessage(Message(MULTIGROUP,"Left the groupchat."));
		storage.setMode("1To1: " + ip);
	}
}

void MainFrame::showNotification(MessageEvent& event){
	Message message = event.GetMessage();
	if(message.checkMultigroup()){
		test->SetTitle("Received a Groupchat Message!");
		test->SetMessage(message.getData());
		if(storage.getMode() != "GroupChat"){
			test->Show();
		}
	} else {
		test->SetTitle("Received a message from " + message.getComputerNumber());
		test->SetMessage(message.getData());
		if(message.getComputerNumber() != storage.getMode().substr(16,storage.getMode().size()-16)){
			test->Show();
		}
	}
}
