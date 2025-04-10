#pragma once

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>
#include <QList>
#include <QPair>

struct Lap {
    qint64 totalElapsed;
    qint64 lapTime;
    int lapId;
};

class Stopwatch : public QObject {
    Q_OBJECT

public:
    Stopwatch(QObject *parent = 0);

    void start();
    void stop();
    void reset();
    Lap lap();

    bool isRunning();

private slots:
    void elapsedTime();

signals:
    void timeUpdated(const QString &formatedTime);
    // void lapsUpdated(;

private:
    QElapsedTimer elapsedTimer;
    QTimer timer;
    bool running = false;
    qint64 storedTime = 0;
    qint64 lastLap = 0;
};
