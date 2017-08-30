#include "newgeneric.h"

#include <QBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

#include "databasefield.h"
#include "buttonwaitdialog.h"

extern "C" {
#include "signetdev.h"
}

#include "generic.h"
#include "generictypedesc.h"

#include "signetapplication.h"

NewGeneric::NewGeneric(int id, genericTypeDesc *typeDesc, const QString &name, QWidget *parent) :
	QDialog(parent),
	m_typeDesc(typeDesc),
	m_id(id),
	m_signetdevCmdToken(-1)
{
	setWindowModality(Qt::WindowModal);
	SignetApplication *app = SignetApplication::get();
	connect(app, SIGNAL(signetdev_cmd_resp(signetdevCmdRespInfo)),
	        this, SLOT(signetdevCmdResp(signetdevCmdRespInfo)));

	QBoxLayout *nameLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	m_nameField = new QLineEdit(name);
	nameLayout->addWidget(new QLabel("Name"));
	nameLayout->addWidget(m_nameField);

	QPushButton *createButton = new QPushButton("Create");
	connect(createButton, SIGNAL(pressed()), this, SLOT(createButtonPressed()));
	createButton->setDefault(true);

	for (auto field : m_typeDesc->fields) {
		DatabaseField *dbField = new DatabaseField(field, 250);
		m_typeFields.push_back(dbField);
	}

	setWindowTitle("New " + m_typeDesc->name);
	QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom);
	layout->setAlignment(Qt::AlignTop);
	layout->addLayout(nameLayout);
	for (auto dbField : m_typeFields) {
		layout->addWidget(dbField);
	}
	layout->addWidget(createButton);
	setLayout(layout);
}

void NewGeneric::createButtonPressed()
{
	QString action = "add bookmark \"" + m_nameField->text() + "\"";
	m_buttonWaitDialog = new ButtonWaitDialog("Add account", action, this);
	connect(m_buttonWaitDialog, SIGNAL(finished(int)), this, SLOT(addEntryFinished(int)));
	m_buttonWaitDialog->show();
	::signetdev_open_id_async(NULL, &m_signetdevCmdToken, m_id);
}

void NewGeneric::signetdevCmdResp(signetdevCmdRespInfo info)
{
	int code = info.resp_code;

	if (m_signetdevCmdToken != info.token) {
		return;
	}
	m_signetdevCmdToken = -1;

	if (m_buttonWaitDialog) {
		m_buttonWaitDialog->done(QMessageBox::Ok);
	}

	switch (code) {
	case OKAY: {
		switch (info.cmd) {
		case SIGNETDEV_CMD_OPEN_ID: {
			block blk;
			generic *g = new generic(m_id);
			g->name = m_nameField->text();
			g->typeName = m_typeDesc->name;
			m_entry = g;
			for (auto dbField : m_typeFields) {
				genericField field;
				field.name = dbField->name();
				field.value = dbField->text();
				g->fields.push_back(field);
			}
			for (auto dbField : m_extraFields) {
				genericField field;
				field.name = dbField->name();
				field.value = dbField->text();
				g->fields.push_back(field);
			}
			g->toBlock(&blk);;
			::signetdev_write_id_async(NULL, &m_signetdevCmdToken,
			                           g->id,
			                           blk.data.size(),
			                           (const u8 *)blk.data.data(),
			                           (const u8 *)blk.mask.data());
		}
		break;
		case SIGNETDEV_CMD_WRITE_ID:
			emit entryCreated(m_entry);
			close();
			break;
		}
	}
	break;
	case BUTTON_PRESS_TIMEOUT:
	case BUTTON_PRESS_CANCELED:
	case SIGNET_ERROR_DISCONNECT:
	case SIGNET_ERROR_QUIT:
		break;
	default: {
		emit abort();
		close();
	}
	break;
	}
}

void NewGeneric::addEntryFinished(int code)
{
	if (m_buttonWaitDialog)
		m_buttonWaitDialog->deleteLater();
	m_buttonWaitDialog = NULL;
	if (code != QMessageBox::Ok) {
		::signetdev_cancel_button_wait();
	}
}