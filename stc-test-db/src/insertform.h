#ifndef INSERTFORM_H
#define INSERTFORM_H

#include <QDialog>
#include <QSqlRecord>

namespace Ui {
class InsertForm;
}

class InsertForm : public QDialog
{
    Q_OBJECT

public:
    explicit InsertForm(QSqlRecord record, QWidget *parent = nullptr);
    ~InsertForm();
    QSqlRecord getRecord();

private slots:
    void on_btn_add();

private:
    Ui::InsertForm *ui;
    QSqlRecord record;
};

#endif // INSERTFORM_H
