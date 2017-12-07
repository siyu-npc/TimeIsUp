#include "../ui/timersettingwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TimerSettingWindow win;
    a.installEventFilter(&win);
    win.show();

    return a.exec();
}
