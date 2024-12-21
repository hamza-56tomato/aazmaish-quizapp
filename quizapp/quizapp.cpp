#include "quizapp.h"

quizapp::quizapp(QWidget* parent)
    : QMainWindow(parent), teacherPortal(nullptr) // Initialize teacherPortal to nullptr
{
    ui.setupUi(this);
	ui.stackedWidget->setCurrentIndex(0);
    this->showMaximized();
	studentPortal = new StudentPortal(&ui, this);
	teacherPortal = new TeacherPortal(this);
}

quizapp::~quizapp()
{
    delete studentPortal;
    delete teacherPortal;
}
