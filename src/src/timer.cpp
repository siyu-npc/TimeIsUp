#include "timer.h"


Timer::Timer(TimerMode m, QObject* parent) : QObject(parent),
     timer(this), mode(m),
     year(nullptr, 100), day(&year, 365), hour(&day, 24),
     min(&hour, 60), sec(&min, 60), msec(&sec, 100)
{
    connect(&timer,&QTimer::timeout,this,&Timer::update);

    connect(&msec,&TimeField::timeChanged,[this](){emit tick();});
    connect(&msec,&TimeField::timeUp,[this](){stop();emit timeUp();});
}

void Timer::update() {
    switch (mode) {
    case CountUp:
        msec.increase();
        break;
    case CountDown:
        msec.decrease();
    default:
        break;
    }
}

void Timer::start(int msec) {
    if (mode != None) {
        timer.start(msec);
    }
}
void Timer::stop() {
    timer.stop();
}
void Timer::clear() {
    year.clear();
    day.clear();
    hour.clear();
    min.clear();
    sec.clear();
    msec.clear();
}
QString Timer::getYearString() const {
    if (year.getTime() < 100) return QString("0") + year.toString();
    return year.toString();
}
QString Timer::getDayString() const {
    if (day.getTime() < 100) return QString("0") + day.toString();
    return day.toString();
}
QString Timer::getHourString() const {
    return hour.toString();
}
QString Timer::getMinString() const {
    return min.toString();
}
QString Timer::getSecString() const {
    return sec.toString();
}
QString Timer::getMSecString() const {
    return msec.toString();
}
QString Timer::toString() const {
    return getYearString() + ":"
            + getDayString() + ":"
            + getHourString() + ":"
            + getMinString() + ":"
            + getSecString() + ":"
            + getMSecString();
}
bool Timer::isActive() const {
    return timer.isActive();
}
int Timer::getYear() const {
    return year.getTime();
}
int Timer::getDay() const {
    return day.getTime();
}
int Timer::getHour() const {
    return hour.getTime();
}
int Timer::getMin() const {
    return min.getTime();
}
int Timer::getSec() const {
    return sec.getTime();
}
int Timer::getMSec() const {
    return msec.getTime();
}
void Timer::setYear(int year) {
    this->year.setTime(year);
}
void Timer::setDay(int day) {
    this->day.setTime(day);
}
void Timer::setHour(int hour) {
    this->hour.setTime(hour);
}
void Timer::setMin(int min) {
    this->min.setTime(min);
}
void Timer::setSec(int sec) {
    this->sec.setTime(sec);
}

void Timer::setMode(TimerMode m) {
    mode = m;
}
