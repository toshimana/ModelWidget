#include "modelwidgettest.h"
#include "ModelWidget.h"

#include <QtGui>
#include <QTest>
#include <QDebug>

#include <opencv2/highgui/highgui.hpp>

void 
ModelWidgetTest::initTestCase()
{
	modelWidget = std::make_shared<ModelWidget>();
	modelWidget->show();
}


void 
ModelWidgetTest::testPointsNoColors()
{
	SpVertices vertices = std::make_shared<Vertices>();
	vertices->push_back( cv::Point3f( 0.0f, 0.0f, 0.0f ) );

	modelWidget->setVertices( vertices );
	QTest::qWait( 1 );
	cv::Mat image = modelWidget->getImage();

	cv::Point center( modelWidget->width() / 2, modelWidget->height() / 2 - 1);
	QCOMPARE( image.at<cv::Vec3b>( center ), cv::Vec3b( 255, 255, 255 ) );
}

void 
ModelWidgetTest::testPointsColors()
{
	SpVertices vertices = std::make_shared<Vertices>();
	vertices->push_back( cv::Point3f( 0.0f, 0.0f, 0.0f ) );

	SpColors colors = std::make_shared<Colors>();
	colors->push_back( cv::Vec3b( 0, 0, 255 ) ); // BGR

	modelWidget->setVertices( vertices );
	modelWidget->setColors( colors );
	modelWidget->repaint();
	QTest::qWait( 1 );
	cv::Mat image = modelWidget->getImage();

	cv::Point center( modelWidget->width() / 2, modelWidget->height() / 2 - 1 );
	cv::Vec3b color = image.at<cv::Vec3b>( center );
	QCOMPARE( color, cv::Vec3b( 0, 0, 255 ) );
}

void
ModelWidgetTest::testMesh()
{
	SpVertices vertices = std::make_shared<Vertices>();
	vertices->push_back( cv::Point3f( 0.0f, 0.0f, 0.0f ) );
	vertices->push_back( cv::Point3f( 0.5f, 0.0f, 0.0f ) );
	vertices->push_back( cv::Point3f( 0.5f, 0.5f, 0.0f ) );
	vertices->push_back( cv::Point3f( 0.0f, 0.5f, 0.0f ) );

	SpColors colors = std::make_shared<Colors>();
	cv::Vec3b c( 255, 0, 0 );
	colors->push_back( c ); // BGR
	colors->push_back( c ); // BGR
	colors->push_back( c ); // BGR
	colors->push_back( c ); // BGR

	SpMeshes meshes = std::make_shared<Meshes>();
	meshes->push_back( cv::Vec3i( 0, 1, 2 ) );
	meshes->push_back( cv::Vec3i( 2, 3, 0 ) );

	modelWidget->setVertices( vertices );
	modelWidget->setColors( colors );
	modelWidget->setMeshes( meshes );
	modelWidget->repaint();
	QTest::qWait( 1 );
	cv::Mat image = modelWidget->getImage();

	int width  = modelWidget->width();
	int height = modelWidget->height();

#define check(x,y,color) QCOMPARE( image.at<cv::Vec3b>(y,x), color );

	check(     width / 2,     height / 2 - 1, c );
	check(     width / 2,     height / 4 ,    c );
	check( 3 * width / 4 - 1, height / 4 ,    c );
	check( 3 * width / 4 - 1, height / 2 - 1, c );

#undef check
}