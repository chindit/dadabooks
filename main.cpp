#include "dadabooks.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    DadaBooks instance;
    instance.show();

    return app.exec();
}
