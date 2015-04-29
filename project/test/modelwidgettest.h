#ifndef MODELWIDGETTEST_H
#define MODELWIDGETTEST_H

#include <QObject>
#include <memory>

class ModelWidget;

class ModelWidgetTest : public QObject
{
	Q_OBJECT

public:

private slots :
	void testPointsNoColors();
	void testPointsColors();
	void testMesh1();
	void testMeshNoColors();
	void testSpreadPoints();

private:
	std::shared_ptr<ModelWidget> modelWidget;
};

#endif // MODELWIDGETTEST_H
