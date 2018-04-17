/*
 * main.h
 *
 *  Created on: Apr 15, 2018
 *      Author: timon
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <thread>
#include "Socket/ip_config.h"

class MainFrame;

class MainApp: public wxApp {
public:
	virtual bool OnInit();
	virtual int OnExit();

	MainFrame* mainFrame;

	std::thread* send_thread;
	std::thread* receive_thread;
};

class MainFrame : public wxFrame {
public:
	MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

	void OnClick( wxCommandEvent& event);
	void setGroupchat( wxCommandEvent& event);
	void setOneToOne(wxCommandEvent& event);
	void showNotification(wxCommandEvent& event);
	wxTextCtrl* input;
	wxButton* button;
	wxListBox* box;
	wxNotificationMessage* test;

	std::string ip = MULTIGROUP;


	DECLARE_EVENT_TABLE()
};

enum{
	BUTTON_send,
	INPUT_box,
	GROUPCHAT,
	ONETOONE
};

DECLARE_APP(MainApp);

wxDECLARE_EVENT(MY_EVENT, wxCommandEvent);



#endif /* MAIN_H_ */
