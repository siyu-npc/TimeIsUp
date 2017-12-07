#ifndef TIMER_H
#define TIMER_H

#include "timefield.h"
#include <QObject>
#include <QTimer>
#include <QString>

/**
 * @brief Timer即定时器类，它完成定时的功能，其内部使用一个QTimer
 * 作为时间信号发射器，默认每10ms为一个tick（滴答）。在其中也组合了
 * year、day、hour、min、sec、msec 6个TimeField类的实例，分别
 * 代表不同的时间域，Timer最多可以满足999年364天99小时99分钟99秒
 * 钟的定时要求（正计时和倒计时）
 */
class Timer : public QObject
{
    Q_OBJECT
public:
    enum TimerMode {None, CountDown, CountUp};
    /**
     * @brief setMode设置Timer的工作模式
     * @param m 工作模式
     */

    /**
     * @brief Timer类的构造函数
     * @param m 表示工作模式
     * @param parent表示父类对象，Timer继承自QObject，所以此处是
     * Qt的习惯用法
     */
    Timer(TimerMode m = None, QObject *parent = nullptr);
    /**
     * @brief TimerMode enum指示Timer的工作模式，None为初始默认
     * 状态，CountDown表示倒计时模式，CountUp表示正计时模式
     */

    void setMode(TimerMode m);

    /**
     * @brief getYearString返回表示年数的字符串
     * @return 表示年数的字符串
     */
    QString getYearString() const;
    /**
     * @brief getDayString返回表示天数的字符串
     * @return 表示天数的字符串
     */
    QString getDayString() const;
    /**
     * @brief getHourString返回表示小时数的字符串
     * @return 表示小时数的字符串
     */
    QString getHourString() const;
    /**
     * @brief getMinString返回表示分钟数的字符串
     * @return 表示分钟数的字符串
     */
    QString getMinString() const;
    /**
     * @brief getSecString返回表示秒数的字符串
     * @return 表示秒数的字符串
     */
    QString getSecString() const;
    /**
     * @brief getMSecString返回表示毫秒数的字符串
     * @return 表示毫秒数的字符串
     */
    QString getMSecString() const;
    /**
     * @brief toString返回一个字符串，字符串的格式为：
     * yyy:ddd:hh:mm:SS:ss，即年:天:小时:分钟:秒:毫秒
     * @return Timer当前计时时间格式化后的字符串
     */
    QString toString() const;

    /**
     * @brief isActive指示当前Timer是否处于工作状态
     * @return 处于工作状态返回真，否则返回假
     */
    bool isActive() const;

    int getYear() const;
    int getDay() const;
    int getHour() const;
    int getMin() const;
    int getSec() const;
    int getMSec() const;
    void setYear(int year);
    void setDay(int day);
    void setHour(int hour);
    void setMin(int min);
    void setSec(int sec);

public slots:
    /**
     * @brief update 每一个tick后更新当前已计时时间
     */
    void update();
    /**
     * @brief start 开始计时，默认的计时间隔即tick为10ms
     * @param msec 计时间隔
     */
    void start(int msec = 10);
    /**
     * @brief stop 停止计时
     */
    void stop();
    /**
     * @brief clear 计时器清零
     */
    void clear();
signals:
    /**
     * @brief timeUp 表示时间到，用于倒计时模式中
     */
    void timeUp();
    /**
     * @brief tick 每一个滴答（默认10ms）发送一个tick信号
     */
    void tick();

private:
    QTimer timer;
    TimerMode mode;
    TimeField year, day, hour, min, sec, msec;
};

#endif // TIMER_H
