#include "quizapp.h"

quizapp::quizapp(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	ui.stackedWidget->setCurrentIndex(0);
    this->showMaximized();

	connect(ui.answer_text, &QLineEdit::returnPressed, this, &quizapp::handleInput);
}

quizapp::~quizapp()
{   
}
void quizapp::on_login_btn_clicked()
{
	ui.stackedWidget->setCurrentIndex(1);
}
void quizapp::on_back_btn_clicked()
{
	ui.stackedWidget->setCurrentIndex(0);
}
void quizapp::on_back_btn_2_clicked()
{
	ui.stackedWidget->setCurrentIndex(1);
}

void quizapp::on_submit_btn_clicked()
{
	handleAnswer();
	ui.stackedWidget->setCurrentIndex(2);
}
void quizapp::handleAnswer()
{
	if (ui.answer_B->isChecked())
	{
		ui.answer_label->setText("Your answer is: Correct!");
	}
	else
	{
		ui.answer_label->setText("Your answer is: Incorrect!");
	}
}
void quizapp::handleInput()
{
	QString text = ui.answer_text->text();
	ui.welcome_label->setText(text);
	ui.answer_text->clear();
	
}
