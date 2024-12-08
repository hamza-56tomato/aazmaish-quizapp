#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_quizapp.h"

class quizapp : public QMainWindow
{
    Q_OBJECT

public:
    quizapp(QWidget *parent = nullptr);
    ~quizapp();

private:
    Ui::quizappClass ui;

private slots:
	void on_login_btn_clicked();
	void on_back_btn_clicked();
	void on_back_btn_2_clicked();
    void handleAnswer();
    void handleInput();
	void on_submit_btn_clicked();
};
