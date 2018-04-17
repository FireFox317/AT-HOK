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

#include "Message.h"

class MessageEvent: public wxEvent
{
public:
	MessageEvent(wxEventType eventType, int winid, const Message& message)
        : wxEvent(winid, eventType),
          message(message)
    {
    }
    // accessors
	Message GetMessage() const { return message; }
    // implement the base class pure virtual
    virtual wxEvent *Clone() const { return new MessageEvent(*this); }
private:
    const Message message;
};

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
	void showNotification(MessageEvent& event);
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

wxDECLARE_EVENT(MY_EVENT, MessageEvent);


#define MY_EVT_MESSAGE(id, func) \
    wx__DECLARE_EVT1(MY_EVENT, id, &func)




#endif /* MAIN_H_ */
