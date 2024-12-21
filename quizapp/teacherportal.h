#pragma once

#include <QWidget>
#include "ui_quizapp.h"
namespace Ui {
    class TeacherPortal;
}

class TeacherPortal : public QObject
{
    Q_OBJECT

public:
    TeacherPortal(QObject* parent = nullptr);
    ~TeacherPortal();
    void setUi(Ui::quizappClass* uiPtr);

private:
    Ui::quizappClass* ui;
};