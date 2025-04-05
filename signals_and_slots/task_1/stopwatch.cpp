#include "stopwatch.h"
#include <QDebug>

Stopwatch::Stopwatch(QObject *parent)
    : QObject(parent) {

    connect(&timer, &QTimer::timeout, this, &Stopwatch::elapsedTime);
    timer.setInterval(100);
}

void Stopwatch::start() {
    if (!elapsedTimer.isValid()) {
        elapsedTimer.start();
    } else {
        elapsedTimer.restart();
    }
    running = true;
    timer.start();
}

void Stopwatch::stop() {
    if (elapsedTimer.isValid()) {
        timer.stop();
        storedTime += elapsedTimer.elapsed();
    }
    running = false;
}

void Stopwatch::reset() {
    if (elapsedTimer.isValid()) {
        timer.stop();
        storedTime = 0;
        laps.clear();
    }
    running = false;
}

void Stopwatch::lap() {
    if (!elapsedTimer.isValid()) return;
    qint64 time = storedTime + elapsedTimer.elapsed();
    qint64 lapTime = time;

    if (!laps.isEmpty()) {
        lapTime -= laps.last().totalElapsed;
    }

    Lap newLap { time, lapTime, laps.length() + 1 };
    laps.append(newLap);

    emit lapsUpdated(laps);
}

void Stopwatch::elapsedTime() {
    qint64 time = storedTime + elapsedTimer.elapsed();
    int seconds = time / 1000;
    int ms = (time % 1000) / 60;

    QString formatedTime = QString("%1.%2")
                               .arg(seconds, 2, 10, QChar('0'))
                               .arg(ms, 2, 10, QChar('0'));


    emit timeUpdated(formatedTime);
}

bool Stopwatch::isRunning() {
    return running;
}








