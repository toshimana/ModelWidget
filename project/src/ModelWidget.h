#ifndef MODELWIDGET_H
#define MODELWIDGET_H

#include <QGLWidget>
#include <memory>
#include <opencv2/core/core.hpp>
 
#include "modelwidget_global.h"

typedef std::vector<cv::Point3f> Vertices;
typedef std::shared_ptr<Vertices> SpVertices;

typedef std::vector<cv::Vec3b> Colors;
typedef std::shared_ptr<Colors> SpColors;

typedef std::vector<cv::Vec3i> Meshes;
typedef std::shared_ptr<Meshes> SpMeshes;

class MODELWIDGET_EXPORT ModelWidget : public QGLWidget
{
	Q_OBJECT

public:
	ModelWidget( QWidget* parent = 0 );
	virtual ~ModelWidget();

	cv::Mat getImage( void );

	void setVertices( const SpVertices& vertices );
	void setColors( const SpColors& colors );
	void setMeshes( const SpMeshes& meshes );

protected:
	void initializeGL( void );
	void resizeGL( int, int );
	void paintGL( void );

private:
	struct Impl;
	std::unique_ptr<Impl> mImpl;
};

#endif // MODELWIDGET_H
