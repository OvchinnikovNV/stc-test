#include "worker.h"

Worker::Worker(int id, QObject *parent)
    : QObject{parent},
     id (id)
{}

double Worker::operation(double value)
{
    return cbrt(cbrt(value) * rand()) / pow(rand(), 1.5) * pow(rand(), 1.1);
}

void Worker::doWork()
{
    stopped = false;

    srand(time(nullptr));
    double result = static_cast<double>(rand());

    for(int i = 0; i < COUNT; i++) {
        for(int k = 0; k < COUNT; k++) {
            if (stopped)
            {
                emit resultReady(id, "остановлен");
                return;
            }

            result = operation(result);
        }

        emit progressChanged(id, static_cast<int>(i / COUNT * 100));
    }

    emit resultReady(id, QString::number(result));
}

void Worker::stop()
{
    stopped = true;
}
