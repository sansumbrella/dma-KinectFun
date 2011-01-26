#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Surface.h"
#include "cinder/Utilities.h"
#include "cinder/gl/Vbo.h"

#include "cinder/Camera.h"
#include "cinder/Quaternion.h"
#include "cinder/params/Params.h"

#include "Kinect.h"

#include <vector>

using namespace ci;
using namespace ci::app;
using namespace std;

static const int VBO_X_RES  = 640;
static const int VBO_Y_RES  = 480;

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
	void createVbo();
	
	Kinect		mKinect;
	CameraPersp mCam;
	Quatf		mSceneRotation;
	
	gl::GlslProg mDisplacementShader;
	gl::VboMesh mVboMesh;
	
	int mFrameOffset, mFramesToDraw;
	
	Surface		mDepthSurface;
	Surface		mScrollingTerrain;
	gl::Texture mTerrainTexture;
	params::InterfaceGl	mParams;
};

void KinectTerrainApp::prepareSettings( Settings* settings )
{
	settings->setWindowSize( 1280, 480 );
	settings->setFrameRate( 30.0f );
}

void KinectTerrainApp::setup()
{
	createVbo();
	loadShaders();
	
	mParams = params::InterfaceGl( "KinectPointCloud", Vec2i( 200, 180 ) );
	mParams.addParam( "Scene Rotation", &mSceneRotation, "opened=1" );
//	mParams.addParam( "Cam Distance", &mCameraDistance, "min=100.0 max=5000.0 step=100.0 keyIncr=s keyDecr=w" );
	
	mKinect = Kinect( Kinect::Device() );
	mKinect.setTilt( 0 );
	
	mCam.setPerspective( 75.0f, getWindowAspectRatio(), 1.0f, 8000.0f );
	mCam.lookAt( Vec3f( 0, 0, 1000.0f ), Vec3f( 0, 0, 0 ), Vec3f::yAxis() );
	
	mScrollingTerrain = Surface( 640, 480, false );
}

void KinectTerrainApp::createVbo()
{
	gl::VboMesh::Layout layout;
	
	layout.setStaticPositions();
	layout.setStaticTexCoords2d();
	layout.setStaticIndices();
	
	std::vector<Vec3f> positions;
	std::vector<Vec2f> texCoords;
	std::vector<uint32_t> indices; 
	
	int numVertices = VBO_X_RES * VBO_Y_RES;
	int numShapes	= ( VBO_X_RES - 1 ) * ( VBO_Y_RES - 1 );
	
	mVboMesh		= gl::VboMesh( numVertices, numShapes, layout, GL_POINTS );
	
	for( int x=0; x<VBO_X_RES; ++x ){
		for( int y=0; y<VBO_Y_RES; ++y ){
			indices.push_back( x * VBO_Y_RES + y );
			
			float xPer	= x / (float)(VBO_X_RES-1);
			float yPer	= y / (float)(VBO_Y_RES-1);
			
			positions.push_back( Vec3f( ( xPer * 2.0f - 1.0f ) * VBO_X_RES, ( yPer * 2.0f - 1.0f ) * VBO_Y_RES, 0.0f ) );
			texCoords.push_back( Vec2f( xPer, yPer ) );			
		}
	}
	
	mVboMesh.bufferPositions( positions );
	mVboMesh.bufferIndices( indices );
	mVboMesh.bufferTexCoords2d( 0, texCoords );
}

void KinectTerrainApp::mouseDown( MouseEvent event )
{
}

void KinectTerrainApp::update()
{
	if( mKinect.checkNewDepthFrame() )
	{
		mDepthSurface = mKinect.getDepthImage();
		
		float slitPosition = 240.0f;
		float slitHeight = 1.0f;
		
		mScrollingTerrain.copyFrom( mScrollingTerrain,
									Area( 0, 0, 640, 480 ), 
									Vec2i( 0, -1 ) );
		
		mScrollingTerrain.copyFrom( mDepthSurface,
									Area( 0, slitPosition, mDepthSurface.getWidth(), slitPosition + slitHeight ),
									Vec2i( 0, mScrollingTerrain.getHeight() - ( slitPosition + slitHeight ) ) );
		
		mTerrainTexture = gl::Texture( mScrollingTerrain );
	}
}

void KinectTerrainApp::draw()
{	
	gl::clear( Color::black() );
	
	if( mDepthSurface )
		gl::draw( gl::Texture( mDepthSurface ), Vec2f( 0, 0 ) );
	
	if( mScrollingTerrain )
		gl::draw( gl::Texture( mScrollingTerrain ), Vec2f( 640, 0 ) );
	
	
	gl::setMatrices( mCam );
	
	mDisplacementShader.bind();
	
	gl::pushMatrices();
	gl::scale( Vec3f( -1.0f, -1.0f, 1.0f ) );
	gl::rotate( mSceneRotation );
	
	if( mTerrainTexture )
		mTerrainTexture.bind( 0 );
	mDisplacementShader.uniform( "displacement", 0 );
	gl::draw( mVboMesh );
	
	gl::popMatrices();
	
	mDisplacementShader.unbind();
	
	gl::setMatricesWindow( getWindowSize() );
	params::InterfaceGl::draw();
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
	console() << "Reloading shaders" << endl;
	mDisplacementShader = gl::GlslProg( loadResource("displacement.vert"), loadResource("displacement.frag") );
}

CINDER_APP_BASIC( KinectTerrainApp, RendererGl( RendererGl::AA_MSAA_4 ) )
