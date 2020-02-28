#include "dadabooks.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QSplashScreen screen(QPixmap(":/programme/images/chargement.png"));
    screen.show();
    app.processEvents();

    DadaBooks instance;
    instance.show();

    screen.finish(&instance);
    return app.exec();
}
