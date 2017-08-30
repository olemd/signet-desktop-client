#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QList>
#include <QListView>
#include <QLineEdit>
#include <QMessageBox>
#include <QMainWindow>
#include <QApplication>

#include "esdbmodel.h"
#include "systemtray.h"
#include "signetapplication.h"

struct signetdevCmdRespInfo;

namespace Ui
{
class MainWindow;
}

#include <map>
#include <QCloseEvent>
#include <QAction>
#include <QTimer>

class LoggedInWidget;
class ButtonWaitDialog;
class QFile;
class LoginWindow;
class QProgressBar;
class QMenu;
class QStackedWidget;
class QByteArray;

struct fwSection {
	QString name;
	unsigned int lma;
	int size;
	QByteArray contents;
};

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = 0);
	void closeEvent(QCloseEvent *event);
	~MainWindow();

	enum device_state {
		STATE_INVALID,
		STATE_NEVER_SHOWN,
		STATE_CONNECTING,
		STATE_RESET,
		STATE_UNINITIALIZED,
		STATE_LOGGED_OUT,
		STATE_LOGGED_IN_LOADING_ACCOUNTS,
		STATE_LOGGED_IN,
		STATE_WIPING,
		STATE_RESTORING,
		STATE_BACKING_UP,
		STATE_UPDATING_FIRMWARE
	};
private:
	QProgressBar *m_wipeProgress;
	QWidget *m_wipingWidget;
	bool m_connected;
	bool m_quitting;
	QMenu *m_fileMenu;
	QMenu *m_deviceMenu;
	QStackedWidget *m_loggedInStack;
	QLabel *m_connectingLabel;
	bool m_loggedIn;
	QTimer m_connectingTimer;
	bool m_wasConnected;

	enum device_state m_deviceState;

	QWidget *m_backupWidget;
	QProgressBar *m_backupProgress;
	int m_backupBlock;
	QFile *m_backupFile;
	enum device_state m_backupPrevState;

	QWidget *m_restoreWidget;
	QProgressBar *m_restoreProgress;
	int m_restoreBlock;
	QFile *m_restoreFile;

	QWidget *m_firmwareUpdateWidget;
	QProgressBar *m_firmwareUpdateProgress;
	QList<fwSection>::iterator m_writingSectionIter;
	unsigned int m_writingAddr;
	unsigned int m_writingSize;
	unsigned int m_totalWritten;
	QList<fwSection> m_fwSections;
	QLabel *m_firmwareUpdateStage;
	QTimer m_resetTimer;

	QWidget *m_uninitPrompt;

	QAction *m_backupAction;
	QAction *m_restoreAction;
	QAction *m_logoutAction;
	QAction *m_wipeDeviceAction;
	QAction *m_eraseDeviceAction;
	QAction *m_changePasswordAction;
	QAction *m_updateFirmwareAction;
	ButtonWaitDialog *m_buttonWaitDialog;
	QMessageBox *m_wipeDeviceDialog;
	int m_signetdevCmdToken;

	void sendFirmwareWriteCmd();

	void showEvent(QShowEvent *event);
#ifdef _WIN32
	bool nativeEvent(const QByteArray &eventType, void *message, long *result);
#endif
public:
	bool connected() const
	{
		return m_connected;
	}
private slots:
	void restoreError();
	void backupError();
public slots:
	void deviceOpened();
	void deviceClosed();
	void signetdevCmdResp(signetdevCmdRespInfo info);
	void signetdevGetProgressResp(signetdevCmdRespInfo info, signetdev_get_progress_resp_data data);
	void signetdevStartupResp(signetdevCmdRespInfo info, int device_state, QByteArray hashfn, QByteArray salt);
	void signetdevReadBlockResp(signetdevCmdRespInfo info, QByteArray block);

	void wipeDeviceDialogFinished(int code);
	void connectionError();
	void open();
	void abort();
	void quit();
	void operationFinished(int);
	void enterDeviceState(int);

	void logoutUi();
	void changePasswordUi();
	void eraseDeviceUi();
	void backupDeviceUi();
	void restoreDeviceUi();
	void updateFirmwareUi();
	void wipeDeviceUi();
	void resetTimer();
	void connectingTimer();
};

#endif // MAINWINDOW_H