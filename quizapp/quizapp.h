#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_quizapp.h"
#include "studentportal.h"
#include "teacherportal.h"
#include "auth.h"
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
	Auth* auth;
	bool isTeacher;
	QString idToken;

public slots: 
	void on_student_btn_clicked();
	void on_teacher_btn_clicked();
	void on_login_btn_clicked();
	void on_goto_signup_btn_clicked();
	void on_login_back_btn_clicked();
	void on_signup_back_btn_clicked();
	void on_signup_btn_clicked();
	void on_signup_success_back_btn_clicked();
	void on_userSignedIn(QString idToken);
	void teacherCodeVerification();
};
