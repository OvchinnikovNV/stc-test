#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QCoreApplication>
#include <QMessageBox>

#include <QSqlError>

#include "mysqltablemodel.h"
#include "insertform.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /*!
     * \brief Собирает строку для метода QSqlTableModel::setFilter из фильтров: по населению и по одному из полей
     * \return QString для QSqlTableModel::setFilter
     */
    const QString collectFilter();

    /*!
     * \brief Вызывает QMessageBox - подтверждение для удаления
     * \param count - количество выделенных записей для удаления
     * \return true - если удаление подтвержено, иначе - false
     */
    bool confirmMsgBox(int count);

private slots:
    /*!
     * \brief Слот, применяющий QSqlTableModel::setFilter к модели со значением от this->collectFilter
     */
    void filtrateTable();

    /*!
     * \brief Слот для кнопки добавления записи
     */
    void on_btn_add();

    /*!
     * \brief Слот для кнопки удаления записей
     */
    void on_btn_remove();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    MySqlTableModel *model = nullptr;

    /*! Строка с sql фильтром */
    QString filter;

    /*! Индекс столбца -> название столбца */
    QMap<uint, QString> column_names;

    /*! Название столбца -> название на русском */
    QMap<QString, QString> columns_en_ru = {
        {"Префикс", "prefix"},
        {"Название", "name"},
        {"Точка привязки", "map_point"},
        {"Население", "population"},
        {"Страна", "country"},
        {"Широта", "lat"},
        {"Долгота", "lon"},
        {"Описание", "description"}
    };

    /*! QStringList для cbox_population с фильтрами в нужном опрядке */
    QStringList population_filters = {
        "Население не учитывать",
        "Население от 1 тыс. чел.",
        "Население от 10 тыс. чел.",
        "Население от 25 тыс. чел.",
        "Население от 50 тыс. чел.",
        "Население от 100 тыс. чел.",
        "Население от 250 тыс. чел.",
        "Население от 500 тыс. чел.",
        "Население от 1 млн. чел.",
        "Население от 2 млн. чел.",
        "Население от 4 млн. чел.",
        "Население от 8 млн. чел.",
        "Население от 16 млн. чел."
    };

    /*! Фильтр -> значение населения */
    QMap<QString, uint> population_map = {
        {"Население не учитывать", 0},
        {"Население от 1 тыс. чел.", 1000},
        {"Население от 10 тыс. чел.", 10000},
        {"Население от 25 тыс. чел.", 25000},
        {"Население от 50 тыс. чел.", 50000},
        {"Население от 100 тыс. чел.", 100000},
        {"Население от 250 тыс. чел.", 250000},
        {"Население от 500 тыс. чел.", 500000},
        {"Население от 1 млн. чел.", 1000000},
        {"Население от 2 млн. чел.", 2000000},
        {"Население от 4 млн. чел.", 4000000},
        {"Население от 8 млн. чел.", 8000000},
        {"Население от 16 млн. чел.", 16000000},
    };
};
#endif // MAINWINDOW_H
