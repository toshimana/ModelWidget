#include "ModelWidget.h"

#include <opencv2/imgproc/imgproc.hpp>

struct ModelWidget::Impl
{
	SpVertices vertices;
	SpColors   colors;
	SpMeshes   meshes;

	GLenum drawMode;

	Impl()
		: drawMode( GL_POINTS )
	{}
};

ModelWidget::ModelWidget( QWidget* parent )
	: QGLWidget( parent )
	, mImpl( std::make_unique<ModelWidget::Impl>())
{

}

ModelWidget::~ModelWidget()
{
}

cv::Mat 
ModelWidget::getImage( void )
{
	glReadBuffer( GL_FRONT );
	cv::Mat m( height(), width(), CV_8UC3 );
	glReadPixels( 0, 0, width(), height(), GL_BGR, GL_UNSIGNED_BYTE, m.data );

	cv::Mat dst;
	cv::flip( m, dst, 0 );

	return dst;
}


void
ModelWidget::setVertices( const SpVertices& vertices )
{
	mImpl->vertices = vertices;
}

void 
ModelWidget::setColors( const SpColors& colors )
{
	mImpl->colors = std::make_shared<Colors>( colors->size() );
	
	std::transform( colors->begin(), colors->end(), mImpl->colors->begin(),
		[]( const cv::Vec3b& elt ){ return cv::Vec3b( elt[2], elt[1], elt[0] ); } );
}

void 
ModelWidget::setMeshes( const SpMeshes& meshes )
{
	mImpl->meshes = meshes;
	mImpl->drawMode = GL_TRIANGLES;
}

void
ModelWidget::initializeGL( void )
{
	glClearColor( 0.0, 0.0, 0.0, 1.0 );
	glOrtho( 0.0, 1.0, 0.0, 1.0, -1.0, 1.0 );
}

void 
ModelWidget::resizeGL( int width, int height )
{
	glViewport( 0, 0, width, height );
	glLoadIdentity();
	glOrtho( -1.0, 1.0, -1.0, 1.0, -1.0, 1.0 );
}

void 
ModelWidget::paintGL( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glColor3f( 1.0f, 1.0f, 1.0f );

	switch ( mImpl->drawMode ){
	case GL_POINTS:{
		if ( mImpl->vertices ) {
			glEnableClientState( GL_VERTEX_ARRAY );
			if ( mImpl->colors ) glEnableClientState( GL_COLOR_ARRAY );

			glVertexPointer( 3, GL_FLOAT, 0, mImpl->vertices->data() );
			if ( mImpl->colors ) glColorPointer( 3, GL_UNSIGNED_BYTE, 0, mImpl->colors->data() );

			glDrawArrays( mImpl->drawMode, 0, mImpl->vertices->size() );

			if ( mImpl->colors) glDisableClientState( GL_COLOR_ARRAY );
			glDisableClientState( GL_VERTEX_ARRAY );
		}
	} break;

	case GL_TRIANGLES: {
		if ( mImpl->vertices ) {
			glEnableClientState( GL_VERTEX_ARRAY );
			if ( mImpl->colors ) glEnableClientState( GL_COLOR_ARRAY );

			glVertexPointer( 3, GL_FLOAT, 0, mImpl->vertices->data() );
			if ( mImpl->colors ) glColorPointer( 3, GL_UNSIGNED_BYTE, 0, mImpl->colors->data() );

			glDrawElements( mImpl->drawMode, 3*mImpl->meshes->size(), GL_UNSIGNED_INT, mImpl->meshes->data() );

			if ( mImpl->colors) glDisableClientState( GL_COLOR_ARRAY );
			glDisableClientState( GL_VERTEX_ARRAY );
		}
	} break;
	}


	glFlush();
}
