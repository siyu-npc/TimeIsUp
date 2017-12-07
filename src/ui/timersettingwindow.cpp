#include "timersettingwindow.h"
#include "ui_timersettingwindow.h"
#include "fullsrceenwatch.h"
#include <QSpinBox>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QIODevice>
#include <QWidget>
#include <QEvent>

#include <QDebug>

TimerSettingWindow::TimerSettingWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TimerSettingWindow),
    timer(), mode(Timer::None)
{
    ui->setupUi(this);

    controlInitState();

    setYear(0);
    setDay(0);
    setHour(0);
    ui->colonLcd1->display(":");
    setMin(0);
    ui->colonLcd2->display(":");
    setSec(0);
    setMSec(0);

    /*ui->startButton->installEventFilter(this);
    ui->stopButton->installEventFilter(this);
    ui->recordButton->installEventFilter(this);
    ui->clearButton->installEventFilter(this);*/
    //ui->countDownRadioButton->installEventFilter(this);
    //ui->countUpRadioButton->installEventFilter(this);

    connect(&timer,SIGNAL(tick()),this,SLOT(updateTime()));
    connect(&timer,SIGNAL(timeUp()),this,SLOT(afterTimeUp()));
    connect(ui->startButton,SIGNAL(clicked()),this,SLOT(start()));
    connect(ui->stopButton,SIGNAL(clicked(bool)),this,SLOT(stop()));
    connect(ui->clearButton,SIGNAL(clicked(bool)),this,SLOT(clear()));
    connect(ui->recordButton,SIGNAL(clicked(bool)),this,SLOT(record()));
    connect(ui->saveRecordButton,SIGNAL(clicked(bool)),this,SLOT(saveRecord()));
    connect(ui->clearRecordButton,SIGNAL(clicked(bool)),this,SLOT(clearRecord()));
    connect(ui->showFullScreenButton,SIGNAL(clicked(bool)),this,SLOT(fullScreen()));

    connect(ui->countUpRadioButton,&QRadioButton::clicked,this,&TimerSettingWindow::setCountUpMode);
    connect(ui->countDownRadioButton,&QRadioButton::clicked,this,&TimerSettingWindow::setCountDownMode);
    connect(ui->yearSetSpinBox,SIGNAL(valueChanged(int)),this,SLOT(setYear(int)));
    connect(ui->daySetSpinBox,SIGNAL(valueChanged(int)),this,SLOT(setDay(int)));
    connect(ui->hourSetSpinBox,SIGNAL(valueChanged(int)),this,SLOT(setHour(int)));
    connect(ui->minSetSpinBox,SIGNAL(valueChanged(int)),this,SLOT(setMin(int)));
    connect(ui->secSetSpinBox,SIGNAL(valueChanged(int)),this,SLOT(setSec(int)));

    connect(ui->shortcutAction,SIGNAL(triggered(bool)),this,SLOT(showShortcutWindow()));
    connect(ui->aboutAction,SIGNAL(triggered(bool)),this,SLOT(showAboutWindow()));
}

TimerSettingWindow::~TimerSettingWindow()
{
    delete ui;
}

