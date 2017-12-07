#include "fullsrceenwatch.h"
#include "ui_fullsrceenwatch.h"

#include <QDebug>

FullSrceenWatch::FullSrceenWatch(QWidget *parent, Timer * timer) :
    QMainWindow(parent),
    ui(new Ui::FullSrceenWatch), timer_(timer)
{
    ui->setupUi(this);
    ui->minLcd->display(timer_->getMinString());
    ui->colonLcd1->display(":");
    ui->secLcd->display(timer_->getSecString());
    ui->colonLcd2->display(":");
    ui->msecLcd->display(timer_->getMSecString());
    ui->hourLcd->display(timer_->getHourString());
    ui->yearLcd->display(timer_->getYearString());
    ui->dayLcd->display(timer_->getDayString());

    connect(timer_,&Timer::tick,this,&FullSrceenWatch::updateTime);
}

FullSrceenWatch::~FullSrceenWatch()
{
    delete ui;
}

void FullSrceenWatch::updateTime() {
    ui->yearLcd->display(timer_->getYearString());
    ui->dayLcd->display(timer_->getDayString());
    ui->hourLcd->display(timer_->getHourString());
    ui->minLcd->display(timer_->getMinString());
    ui->secLcd->display(timer_->getSecString());
    ui->msecLcd->display(timer_->getMSecString());
}

void FullSrceenWatch::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
    case (Qt::Key_Space):
        if (timer_->isActive()) {
            timer_->stop();
            emit stoped();
        } else {
            qDebug()<<"starting";
            timer_->start();
            emit started();
        }
        break;
    case (Qt::Key_Enter):        
    case (Qt::Key_Return):
        if (!timer_->isActive()) {
            timer_->clear();
            updateTime();
        }
        break;
    case (Qt::Key_Escape):
        close();
        break;
    default:
        break;
    }
}
