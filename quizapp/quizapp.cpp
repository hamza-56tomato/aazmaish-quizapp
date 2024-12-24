#include "quizapp.h"
#include <QMessageBox>
quizapp::quizapp(QWidget* parent)
    : QMainWindow(parent), teacherPortal(nullptr) // Initialize teacherPortal to nullptr
{
    ui.setupUi(this);
	ui.stackedWidget->setCurrentIndex(0);
    this->showMaximized();
	isTeacher = false;
	
	
	auth = new Auth(this, &ui);
	auth->setAPIkey("AIzaSyDi8hYQRv509aVPL_H7lkMB3_7lUqEAzAQ");
	connect(ui.teacher_code_btn, &QPushButton::clicked, this, &quizapp::teacherCodeVerification);
	connect(ui.teacher_code_input, &QLineEdit::returnPressed, this, &quizapp::teacherCodeVerification);
	connect(auth, &Auth::userSignedIn, this, &quizapp::on_userSignedIn);
}

quizapp::~quizapp()
{
    delete studentPortal;
    if(teacherPortal != nullptr) delete teacherPortal;
	delete auth;
}

void quizapp::on_userSignedIn(QString idToken_p)
{
	idToken = idToken_p;
	if (isTeacher) {
		ui.stackedWidget->setCurrentWidget(ui.teacherPortal);
		teacherPortal = new TeacherPortal(this, &ui);
		teacherPortal->setIDToken(idToken);
	}
	else {
		ui.stackedWidget->setCurrentWidget(ui.studentPortal);
		studentPortal = new StudentPortal(this, &ui);
		studentPortal->setIDToken(idToken);
	}
}

void quizapp::on_login_btn_clicked()
{
	QString email = ui.login_email_input->text();
	QString password = ui.login_password_input->text();
	ui.login_password_input->clear();
	ui.login_email_input->clear();
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
	ui.stackedWidget->setCurrentWidget(ui.teacherCodePage);
}
void quizapp::teacherCodeVerification()
{
	QString code = ui.teacher_code_input->text();
	ui.teacher_code_input->clear();
	if (code == "1234") {
		isTeacher = true;
		ui.stackedWidget->setCurrentWidget(ui.loginPage);
	}
	else {
		QMessageBox::warning(nullptr, "Error", "Invalid code");
		ui.stackedWidget->setCurrentWidget(ui.welcomePage);
	}
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
	ui.signup_email_input->clear();
	ui.signup_password_input->clear();
	auth->signUp(email, password);
}
void quizapp::on_signup_success_back_btn_clicked()
{
	ui.stackedWidget->setCurrentWidget(ui.signUpPage);
}
