#include "teacherportal.h"

TeacherPortal::TeacherPortal(QObject* parent)
	: QObject(parent)
{

}
TeacherPortal::~TeacherPortal()
{
}
void TeacherPortal::setUi(Ui::quizappClass* uiPtr)
{
	ui = uiPtr;
}