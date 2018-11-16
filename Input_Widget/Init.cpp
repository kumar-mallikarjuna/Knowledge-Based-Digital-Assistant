#include "QApplication"
#include "QWidget"
#include "QLineEdit"
#include "QObject"
#include "QDebug"
#include "string"
#include "unistd.h"
#include "mysql++.h"
#include "Init.h"

using namespace std;

int main(int argc,char *argv[]){

	QApplication application(argc,argv);

	Window window;
	mysqlpp::Connection connection(false);
	connection.connect("","localhost","root","");
	Label input;
	
	mysqlpp::StoreQueryResult result = connection.query("SELECT * FROM `input`").store();

	input.setParent(&window);
	input.resize(1055,33);

	string done;
	result[0][2].to_string(done);
	if(done == "0"){
		input.setStyleSheet("padding-left:2.5px;background:#00D9FF;color:#FFFFFF;font-family:Ubuntu;font-size:17px;font-weight:450;selection-background-color:#FFFFFF;selection-color:#000;border-bottom:2px solid #F00");
	}
	if(done == "1"){
		input.setStyleSheet("padding-left:2.5px;background:#00D9FF;color:#FFFFFF;font-family:Ubuntu;font-size:17px;font-weight:450;selection-background-color:#FFFFFF;selection-color:#000;border-bottom:2px solid #0F0;");
	}

	window.setWindowTitle("Input Dialog");
	window.setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowStaysOnBottomHint);
	window.resize(1055,33);
	window.show();

	Thread loop(&connection,&input);
	QObject::connect(&loop,SIGNAL(update(QString)),&input,SLOT(update(QString)));
	loop.start();

	return application.exec();
}
