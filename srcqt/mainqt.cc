/*
 * mainqt.cc
 *
 *  Created on: 28.02.2023
 *      Author: martin
 */
#include "mainqt.h"
#include <QtWidgets>
#include <arg.h>
#include <iostream>
#include <CpputilsDebug.h>
#include "OutDebug.h"
#include <getline.h>
#include <QSettings>
#include "AsyncOutDebug.h"
#include <chrono>
#include <format.h>

using namespace std::chrono_literals;
using namespace Tools;

static std::string format_date_time( time_t t, const std::string fmt = "%Y-%m-%d %H:%M:%S" )
{
    char acBuf[100];
    struct tm *ptm = localtime(&t);
    strftime( acBuf, 100, fmt.c_str(), ptm );

    return std::string(acBuf);
}


MainWindowQt::MainWindowQt( int argc, char **argv, QWidget *parent)
: QMainWindow( parent ),
  lang()
{
#if 0
	QSettings settings;

	QAction *actionQuit;

	QMenuBar *menuBar = new QMenuBar(this);
	QMenu *menuProgram = new QMenu(menuBar);
	menuProgram->setTitle( wLCQ(L"&Program") );
	setMenuBar(menuBar);

	menuBar->addAction(menuProgram->menuAction());

	menuProgram->addAction(actionQuit);

	// new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_T), this, SLOT(newSearch()));

	connect(actionQuit, SIGNAL (triggered()), QApplication::instance(), SLOT (quit()));
#endif

	std::filesystem::path( "images" );

	std::vector<std::filesystem::path> paths;

	for (const auto & entry : std::filesystem::directory_iterator("images")) {
		CPPDEBUG( Tools::format( entry.path() ) );
		paths.push_back(entry.path());
	}

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen(rd()); // seed the generator
	std::uniform_int_distribution<> distr(1, paths.size()); // define the range

	auto num = distr(gen);
	CPPDEBUG( Tools::format( "num: %d", num ));

	auto p = paths.at(num-1);

    label = new QLabel();
    label->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
    label->setStyleSheet(
    		Tools::format("font-weight: bold; font-size: 300px;"
    				"background-image: url('%s')",p.string()).c_str());
    setCentralWidget(label);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindowQt::timeUpdate);
    timer->start(1000);

    timeUpdate();

    // restoreGeometry(settings.value(CONFIG_MAIN_WIN_GEOMETRY).toByteArray());
}

MainWindowQt::~MainWindowQt()
{
	// QSettings settings;
	// settings.setValue(CONFIG_MAIN_WIN_GEOMETRY, saveGeometry() );
}

void MainWindowQt::timeUpdate()
{
    std::string text = format_date_time( time(0), "%H:%M:%S" );
    label->setText( text.c_str() );
}


std::wstring MainWindowQt::wLC( const std::wstring & s )
{
	return lang.wLC( s );
}

const char* MainWindowQt::LC( const char * msg )
{
	return lang.LC( msg );
}

QString MainWindowQt::wLCQ( const std::wstring & s )
{
	return QString::fromStdWString( lang.wLC( s ) );
}

static void usage( const std::string & prog )
{
  std::cerr << "usage: "
			<< prog << " SEARCH PATTERN\n";
}

int main(int argc, char **argv)
{
	setlocale( LC_ALL, "" );

	Arg::Arg arg( argc, argv );

	arg.addPrefix( "-" );
	arg.addPrefix( "--" );

	Arg::OptionChain oc_info;
	arg.addChainR( &oc_info );
	oc_info.setMinMatch( 1 );
	oc_info.setContinueOnMatch( false );
	oc_info.setContinueOnFail( true );

	Arg::FlagOption o_help( "h" );
	o_help.addName( "help" );
	o_help.setDescription( "Show this page" );
	oc_info.addOptionR( &o_help );

	Arg::FlagOption o_debug("debug");
	o_debug.setDescription("print debugging messages");
	o_debug.setRequired(false);
	arg.addOptionR( &o_debug );

	arg.parse();

	if( o_help.getState() )
	{
		usage(argv[0]);
		std::cout << arg.getHelp(5,20,30, 80 ) << std::endl;
		return 0;
	}

	if( o_debug.getState() )
	{
#ifdef _WIN32
		AllocConsole();
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
#endif
		// Tools::x_debug = new OutDebug();

		auto log_frontend = new AsyncOut::Debug();

		Tools::x_debug = log_frontend;

		std::thread( []( auto log_frontend ) {
			AsyncOut::Logger backend;
			log_frontend->subscribe(&backend);

			while( true ) {
				backend.log();
				backend.wait();
				//std::this_thread::sleep_for(200ms);
			}
		}, log_frontend ).detach();
	}

	QApplication app (argc, argv);

	auto font = app.font();
	font.setPointSize(10);
	app.setFont(font);

	QCoreApplication::setOrganizationName("RedeyeLabs");
	QCoreApplication::setOrganizationDomain("borger.co.at");
	QCoreApplication::setApplicationName("Reminder");

	MainWindowQt mainwindow( argc, argv);

	//mainwindow.move(200,200);
	//mainwindow.show();
	mainwindow.showFullScreen();

	int ret = 0;

	try {
		ret = app.exec();

	} catch( const std::exception & error ) {
		std::wcerr << L"Error: " << error.what() << std::endl;
#ifdef _WIN32
		system("pause");
#endif
	} catch( ... ) {
		std::wcerr << L"UnknownError" << std::endl;
#ifdef _WIN32
		system("pause");
#endif
	}

	return ret;
}
