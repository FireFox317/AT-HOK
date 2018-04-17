/*
 * Storage.cpp
 *
 *  Created on: Apr 17, 2018
 *      Author: timon
 */

#include "Storage.h"

Storage storage;

Storage::Storage() {
	// TODO Auto-generated constructor stub

}

Storage::~Storage() {
	// TODO Auto-generated destructor stub
}

void Storage::addGroupChatMessage(std::string message){
	groupChatStorage.Insert(message,0);
	updateBox();
}

void Storage::addOneToOneMessage(std::string ip, std::string message){
	OneToOneStorage[ip].Insert(message,0);
	updateBox();
}

void Storage::setBox(wxListBox* _box){
	box = _box;
}

void Storage::updateBox(){
	if(mode == "GroupChat"){
		if(box->GetCount() != 0){
			box->Clear();
		}
		if(groupChatStorage.size() != 0 ){
			box->InsertItems(groupChatStorage,0);
			box->EnsureVisible(0);
		}
	} else {
		if(box->GetCount() != 0){
			box->Clear();
		}
		std::string ip = mode.substr(6,mode.size()-6);
		if(OneToOneStorage[ip].size() != 0){
			box->InsertItems(OneToOneStorage[ip],0);
			box->EnsureVisible(0);
		}
	}

}

void Storage::setMode(std::string _mode){
	mode = _mode;
	updateBox();
}

std::string Storage::getMode(){
	return mode;
}



