/*
 * mainqt.h
 *
 *  Created on: 28.02.2023
 *      Author: martin
 */

#ifndef SRCQT_MAINQT_H_
#define SRCQT_MAINQT_H_

#include <qmainwindow.h>
#include <qtabwidget.h>
#include "lang.h"
#include "search.h"

class QLabel;

class MainWindowQt : public QMainWindow
{
	 Q_OBJECT
private:
	 static constexpr const char* CONFIG_MAIN_WIN_GEOMETRY = "MainWin/Geometry";

	 QTimer *timer;
	 QLabel *label;
	 Lang lang;

public:
	 explicit MainWindowQt( int argc, char **argv, QWidget *parent = 0);
	 ~MainWindowQt();

	 std::wstring wLC( const std::wstring & s );
	 const char* LC( const char * msg );
	 QString wLCQ( const std::wstring & s );

public slots:
	void timeUpdate();

};

#endif /* SRCQT_MAINQT_H_ */
