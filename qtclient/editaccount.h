#ifndef EDITACCOUNT_H
#define EDITACCOUNT_H

#include <QDialog>
#include "signetapplication.h"

class QLineEdit;
class QPushButton;
class ButtonWaitDialog;
class QString;
class CommThread;
class DatabaseField;
class PasswordEdit;
struct account;
struct block;

struct signetdevCmdRespInfo;

class EditAccount : public QDialog
{
	Q_OBJECT
	QLineEdit *m_accountNameEdit;
	DatabaseField *m_usernameField;
	PasswordEdit *m_passwordEdit;
	DatabaseField *m_urlField;
	DatabaseField *m_emailField;
	QPushButton *m_saveButton;
	QPushButton *m_browseUrlButton;
	QPushButton *m_undoChangesButton;
	account *m_acct;
	ButtonWaitDialog *m_buttonDialog;
	void setAccountValues();
	int m_signetdevCmdToken;
public:
	EditAccount(account *acct, QWidget *parent = 0);
	virtual ~EditAccount();
signals:
	void abort();
	void accountChanged(int id);
public slots:
	void signetdevCmdResp(signetdevCmdRespInfo info);
	void editAccountFinished(int);
	void browseUrl();
	void textEdited(QString);
	void closePressed();
	void savePressed();
	void undoChangesUi();
};

#endif // EDITACCOUNT_H