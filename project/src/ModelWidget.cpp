#include "ModelWidget.h"

#include <opencv2/imgproc/imgproc.hpp>

namespace
{
	template <typename T>
	struct MinMax
	{
		T minValue;
		T maxValue;

		MinMax( void ) : minValue( std::numeric_limits<T>::max() ), maxValue( std::numeric_limits<T>::min() ){}
		void push( const T& value ) {
			if ( value < minValue ) minValue = value;
			if ( maxValue < value ) maxValue = value;
		}
	};
};

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

	// 最小値、最大値を計算
	std::vector<MinMax<float> > mm( 3 );
	for ( size_t index = 0, n = mImpl->vertices->size(); index < n;  ++index ) {
		mm[0].push( mImpl->vertices->at(index).x );
		mm[1].push( mImpl->vertices->at(index).y );
		mm[2].push( mImpl->vertices->at(index).z );
	}


	// 視体積
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 2*mm[0].minValue, 2*mm[0].maxValue, 2*mm[1].minValue, 2*mm[1].maxValue, 2*mm[2].minValue, 2*mm[2].maxValue );
	glMatrixMode( GL_MODELVIEW );

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
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( -1.0, 1.0, -1.0, 1.0, -1.0, 1.0 );
	glMatrixMode( GL_MODELVIEW );
}

void 
ModelWidget::resizeGL( int width, int height )
{
	glViewport( 0, 0, width, height );
}

void 
ModelWidget::paintGL( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glColor3f( 1.0f, 1.0f, 1.0f );

	if ( mImpl->vertices ) {
		glEnableClientState( GL_VERTEX_ARRAY );
		if ( mImpl->colors ) glEnableClientState( GL_COLOR_ARRAY );
		
		glVertexPointer( 3, GL_FLOAT, 0, mImpl->vertices->data() );
		if ( mImpl->colors ) glColorPointer( 3, GL_UNSIGNED_BYTE, 0, mImpl->colors->data() );
		
		switch ( mImpl->drawMode ) {
		case GL_POINTS:
			glDrawArrays( mImpl->drawMode, 0, mImpl->vertices->size() ); 
			break;
		case GL_TRIANGLES:			
			glDrawElements( mImpl->drawMode, 3 * mImpl->meshes->size(), GL_UNSIGNED_INT, mImpl->meshes->data() ); 
			break;
		}
		
		if ( mImpl->colors ) glDisableClientState( GL_COLOR_ARRAY );
		glDisableClientState( GL_VERTEX_ARRAY );
	}

	glFlush();
}

void 
ModelWidget::mousePressEvent( QMouseEvent* event )
{
}

void 
ModelWidget::mouseReleaseEvent( QMouseEvent* event )
{
}

void 
ModelWidget::mouseMoveEvent( QMouseEvent* event )
{
}

void 
ModelWidget::wheelEvent( QWheelEvent* event )
{
}
