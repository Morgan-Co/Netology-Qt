#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include "stopwatch.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void toggleStartStop();
    void updateDisplay(const QString &time);
    void updateLaps(const QList<Lap> &laps);
private:
    Ui::MainWindow *ui;
    Stopwatch stopwatch;
};
#endif // MAINWINDOW_H