bool TimerSettingWindow::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent * keyEvent = static_cast<QKeyEvent*>(event);
        switch (keyEvent->key()) {
        case (Qt::Key_Space):
            if (ui->controlFrame->isEnabled()) {
                if (timer.isActive()) {
                    stop();
                    qDebug()<<"stop";
                    return true;
                } else {
                    qDebug()<<"starting";
                    this->start();
                    return true;
                }
            }
            break;
        case (Qt::Key_R) :
            if (!timer.isActive()) {
                clear();
                return true;
            }
            break;
        default:
            if (keyEvent->modifiers() == Qt::ControlModifier && keyEvent->key() == Qt::Key_C) {
                if (ui->clearRecordButton->isEnabled()) {
                    clearRecord();
                    return true;
                }
            } else if (keyEvent->modifiers() == Qt::ControlModifier && keyEvent->key() == Qt::Key_S) {
                if (ui->saveRecordButton->isEnabled()) {
                    saveRecord();
                    return true;
                }
            }
            break;
        }
    } else {
        return false;
    }
    return QWidget::eventFilter(watched, event);
}
void TimerSettingWindow::setYear(int year) {
    timer.setYear(year);
    ui->yearLcd->display(timer.getYearString());
}
void TimerSettingWindow::setDay(int day) {
    timer.setDay(day);
    ui->dayLcd->display(timer.getDayString());
}
void TimerSettingWindow::setHour(int hour) {
    timer.setHour(hour);
    ui->hourLcd->display(timer.getHourString());
}
void TimerSettingWindow::setMin(int min) {
    timer.setMin(min);
    ui->minLcd->display(timer.getMinString());
}
void TimerSettingWindow::setSec(int sec) {
    qDebug()<<"Sec : "<<sec;
    timer.setSec(sec);
    ui->secLcd->display(timer.getSecString());
}
void TimerSettingWindow::setMSec(int msec) {
    ui->msecLcd->display(TimeField::toString(msec));
}

void TimerSettingWindow::setMode(Timer::TimerMode m) {
    if (!ui->controlFrame->isEnabled()) {
        ui->controlFrame->setEnabled(true);
        controlBeforeStartState();
    }
    switch (m) {
    case Timer::CountUp:
        ui->countDownSetFrame->setEnabled(false);
        if (mode == Timer::CountDown) {
            timer.clear();
            updateTime();
            controlAfterModeChangedState();
        }
        break;
    case Timer::CountDown:
        ui->countDownSetFrame->setEnabled(true);
        if (mode == Timer::CountUp) {
            timer.clear();
            updateTime();
            controlAfterModeChangedState();
        }
    default:
        break;
    }
    mode = m;
    timer.setMode(mode);
}
void TimerSettingWindow::setCountUpMode() {
    setMode(Timer::CountUp);
}
void TimerSettingWindow::setCountDownMode() {
    setMode(Timer::CountDown);
}

void TimerSettingWindow::start() {
    if (mode != Timer::None) {
        controlRunningState();
        timer.start();
    }
}
void TimerSettingWindow::stop() {
    timer.stop();
    controlAfterStopState();
    if (ui->countDownRadioButton->isChecked()) {
        ui->countDownSetFrame->setEnabled(true);
    }
    if (!ui->recordBrowser->toPlainText().isEmpty()) {
        ui->saveRecordButton->setEnabled(true);
        ui->clearRecordButton->setEnabled(true);
    }
}
void TimerSettingWindow::clear() {
    timer.clear();
    updateTime();
    ui->startButton->setText(tr("开始计时"));
    ui->clearButton->setEnabled(false);
}

void TimerSettingWindow::updateTime() {
    setYear(timer.getYear());
    setDay(timer.getDay());
    setHour(timer.getHour());
    setMin(timer.getMin());
    setSec(timer.getSec());
    setMSec(timer.getMSec());
}
void TimerSettingWindow::afterTimeUp() {
    timer.stop();
    QMessageBox::information(this, tr("时间到"), tr("倒计时时间到！"));
    //this->setFocus();
    controlAfterTimeUpState();
}
void TimerSettingWindow::record() {
    static int lineNumber = 0;
    ui->recordBrowser->append(QString::number(++lineNumber) + "." + timer.toString());
}
void TimerSettingWindow::saveRecord() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存时间记录"), "./");
    if (!fileName.isEmpty()) {
        QFile recordFile(fileName);
        if (recordFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            recordFile.write(ui->recordBrowser->toPlainText().toLocal8Bit());
            recordFile.flush();
            recordFile.close();
        } else {
            QMessageBox::information(this, tr("文件无法打开"), QString("打开文件") + fileName + "时出现错误!");
        }
    }
}
void TimerSettingWindow::clearRecord() {
    ui->recordBrowser->clear();
    ui->saveRecordButton->setEnabled(false);
    ui->clearRecordButton->setEnabled(false);
}

