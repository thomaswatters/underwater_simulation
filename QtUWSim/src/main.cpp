#include <ros/ros.h>

#include <string>
#include <vector>

#include "ConfigXMLParser.h"

#include "UWSimUtils.h"

#include <QApplication>
#include "MainWindow.h"

using namespace std;

//#include "BulletPhysics.h"


int main(int argc, char *argv[])
{
	//osg::notify(osg::ALWAYS) << "UWSim; using osgOcean " << osgOceanGetVersion() << std::endl;

	boost::shared_ptr<osg::ArgumentParser> arguments(new osg::ArgumentParser(&argc,argv));
	arguments->getApplicationUsage()->setApplicationName(arguments->getApplicationName());
	arguments->getApplicationUsage()->setDescription(arguments->getApplicationName()+" is using osgOcean.");
	arguments->getApplicationUsage()->setCommandLineUsage(arguments->getApplicationName()+" [options] ...");
	arguments->getApplicationUsage()->addCommandLineOption("--windx <x>","Wind X direction.");
	arguments->getApplicationUsage()->addCommandLineOption("--windy <y>","Wind Y direction.");
	arguments->getApplicationUsage()->addCommandLineOption("--windSpeed <speed>","Wind speed.");
	arguments->getApplicationUsage()->addCommandLineOption("--isNotChoppy","Set the waves not choppy.");
	arguments->getApplicationUsage()->addCommandLineOption("--choppyFactor <factor>","How choppy the waves are.");
	arguments->getApplicationUsage()->addCommandLineOption("--crestFoamHeight <height>","How high the waves need to be before foam forms on the crest.");
	arguments->getApplicationUsage()->addCommandLineOption("--oceanSurfaceHeight <z>","Z position of the ocean surface in world coordinates.");
	arguments->getApplicationUsage()->addCommandLineOption("--disableShaders","Disable use of shaders for the whole application. Also disables most visual effects as they depend on shaders.");
	arguments->getApplicationUsage()->addCommandLineOption("--disableTextures","Disable use of textures by default. Can be toggled with the 't' key.");
	arguments->getApplicationUsage()->addCommandLineOption("--resw <width>","Set the viewer width resolution");
	arguments->getApplicationUsage()->addCommandLineOption("--resh <height>","Set the viewer height resolution");
	arguments->getApplicationUsage()->addCommandLineOption("--freeMotion","Sets the main camera to move freely");
	arguments->getApplicationUsage()->addCommandLineOption("--configfile","Indicate config file location (default: data/scenes/cirs.xml). The rest of the options override the values defined in this file.");
	arguments->getApplicationUsage()->addCommandLineOption("--v","Be verbose. (OSG notify level NOTICE)");
	arguments->getApplicationUsage()->addCommandLineOption("--vv","Be much verbose. (OSG notify level DEBUG)");
	arguments->getApplicationUsage()->addCommandLineOption("--dataPath <path>","Search for models in this path, besides the default ones");


	unsigned int helpType = 0;
	if ((helpType = arguments->readHelpType()))
	{
		arguments->getApplicationUsage()->write(std::cout, helpType);
		return 1;
	}

	//Default notify level
	osg::setNotifyLevel(osg::FATAL);
	if (arguments->read("--v")) osg::setNotifyLevel(osg::NOTICE);
	if (arguments->read("--vv")) osg::setNotifyLevel(osg::DEBUG_FP);

	// report any errors if they have occurred when parsing the program arguments->
	if (arguments->errors())
	{
		arguments->writeErrorMessages(std::cout);

		return 1;
	}
	std::string dataPath("");
	while( arguments->read("--dataPath",dataPath));
	//Add current folder to path
	osgDB::Registry::instance()->getDataFilePathList().push_back(std::string("."));
	//Add UWSim folders to path
	osgDB::Registry::instance()->getDataFilePathList().push_back(std::string(SIMULATOR_DATA_PATH)+"/scenes");
	osgDB::Registry::instance()->getDataFilePathList().push_back(std::string(SIMULATOR_DATA_PATH));
	osgDB::Registry::instance()->getDataFilePathList().push_back(std::string(SIMULATOR_ROOT_PATH));
	//Add dataPath folder to path
	if (dataPath!=std::string("")) {
		osgDB::Registry::instance()->getDataFilePathList().push_back(dataPath);
	}

	QApplication app(argc, argv);  
	ros::init(argc,argv,"QtUWSim");
	ros::start();
	MainWindow* window=new MainWindow(arguments);
	window->show();

	app.exec();
	
	return 0;
}