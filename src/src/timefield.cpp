#include "timefield.h"

QString TimeField::timeString[100] = {
    "00","01","02","03","04","05","06","07","08","09",
    "10","11","12","13","14","15","16","17","18","19",
    "20","21","22","23","24","25","26","27","28","29",
    "30","31","32","33","34","35","36","37","38","39",
    "40","41","42","43","44","45","46","47","48","49",
    "50","51","52","53","54","55","56","57","58","59",
    "60","61","62","63","64","65","66","67","68","69",
    "70","71","72","73","74","75","76","77","78","79",
    "80","81","82","83","84","85","86","87","88","89",
    "90","91","92","93","94","95","96","97","98","99",
};

int TimeField::getTime() const {
    return timeValue_;
}
void TimeField::setTime(int time) {
    timeValue_ = time;
}
QString TimeField::toString(int time) {
    if (time < 100) {
        return timeString[time];
    } else {
        return QString::number(time);
    }
}

TimeField::TimeField() : QObject(), superior_(nullptr), timeValue_(0), MAX_VALUE(0) {}
TimeField::TimeField(TimeField *superior, unsigned int maxValue, unsigned int timeValue)
    : QObject(), superior_(superior), timeValue_(timeValue), MAX_VALUE(maxValue) {

}

void TimeField::increase() {
    ++timeValue_;
    if (timeValue_ >= MAX_VALUE) {
        if (carry()) {
            emit timeChanged();
        }
    } else {
        emit timeChanged();
    }
}
void TimeField::decrease() {
    if (timeValue_ == 0) {
        if (borrow()) {
            emit timeChanged();
        } else {            
            emit timeUp();
        }
    } else {
        --timeValue_;
        emit timeChanged();
    }
}
void TimeField::clear() {
    timeValue_ = 0;
}

QString TimeField::toString() const {
    return toString(timeValue_);
}
int TimeField::getMaxValue() const {
    return MAX_VALUE;
}
bool TimeField::carry() {
    if (superior_ == nullptr) return false;
    if (superior_->getTime() < superior_->getMaxValue()) {
        superior_->increase();
        timeValue_ = 0;
        return true;
    }
    return superior_->carry();
}

bool TimeField::borrow() {
    if (superior_ == nullptr) return false;
    if (superior_->getTime() > 0) {
        superior_->decrease();
        timeValue_ = MAX_VALUE;
        return true;
    }
    return superior_->borrow();
}

