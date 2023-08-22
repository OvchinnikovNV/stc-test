#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <cmath>
#include <ctime>

#define COUNT 2000.0

class Worker : public QObject
{
    Q_OBJECT

public:
    explicit Worker(int id, QObject *parent = nullptr);

private:
    /*!
     * \brief Сложное математическое выражение
     * \param value - произвольное число
     * \return результат сложного математического выражения
     */
    double operation(double value);

public slots:
    /*!
     * \brief Слот, начинающий цикл рассчета сложного математического выражения
     */
    void doWork();

    /*!
     * \brief Слот, останавливающий процесс вычеслений
     */
    void stop();

signals:
    /*!
     * \brief Сигнал окончания вычислений
     * \param id - идентификатор потока
     * \param result - результат вычислений
     */
    void resultReady(int id, const QString &result);

    /*!
     * \brief Сигнал изменения прогресса вычислений
     * \param id - идентификатор потока
     * \param percent - процент выполнения
     */
    void progressChanged(int id, int percent);

private:
    /*!
     * \brief Идентификатор потока
     */
    int id = 0;

    /*!
     * \brief Флаг остановки вычислений
     */
    bool stopped = false;
};

#endif // WORKER_H
