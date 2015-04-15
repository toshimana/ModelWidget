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
    void initTestCase();
	void testPointsNoColors();
	void testPointsColors();
	void testMesh();

private:
	std::shared_ptr<ModelWidget> modelWidget;
};

#endif // MODELWIDGETTEST_H
