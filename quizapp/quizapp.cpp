#include "quizapp.h"

quizapp::quizapp(QWidget* parent)
    : QMainWindow(parent), teacherPortal(nullptr) // Initialize teacherPortal to nullptr
{
    ui.setupUi(this);
	ui.stackedWidget->setCurrentIndex(0);
    this->showMaximized();
	studentPortal = new StudentPortal(&ui, this);
	teacherPortal = new TeacherPortal(this);
	auth = new Auth(this, &ui);
	auth->setAPIkey("AIzaSyDi8hYQRv509aVPL_H7lkMB3_7lUqEAzAQ");
	
}

quizapp::~quizapp()
{
    delete studentPortal;
    delete teacherPortal;
}

void quizapp::on_login_btn_clicked()
{
	QString email = ui.login_email_input->text();
	QString password = ui.login_password_input->text();
	auth->signIn(email, password);
}
void quizapp::on_goto_signup_btn_clicked()
{
	ui.stackedWidget->setCurrentWidget(ui.signUpPage);
}
void quizapp::on_student_btn_clicked()
{
	isTeacher = false;
	ui.stackedWidget->setCurrentWidget(ui.loginPage);
}
void quizapp::on_teacher_btn_clicked()
{
	isTeacher = true;
	ui.stackedWidget->setCurrentWidget(ui.loginPage);
}
void quizapp::on_login_back_btn_clicked()
{
	ui.stackedWidget->setCurrentWidget(ui.welcomePage);
}
void quizapp::on_signup_back_btn_clicked()
{
	ui.stackedWidget->setCurrentWidget(ui.loginPage);
}
void quizapp::on_signup_btn_clicked()
{
	QString email = ui.signup_email_input->text();
	QString password = ui.signup_password_input->text();
	auth->signUp(email, password);
}
void quizapp::on_signup_success_back_btn_clicked()
{
	ui.stackedWidget->setCurrentWidget(ui.signUpPage);
}
