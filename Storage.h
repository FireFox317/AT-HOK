/*
 * Storage.h
 *
 *  Created on: Apr 17, 2018
 *      Author: timon
 */

#ifndef STORAGE_H_
#define STORAGE_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#	include <wx/wx.h>
#endif

#include <map>

class Storage {
public:
	Storage();
	virtual ~Storage();
	void addGroupChatMessage(std::string message);
	void addOneToOneMessage(std::string ip,std::string message);
	void setBox(wxListBox* _box);
	void setMode(std::string _mode);
	std::string getMode();
private:
	wxArrayString groupChatStorage;
	std::map<std::string,wxArrayString> OneToOneStorage;
	wxListBox* box;
	void updateBox();
	std::string mode;
};

extern Storage storage;

#endif /* STORAGE_H_ */
