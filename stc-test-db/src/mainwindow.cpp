#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*! Проверка на наличие драйвера SQLITE */
    if (!QSqlDatabase::drivers().contains("QSQLITE")) {
        QMessageBox::critical(
                 nullptr,
                 "Ошибка",
                 "Отсутствует драйвер QSQLITE");
        return;
    }

    /*! Инициализация и открытие БД */
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/cities.db3");
    if ( !db.open() ) {
        QMessageBox::critical(
                    nullptr,
                    "Ошибка",
                    "Не удалось подключиться к БД");
        return;
    }

    /*! Инициализация модели, выбор таблицы */
    model = new MySqlTableModel();
    model->setTable("cities");
    model->select();
    model->setColumsNames();

    /*! Инициализация таблицы, скрытие ненужных столбцов */
    ui->tableView->setModel(model);
    QList<uint> hide_colums = {0, 1, 2, 5, 9};
    for(int i = 0; i < hide_colums.count(); i++)
        ui->tableView->hideColumn(hide_colums.at(i));
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    /*! Инициализация QComboBox фильтрами по населению */
    ui->cbox_population->addItems(population_filters);

    /*! Инициализация QComboBox полями для фильтрации */
    ui->cbox_filter->addItems(columns_en_ru.keys());

    /*! Подключение фильтрации при изменении фильтров */
    connect(ui->cbox_population, SIGNAL(currentTextChanged(QString)), this, SLOT(filtrateTable()));
    connect(ui->line_filter, SIGNAL(textEdited(QString)), this, SLOT(filtrateTable()));
    connect(ui->cbox_filter, SIGNAL(currentTextChanged(QString)), this, SLOT(filtrateTable()));

    connect(ui->btn_add, SIGNAL(clicked()), this, SLOT(on_btn_add()));
    connect(ui->btn_remove, SIGNAL(clicked()), this, SLOT(on_btn_remove()));
}

MainWindow::~MainWindow()
{
    db.close();
    delete model;
    delete ui;
}

const QString MainWindow::collectFilter()
{
    const QString f_population = QString("population > %1")
                                 .arg(population_map.value(ui->cbox_population->currentText(), 0));

    const QString f_field = QString("%1 LIKE '%2%'").arg(
                columns_en_ru.value(ui->cbox_filter->currentText()),
                ui->line_filter->text()
                );

    return QString("%1 AND %2").arg(f_population, f_field);
}

bool MainWindow::confirmMsgBox(int count)
{
    QMessageBox box(
                QMessageBox::Question,
                "Подтвердите удаление",
                QString("Вы действительно хотите удалить записи в количестве %1?").arg(count),
                QMessageBox::Yes | QMessageBox::No
                );
    box.setButtonText(QMessageBox::No, "Нет");
    box.setButtonText(QMessageBox::Yes, "Да");
    return box.exec() == QMessageBox::Yes;
}

void MainWindow::filtrateTable()
{
    model->setFilter( collectFilter() );
}

void MainWindow::on_btn_add()
{
    InsertForm form(model->record(), this);
    if ( form.exec() == QDialog::Rejected )
        return;

    if ( !db.transaction() )
    {
        ui->statusbar->showMessage("Ошибка базы данных", 5000);
        return;
    }



    if ( !model->insertRecord(-1, form.getRecord()) ) {
        ui->statusbar->showMessage("Не удалось добавить запись", 5000);
        db.rollback();
        return;
    }

    model->submitAll();
    db.commit();
    filtrateTable();
}

void MainWindow::on_btn_remove()
{
    QModelIndexList selected_rows = ui->tableView->selectionModel()->selectedRows();

    if ( selected_rows.count() < 1 )
        return;

    if ( !confirmMsgBox(selected_rows.count()) )
        return;

    if ( !db.transaction() )
    {
        ui->statusbar->showMessage("Не удалось удалить записи", 5000);
        return;
    }

    for(int i = 0; i < selected_rows.count(); i++)
    {
         if ( !model->removeRow( selected_rows.at(i).row() ) )
         {
             ui->statusbar->showMessage("Не удалось удалить записи", 5000);
             db.rollback();
             return;
         }
    }

    model->submitAll();
    db.commit();
    filtrateTable();
}

