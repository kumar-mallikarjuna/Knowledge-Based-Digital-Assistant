#include "QtGui/QApplication"
#include "QtGui/QLabel"
#include "QtGui/QDesktopWidget"
#include "QtGui/QScrollArea"
#include "QtGui/QFont"
#include "QtGui/QIcon"

#include "QtCore/QDebug"
#include "QtCore/QString"

int main(int argc,char *argv[]){
	
	qDebug() << QString(argv[1]);

	QApplication application(argc,argv);
	QWidget widget;
	
	widget.setWindowIcon(QIcon("<Picture>.png"));
	widget.setWindowTitle(QString(argv[1]));
	widget.setFixedSize(900,500);
	widget.move((application.desktop()->width()-widget.width())/2,(application.desktop()->height()-widget.height())/2);
	
	QScrollArea area(&widget);
	area.resize(widget.size());
	area.setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	area.setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	area.setStyleSheet("padding:5px;font-size:15px");

	QLabel label;
	label.setFixedWidth(850);
	label.setText(QString(argv[2]));
	label.setWordWrap(true);
	label.setFont(QFont("Monospace"));

	area.setWidget(&label);

	widget.show();

	return application.exec();
}
