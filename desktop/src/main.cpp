#include <QApplication>
#include <QStyleFactory>

#include "qt/settings/app_settings.h"
#include "qt/window/qt_window_system.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    AppSettings::instance.load();

    QtWindowSystem window;
    window.show();

    const int result = app.exec();

    AppSettings::instance.save();
    return result;
}
