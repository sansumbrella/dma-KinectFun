#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Surface.h"
#include "cinder/Camera.h"
#include "cinder/Utilities.h"
#include "cinder/qtime/MovieWriter.h"
#include "Kinect.h"

#include <vector>

using namespace ci;
using namespace ci::app;
using namespace std;

class KinectTerrainApp : public AppBasic {
  public:
	
	void keyDown( KeyEvent event );
	void prepareSettings( Settings* settings );
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	
private:
	void loadShaders();
	
	Kinect mKinect;
	CameraPersp mCam;
	
	gl::GlslProg mCompositeShader;
	
	int mFrameOffset, mFramesToDraw;
	
	vector<gl::Texture> mDepthSurfaces;
	vector<gl::Texture> mVideoSurfaces;
};

void KinectTerrainApp::prepareSettings( Settings* settings )
{
	settings->setWindowSize( 1280, 960 );
	settings->setFrameRate( 30.0f );
}

void KinectTerrainApp::setup()
{
	mKinect = Kinect( Kinect::Device() );
	
	loadShaders();
	
	mCam.setPerspective( 60.0f, getWindowAspectRatio(), 0.0f, 2000.0f );
	mCam.lookAt( Vec3f( 0.0f, 0.0f, 500.0f ), Vec3f::zero() );
	
	mFrameOffset = 24;
	mFramesToDraw = 8;
	
	for( int i = 0; i < mFrameOffset * mFramesToDraw; i++ )
	{
		mDepthSurfaces.push_back( gl::Texture( 640, 480 ) );
		mVideoSurfaces.push_back( gl::Texture( 640, 480 ) );
	}
}

void KinectTerrainApp::mouseDown( MouseEvent event )
{
}

void KinectTerrainApp::update()
{
	if( mKinect.checkNewDepthFrame() )
	{
		for( int i = mDepthSurfaces.size() - 1; i > 0; i-- )
		{
			mDepthSurfaces[i] = mDepthSurfaces[i-1];
		}
		
		mDepthSurfaces[0] = mKinect.getDepthImage();
	}
	
	if ( mKinect.checkNewVideoFrame() )
	{
		for( int i = mDepthSurfaces.size() - 1; i > 0; i-- )
		{
			mVideoSurfaces[i] = mVideoSurfaces[i-1];
		}
		
		mVideoSurfaces[0] = mKinect.getVideoImage();
	}
	
	
}

void KinectTerrainApp::draw()
{
	// clear out the window with black
	
	mCompositeShader.bind();
	
	for ( int i=0; i < mFramesToDraw; i++ )
	{
		mVideoSurfaces[i*mFrameOffset].bind( i );
		mDepthSurfaces[i*mFrameOffset].bind( (mFramesToDraw-1) + i );
	}
	
	int colors[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	int depths[8] = { 8, 9, 10, 11, 12, 13, 14, 15 };
	
	mCompositeShader.uniform( "colors", colors, mFramesToDraw );
	mCompositeShader.uniform( "depths", depths, mFramesToDraw );
		
	gl::drawSolidRect( Rectf( 0, 0, getWindowWidth(), getWindowHeight() ) );
//	gl::setMatrices( mCam );
}

void KinectTerrainApp::keyDown( KeyEvent event )
{
	switch ( event.getChar() ) {
		case 's':
			loadShaders();
			break;
		case 'i':
			mKinect.setVideoInfrared(true);
			break;
		case 'c':
			mKinect.setVideoInfrared(false);
			break;
		default:
			break;
	}
}

void KinectTerrainApp::loadShaders()
{
	try {
		console() << "Relaoding shaders, typo" << endl;
		mCompositeShader = gl::GlslProg( loadFile("../../../resources/Merge.vert"), loadFile("../../../resources/Merge.frag") );
	}
	catch (...) {
		console() << "Ran into problems" << endl;
	}
	
	
}

CINDER_APP_BASIC( KinectTerrainApp, RendererGl( RendererGl::AA_MSAA_4 ) )
