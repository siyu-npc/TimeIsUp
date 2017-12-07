#ifndef TIMERSETTINGWINDOW_H
#define TIMERSETTINGWINDOW_H

#include "src/timer.h"
#include <QMainWindow>

namespace Ui {
class TimerSettingWindow;
}

class TimerSettingWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TimerSettingWindow(QWidget *parent = 0);
    ~TimerSettingWindow();

protected:
    bool eventFilter(QObject *watched, QEvent *event);
    //void keyPressEvent(QKeyEvent *event);
public slots:
    void setYear(int);
    void setDay(int);
    void setHour(int);
    void setMin(int);
    void setSec(int);
    void setMSec(int);

    void setCountUpMode();
    void setCountDownMode();

    void start();
    void stop();
    void clear();
    void updateTime();
    void afterTimeUp();
    void record();
    void saveRecord();
    void clearRecord();
    void fullScreen();

    void showShortcutWindow();
    void showAboutWindow();
private:
    Ui::TimerSettingWindow *ui;
    Timer timer;
    Timer::TimerMode mode;

    void setMode(Timer::TimerMode m);
    /**以下几个函数用于设置图形界面在不同的状态下显示的模式的区别，
     * 这些状态包括：
     * 1. 初始状态：initState，即软件刚打开时的状态，此时只能进行定时器工作模式
     * 的选择
     * 2. 开始前状态：beforeStartState，在选择完工作模式，尚未开始计时前
     * 3. 运行状态：runningState，表示正在计时的状态，在此状态下，只能进行停止、全屏显示
     * 和记录数据的操作
     * 4. 停止后状态：afterStopState，表示停止或暂停计时后的状态，在此状态下，可以进行模式
     * 的选择、继续计时、保存/清楚记录等操作
     * 5. 切换工作模式后状态：afterModeChangedState，表示切换了工作模式后的状态，每切换
     * 一次工作模式，计时器都将被清零
     * 6. 时间到模式：afterTimeUpState，用于倒计时工作中，在时间到后，将重新恢复为开始前模式，
     * 但可以进行记录的保存和清楚工作
     * */
    void controlInitState();
    void controlBeforeStartState();
    void controlRunningState();
    void controlAfterStopState();
    void controlAfterModeChangedState();
    void controlAfterTimeUpState();
};

#endif // TIMERSETTINGWINDOW_H
