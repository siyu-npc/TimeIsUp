#ifndef FULLSRCEENWATCH_H
#define FULLSRCEENWATCH_H

#include "../src/timer.h"
#include <QMainWindow>
#include <QKeyEvent>

namespace Ui {
class FullSrceenWatch;
}

class FullSrceenWatch : public QMainWindow
{
    Q_OBJECT

public:
    explicit FullSrceenWatch(QWidget *parent = 0, Timer* timer = nullptr);
    ~FullSrceenWatch();
public slots:
    void updateTime();
protected:
    void keyPressEvent(QKeyEvent *);
signals:
    void stoped();
    void started();
private:
    Ui::FullSrceenWatch *ui;
    Timer* timer_;
};

#endif // FULLSRCEENWATCH_H
