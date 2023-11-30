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

class MainWindowQt : public QMainWindow
{
	 Q_OBJECT
private:
	 static constexpr const char* CONFIG_MAIN_WIN_GEOMETRY = "MainWin/Geometry";

	 QTabWidget *tabs;

	  Lang lang;

public:
	 explicit MainWindowQt( int argc, char **argv, QWidget *parent = 0);
	 ~MainWindowQt();

	 std::wstring wLC( const std::wstring & s );
	 const char* LC( const char * msg );
	 QString wLCQ( const std::wstring & s );
};

#endif /* SRCQT_MAINQT_H_ */
