#pragma once
#include <Qobject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "ui_quizapp.h"
class StudentPortal : public QObject
{
	Q_OBJECT
public:
	StudentPortal(Ui::quizappClass* uiPtr, QObject* parent = nullptr);
	~StudentPortal();

private:
	Ui::quizappClass *ui;
	
	/*QNetworkAccessManager* networkManager;
	QNetworkReply* networkReply;*/

public slots:
	
};