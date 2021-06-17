#include <QApplication>
#include "vwtoolsetswindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    VWToolSetsWindow window;
    window.show();
    return QApplication::exec();
}
