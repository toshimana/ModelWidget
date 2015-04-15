#include <QApplication>
#include <QTest>
#include "modelwidgettest.h"

int main( int argc, char* argv[] )
{
	QApplication app( argc, argv );
	ModelWidgetTest m;
	QTest::qExec( &m );
}