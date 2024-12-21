#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_quizapp.h"
#include "studentportal.h"
#include "teacherportal.h"
class quizapp : public QMainWindow
{
	Q_OBJECT

	public:
	quizapp(QWidget *parent = nullptr);
	~quizapp();

	private:
	Ui::quizappClass ui;
	StudentPortal* studentPortal;
	TeacherPortal* teacherPortal;

};