void TimerSettingWindow::fullScreen() {
    static FullSrceenWatch * fullWinWatch = new FullSrceenWatch(this, &timer);
    connect(fullWinWatch,SIGNAL(started()),this,SLOT(start()),Qt::UniqueConnection);
    connect(fullWinWatch,SIGNAL(stoped()),this,SLOT(stop()),Qt::UniqueConnection);
    fullWinWatch->updateTime();
    fullWinWatch->showFullScreen();
}

void TimerSettingWindow::controlInitState() {
    ui->controlFrame->setEnabled(false);
}
void TimerSettingWindow::controlBeforeStartState() {
    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    ui->recordButton->setEnabled(false);
    ui->clearButton->setEnabled(false);
    ui->showFullScreenButton->setEnabled(true);
    ui->recordBrowser->setEnabled(false);
    ui->saveRecordButton->setEnabled(false);
    ui->clearRecordButton->setEnabled(false);
}
void TimerSettingWindow::controlRunningState() {
    ui->modeSelectBox->setEnabled(false);
    ui->startButton->setText(tr("继续"));
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    ui->recordButton->setEnabled(true);
    ui->clearButton->setEnabled(false);
    ui->showFullScreenButton->setEnabled(true);
    ui->recordBrowser->setEnabled(true);
    ui->saveRecordButton->setEnabled(false);
    ui->clearRecordButton->setEnabled(false);
}
void TimerSettingWindow::controlAfterStopState() {
    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    ui->recordButton->setEnabled(false);
    ui->clearButton->setEnabled(true);
    ui->showFullScreenButton->setEnabled(true);
    ui->recordBrowser->setEnabled(true);
    ui->saveRecordButton->setEnabled(true);
    ui->clearRecordButton->setEnabled(true);
    ui->modeSelectBox->setEnabled(true);
}
void TimerSettingWindow::controlAfterModeChangedState() {
    ui->startButton->setText(tr("计时开始"));
    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    ui->recordButton->setEnabled(false);
    ui->clearButton->setEnabled(false);
    ui->showFullScreenButton->setEnabled(true);
    ui->recordBrowser->setEnabled(false);
    ui->saveRecordButton->setEnabled(false);
    ui->clearRecordButton->setEnabled(false);
}
void TimerSettingWindow::controlAfterTimeUpState() {
    ui->startButton->setText(tr("计时开始"));
    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    ui->recordButton->setEnabled(false);
    ui->clearButton->setEnabled(false);
    ui->showFullScreenButton->setEnabled(true);
    ui->recordBrowser->setEnabled(false);
    ui->saveRecordButton->setEnabled(false);
    ui->clearRecordButton->setEnabled(false);
    ui->yearSetSpinBox->setValue(0);
    ui->daySetSpinBox->setValue(0);
    ui->hourSetSpinBox->setValue(0);
    ui->minSetSpinBox->setValue(0);
    ui->secSetSpinBox->setValue(0);
}
void TimerSettingWindow::showShortcutWindow() {
    static QMainWindow * shortWin = new QMainWindow(this);
    static QTextBrowser * shortBrowser = new QTextBrowser(this);
    if (shortBrowser->toPlainText().isEmpty()) {
        shortBrowser->append(QString("开始计时前请先选择计时模式！"));
        shortBrowser->append(QString("空格键\t\t开始/停止"));
        shortBrowser->append(QString("R键\t\t清零"));
        shortBrowser->append(QString("Ctrl+c\t\t清除记录"));
        shortBrowser->append(QString("Ctrl+s\t\t保存记录"));
    }
    shortWin->setCentralWidget(shortBrowser);
    shortWin->show();
}
void TimerSettingWindow::showAboutWindow() {
    static QString aboutText("作者：思雨\n邮箱：1036014410@qq.com");
    QMessageBox::about(this, "关于TimeIsUp", aboutText);
}
