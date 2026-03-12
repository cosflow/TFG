#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

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

private:
    Ui::MainWindow *ui;
    void generateCollisionLabels();
    QVector <QLabel*> colLbls;

signals:
    void collisionsDetected(int vibId);

public slots:
    void moveCursor(QByteArray data);
};
#endif // MAINWINDOW_H
