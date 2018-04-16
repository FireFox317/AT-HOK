/*
 * main.h
 *
 *  Created on: Apr 15, 2018
 *      Author: timon
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <thread>

class MainApp: public wxApp {
public:
	virtual bool OnInit();
	virtual int OnExit();

	std::thread* send_thread;
	std::thread* receive_thread;
};

class MainFrame : public wxFrame {
public:
	MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

	void OnClick( wxCommandEvent& event);
	wxTextCtrl* input;
	wxButton* button;
	wxListBox* box;



	DECLARE_EVENT_TABLE()
};

enum{
	BUTTON_send,
	INPUT
};

DECLARE_APP(MainApp);


#endif /* MAIN_H_ */
