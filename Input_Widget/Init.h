#ifndef INIT_H
#define INIT_H

#include "mysql++.h"
#include "string"
#include "QWidget"
#include "QLabel"
#include "QContextMenuEvent"
#include "QFocusEvent"
#include "QThread"
#include "QtCore/QDebug"
#include "unistd.h"

using namespace std;

class Window : public QWidget{
};

class Thread : public QThread{
	Q_OBJECT
	
	signals:
		void update(QString stylesheet);

	public:
		mysqlpp::Connection *connection;
		QLabel *input;
		Thread(mysqlpp::Connection *connection_,QLabel *input_){
			connection = connection_;
			input = input_;
		}

		void run(){
			bool input_focus_bool = false;
			while(1){
				if(!input_focus_bool){
					mysqlpp::StoreQueryResult result = connection->query("SELECT * FROM `input`").store();
					string str,done;
	 			 	result[0][1].to_string(str);
					result[0][2].to_string(done);
					if(input->text().toStdString() != str){
						if(str != ""){
							input->setText(str.c_str());
						}else{
							if(done == "0"){
//								input->setStyleSheet("padding-left:2.5px;background:#00D9FF;color:#FFFFFF;font-family:Ubuntu;font-size:17px;font-weight:450;selection-background-color:#FFFFFF;selection-color:#000;border-bottom:2px solid #F00");
								emit update("padding-left:2.5px;background:#00D9FF;color:#FFFFFF;font-family:Ubuntu;font-size:17px;font-weight:450;selection-background-color:#FFFFFF;selection-color:#000;border-bottom:2px solid #F00");
							}
							if(done == "1"){
//								input->setStyleSheet("padding-left:2.5px;background:#00D9FF;color:#FFFFFF;font-family:Ubuntu;font-size:17px;font-weight:450;selection-background-color:#FFFFFF;selection-color:#000;border-bottom:2px solid #0F0;");
								emit update("padding-left:2.5px;background:#00D9FF;color:#FFFFFF;font-family:Ubuntu;font-size:17px;font-weight:450;selection-background-color:#FFFFFF;selection-color:#000;border-bottom:2px solid #0F0");
							}
						}
					}

					usleep(100000);
				}
			}
		}
};

class Label : public QLabel{
	Q_OBJECT
	
	public slots:
		void update(QString stylesheet){
			this->setStyleSheet(stylesheet);
		}
};

/*

class Textbox : public QLabel{
	Q_OBJECT

	public:
		mysqlpp::Connection *connection;
		bool *input_focus_bool;
		Textbox(mysqlpp::Connection *connection_2,bool *input_focus_bool_){
			connection = connection_2;
			input_focus_bool = input_focus_bool_;
		}

		void focusInEvent(QFocusEvent *event){
			*input_focus_bool = true;
		}
		void focusOutEvent(QFocusEvent *event){
			*input_focus_bool = false;
		}

	public slots:
		void submit(){
			connection->query("UPDATE `input` SET `input`='" + this->text().toStdString() + "' WHERE `id`=1").execute();
		}

	void contextMenuEvent(QContextMenuEvent *event){
	}
};

*/

#endif
