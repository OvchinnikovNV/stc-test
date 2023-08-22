#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->btn_runAll, &QPushButton::clicked, this, &MainWindow::on_btn_runAll);
    connect(ui->btn_stopAll, &QPushButton::clicked, this, &MainWindow::on_btn_stopAll);
    connect(ui->btn_runOne, &QPushButton::clicked, this, &MainWindow::on_btn_runOne);
    connect(ui->btn_stopOne, &QPushButton::clicked, this, &MainWindow::on_btn_stopOne);

    initThreads();
    initTable();
}

MainWindow::~MainWindow()
{
    for(int i = 0; i < N; i++)
    {
        threads[i]->quit();
        threads[i]->wait();
        delete workers[i];
        delete threads[i];
    }
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    /*! Остановка всех потоков при закрытии приложения */
    on_btn_stopAll();
    event->accept();
}

void MainWindow::initThreads()
{
    for(int i = 0; i < N; i++)
    {
        QThread *thread = new QThread;
        threads.append(thread);

        Worker *worker = new Worker(i);
        worker->moveToThread(threads[i]);
        connect(threads[i], &QThread::started, worker, &Worker::doWork);
        connect(worker, &Worker::resultReady, this, &MainWindow::handleResults);
        connect(worker, &Worker::progressChanged, this, &MainWindow::handleProgress);
        workers.append(worker);
    }
}

void MainWindow::initTable()
{
    if ( workers.count() != N )
        return;

    ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->table->setRowCount(N);

    for(int i = 0; i < N; i++)
    {
        QProgressBar *bar = new QProgressBar(this);
        progressBars.append(bar);

        QTableWidgetItem *item = new QTableWidgetItem(QString::number(i+1));
        item->setTextAlignment(Qt::AlignHCenter);
        ui->table->setItem(i, 0, item);

        ui->table->setCellWidget(i, 1, bar);

        item = new QTableWidgetItem("-");
        item->setTextAlignment(Qt::AlignHCenter);
        ui->table->setItem(i, 2, item);
    }
}

void MainWindow::handleResults(int id, const QString &result)
{
    if ( N <= id )
        return;

    threads[id]->quit();
    threads[id]->wait();

    if (result == "остановлен")
        progressBars[id]->setValue(0);
    else
        progressBars[id]->setValue(100);

    ui->table->item(id, 2)->setText(result);
}

void MainWindow::handleProgress(int id, int percent)
{
    if ( progressBars.count() <= id )
        return;

    progressBars[id]->setValue(percent);
}

void MainWindow::on_btn_runAll()
{
    for(int i = 0; i < N; i++) {
        ui->table->item(i, 2)->setText("-");

        if ( !threads[i]->isRunning() )
            threads[i]->start();
    }
}

void MainWindow::on_btn_stopAll()
{
    for(int i = 0; i < N; i++)
        if ( threads[i]->isRunning() ) {
            workers[i]->stop();
            threads[i]->quit();
            threads[i]->wait();
        }
}

void MainWindow::on_btn_runOne()
{
    QList<QTableWidgetItem*> selectedItems = ui->table->selectedItems();
    if ( selectedItems.count() < 1 )
        return;

    int id = selectedItems.at(0)->row();
    if ( !threads[id]->isRunning() ) {
        ui->table->item(id, 2)->setText("-");
        threads[id]->start();
    }
}

void MainWindow::on_btn_stopOne()
{
    QList<QTableWidgetItem*> selectedItems = ui->table->selectedItems();
    if ( selectedItems.count() < 1 )
        return;

    int id = selectedItems.at(0)->row();
    if ( threads[id]->isRunning() ) {
        workers[id]->stop();
        threads[id]->quit();
        threads[id]->wait();
    }
}

