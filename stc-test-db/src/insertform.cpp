#include "insertform.h"
#include "ui_insertform.h"

InsertForm::InsertForm(QSqlRecord record, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsertForm),
    record(record)
{
    ui->setupUi(this);

    connect(ui->btn_add, SIGNAL(clicked()), this, SLOT(on_btn_add()));
    connect(ui->btn_cancel, SIGNAL(clicked()), this, SLOT(reject()));
}

InsertForm::~InsertForm()
{
    delete ui;
}

QSqlRecord InsertForm::getRecord()
{
    return record;
}

void InsertForm::on_btn_add()
{
    record.setValue("prefix", ui->line_prefix->text());
    record.setValue("parent_id", 0);
    record.setValue("name", ui->line_name->text().toUtf8());
    record.setValue("map_point", ui->line_point->text());
    record.setValue("lat", ui->dspin_lat->value());
    record.setValue("lon", ui->dspin_lon->value());
    record.setValue("alt", 0);
    record.setValue("population", ui->spin_population->value());
    record.setValue("description", ui->line_description->text());
    record.setValue("country", ui->line_country->text());
    accept();
}
