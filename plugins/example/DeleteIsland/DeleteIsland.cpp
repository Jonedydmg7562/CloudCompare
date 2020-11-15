//##########################################################################
//#                                                                        #
//#                CLOUDCOMPARE PLUGIN: DeleteIsland                      #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU General Public License as published by  #
//#  the Free Software Foundation; version 2 of the License.               #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#                             COPYRIGHT: XXX                             #
//#                                                                        #
//##########################################################################

// First:
//	Replace all occurrences of 'DeleteIsland' by your own plugin class name in this file.
//	This includes the resource path to info.json in the constructor.

// Second:
//	Open DeleteIsland.qrc, change the "prefix" and the icon filename for your plugin.
//	Change the name of the file to <yourPluginName>.qrc

// Third:
//	Open the info.json file and fill in the information about the plugin.
//	 "type" should be one of: "Standard", "GL", or "I/O" (required)
//	 "name" is the name of the plugin (required)
//	 "icon" is the Qt resource path to the plugin's icon (from the .qrc file)
//	 "description" is used as a tootip if the plugin has actions and is displayed in the plugin dialog
//	 "authors", "maintainers", and "references" show up in the plugin dialog as well

#include <QtGui>

#include "DeleteIsland.h"

//mine_add
#include<ccPointCloud.h>//基础点类

// Default constructor:
//	- pass the Qt resource path to the info.json file (from <yourPluginName>.qrc file) 
//  - constructor should mainly be used to initialize actions and other members
DeleteIsland::DeleteIsland( QObject *parent )
	: QObject( parent )
	, ccStdPluginInterface( ":/CC/plugin/DeleteIsland/info.json" )
	, m_action( nullptr )
{
}

// This method should enable or disable your plugin actions
// depending on the currently selected entities ('selectedEntities').
void DeleteIsland::onNewSelection( const ccHObject::Container &selectedEntities )
{
	if ( m_action == nullptr )
	{
		return;
	}
	
	// If you need to check for a specific type of object, you can use the methods
	// in ccHObjectCaster.h or loop and check the objects' classIDs like this:
	//
	//	for ( ccHObject *object : selectedEntities )
	//	{
	//		if ( object->getClassID() == CC_TYPES::VIEWPORT_2D_OBJECT )
	//		{
	//			// ... do something with the viewports
	//		}
	//	}
	
	// For example - only enable our action if something is selected.
	m_action->setEnabled( !selectedEntities.empty() );
}

// This method returns all the 'actions' your plugin can perform.
// getActions() will be called only once, when plugin is loaded.
QList<QAction *> DeleteIsland::getActions()
{
	// default action (if it has not been already created, this is the moment to do it)
	if ( !m_action )
	{
		// Here we use the default plugin name, description, and icon,
		// but each action should have its own.
		m_action = new QAction( getName(), this );
		m_action->setToolTip( getDescription() );
		m_action->setIcon( getIcon() );
		
		// Connect appropriate signal
		connect( m_action, &QAction::triggered, this, &DeleteIsland::doAction );
	}

	return { m_action };
}

// This is an example of an action's method called when the corresponding action
// is triggered (i.e. the corresponding icon or menu entry is clicked in CC's
// main interface). You can access most of CC's components (database,
// 3D views, console, etc.) via the 'm_app' variable (see the ccMainAppInterface
// class in ccMainAppInterface.h).
//void DeleteIsland::doAction_old()
//{	
//	if ( m_app == nullptr )
//	{
//		// m_app should have already been initialized by CC when plugin is loaded
//		Q_ASSERT( false );
//		
//		return;
//	}
//
//	/*** HERE STARTS THE ACTION ***/
//
//	// Put your code here
//	// --> you may want to start by asking for parameters (with a custom dialog, etc.)
//
//	// This is how you can output messages
//	// Display a standard message in the console
//	m_app->dispToConsole( "[DeleteIsland] Hello world!", ccMainAppInterface::STD_CONSOLE_MESSAGE );
//	m_app->dispToConsole("[DeleteIsland] Hello Aijun Shi!", ccMainAppInterface::STD_CONSOLE_MESSAGE);
//
//	// Display a warning message in the console
//	m_app->dispToConsole( "[DeleteIsland] Warning: example plugin shouldn't be used as is", ccMainAppInterface::WRN_CONSOLE_MESSAGE );
//	
//	// Display an error message in the console AND pop-up an error box
//	m_app->dispToConsole( "Example plugin shouldn't be used - it doesn't do anything!", ccMainAppInterface::ERR_CONSOLE_MESSAGE );
//
//	/*** HERE ENDS THE ACTION ***/
//}


void DeleteIsland::doAction()
{
	if (m_app == nullptr)
	{
		// m_app should have already been initialized by CC when plugin is loaded
		Q_ASSERT(false);

		return;
	}

	/*** HERE STARTS THE ACTION ***/

	/*********************************插入代码*****************************************/
	// 在这里插入需要的操作

	const ccHObject::Container& selected_objs = m_app->getSelectedEntities();// 获取选择的点云
	if (selected_objs.size() != 1)
	{
		m_app->dispToConsole("Please select one cloud!", ccMainAppInterface::ERR_CONSOLE_MESSAGE);
		return;
	}
	ccHObject* ent = selected_objs[0];
	assert(ent);
	if (!ent || !ent->isA(CC_TYPES::POINT_CLOUD))//判断所选的对象是否为点云
	{
		m_app->dispToConsole("Please select one cloud!", ccMainAppInterface::ERR_CONSOLE_MESSAGE);
		return;
	}
	// 插件功能：把选择的点云的x坐标增加100
	ccPointCloud* pc = static_cast<ccPointCloud*>(ent);

	//复制点云
	ccPointCloud* pc_new = new ccPointCloud("new cc");

	for (int i = 0; i < pc->size(); ++i)
	{
		pc_new->addPoint(CCVector3(pc->getPoint(i)->x + 30, pc->getPoint(i)->y, pc->getPoint(i)->z));
	}

	//需要将点云转换为generic格式，才可调用数据结构
	//ccGenericPointCloud gpc(pc_new);

	//需要利用kd树（cckdtree）或者八叉树（ccoctree）进行邻域搜索，达到邻域聚类滤波的目的



	ccHObject* cc_containers = new ccHObject("new-cc");
	cc_containers->addChild(pc_new);
	m_app->addToDB(cc_containers, true, true);
	m_app->refreshAll();
	/*********************************插入代码*****************************************/

	// This is how you can output messages
	// Display a standard message in the console
	m_app->dispToConsole("[DeleteIsland] plugin run successfully!", ccMainAppInterface::STD_CONSOLE_MESSAGE);

	//Display a warning message in the console
		//m_app->dispToConsole( "[DeleteIsland] Warning: example plugin shouldn't be used as is", ccMainAppInterface::WRN_CONSOLE_MESSAGE );
		//
		//Display an error message in the console AND pop - up an error box
		//m_app->dispToConsole( "Example plugin shouldn't be used - it doesn't do anything!", ccMainAppInterface::ERR_CONSOLE_MESSAGE );

		/*** HERE ENDS THE ACTION ***/
}