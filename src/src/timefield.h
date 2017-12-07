#ifndef TIMEFIELD_H
#define TIMEFIELD_H

#include <QObject>
#include <QString>

/**
 * @brief TimeField 时间域类，表示一个时间单位，如年、月、日等，
 * 每个时间域实例都有一个最大值，表示相应的时间域所能表示的最大值，如
 * 表示分钟的时间域最大值为60，表示天数的时间域最大值为365.每个时间
 * 域类实例都有一个时间域类指针指向自己上一个单位，如表示分钟
 * 的时间域实例中会包含一个表示小时的时间域实例，这个上级域的作用为：
 * 当时间域减小到0或增加到最大值时，向其借位或进位
 */
class TimeField : public QObject {
    Q_OBJECT
public:
    TimeField();
    TimeField(TimeField *superior, unsigned int maxValue, unsigned int timeValue = 0);
    ~TimeField() = default;
public:
    /**
     * @brief increase 增加时间域的值，步长为1，如果时间域的值达到最大，
     * 则进行进位操作，进位操作是一个级联操作，如果上一级在进位后也达到最大
     * 值，则上一级也将进行进位操作，直到成功或失败为止
     */
    void increase();
    /**
     * @brief decrease 减小时间域的值，步长为1，如果时间域的值为0，则进行
     * 借位操作，借位操作是一个级联操作，如果上一级时间域值也为0，则上一级也
     * 将进行借位操作，知道成功或失败为止
     */
    void decrease();
    /**
     * @brief clear 使时间域清零
     */
    void clear();

    void setTime();
    int getTime() const;
    void setTime(int time);
    QString toString() const;
    static QString toString(int time);
    int getMaxValue() const;

signals:
    void timeChanged();
    void timeUp();
private:
    TimeField* superior_;
    int timeValue_;
    const int MAX_VALUE;
    static QString timeString[100];

    /**
     * @brief carry 表示向上级时间域进位，此函数用于实现increase
     * @return 进位成功返回true，失败返回false
     */
    bool carry();
    /**
     * @brief borrow 表示向上级时间域借位，此函数用于实现decrease
     * @return
     */
    bool borrow();
};
#endif // TIMEFIELD_H
