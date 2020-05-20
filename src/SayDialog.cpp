#include "SayDialog.h"
#include "ui_SayDialog.h"

SayDialog::SayDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SayDialog)
{
    ui->setupUi(this);
    auto emitter = [this]{emit say(ui->lineEdit->text());};

    connect(ui->lineEdit, &QLineEdit::returnPressed, emitter);
    connect(ui->sayButton, &QPushButton::clicked, emitter);
}

SayDialog::~SayDialog()
{
    delete ui;
}
