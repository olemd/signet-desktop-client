#ifndef DATABASEFIELD_H
#define DATABASEFIELD_H

#include <QWidget>
#include "signetapplication.h"

class CommThread;
class QLineEdit;
class QPushButton;
class ButtonWaitDialog;
struct signetdevCmdRespInfo;

class DatabaseField : public QWidget
{
	Q_OBJECT
	ButtonWaitDialog *m_buttonWait;
	QString m_name;
	int m_signetdevCmdToken;
	QLineEdit *m_fieldEdit;
	QVector<u16> m_keysToType;
public:
	explicit DatabaseField(const QString &name, int width, QWidget *middle = 0, QWidget *parent = 0);
	QString text() const;
	QString name() const
	{
		return m_name;
	}
	void setText(const QString &s);
signals:
	void editingFinished();
	void textEdited(QString);
public slots:
	void signetdevCmdResp(signetdevCmdRespInfo info);
	void typeFieldUi();
	void copyFieldUi();
	void typeFieldFinished(int code);
	void retryTypeData();
};

#endif // DATABASEFIELD_H
