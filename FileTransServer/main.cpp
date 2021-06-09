#include <QCoreApplication>

int main(int argc, char **argv)
{
	QCoreApplication app(argc, argv);

	qDebug("hello");
	return app.exec();
}
