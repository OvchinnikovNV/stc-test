#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QProgressBar>
#include <QCloseEvent>
#include "worker.h"

#define N 50

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private:
    /*!
     * \brief Инициализация классов для рассчетов (Worker) и связывание их с потоками
     */
    void initThreads();

    /*!
     * \brief Инициализация таблицы с индикаторами прогресса
     */
    void initTable();

public slots:
    /*!
     * \brief Слот, вызываемый при окончании рассчета класса Worker
     * \param id - номер потока/строки
     * \param result - результат вычисления
     */
    void handleResults(int id, const QString &result);

    /*!
     * \brief Слот, вызываемый при изменении прогресса вычисления класса Worker
     * \param id - номер потока/строки
     * \param percent - процент выполнения
     */
    void handleProgress(int id, int percent);

private slots:
    void on_btn_runAll();
    void on_btn_stopAll();
    void on_btn_runOne();
    void on_btn_stopOne();

private:
    Ui::MainWindow *ui;
//    QThread threads[N];
    QList<QThread*> threads;
    QList<Worker*> workers;
    QList<QProgressBar*> progressBars;
};
#endif // MAINWINDOW_H
