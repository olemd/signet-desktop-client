#ifndef ESDBGENERICMODULE_H
#define ESDBGENERICMODULE_H

#include <QObject>
#include <QList>

#include "esdbtypemodule.h"

class LoggedInWidget;

struct generic;
struct genericTypeDesc;

class esdbGenericModule : public esdbTypeModule
{
private:
	LoggedInWidget *m_parent;
	genericTypeDesc *m_typeDesc;
	esdbEntry *decodeEntry(int id, int revision, esdbEntry *prev, struct block *blk) const;
	bool m_userDefined;
public:
	EsdbActionBar *newActionBar();
	esdbGenericModule(genericTypeDesc *typeDesc, LoggedInWidget *parent, bool userDefined = false, bool plural = true);
};

#endif // ESDBGENERICMODULE_H
