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
#include "DeleteIslandDialog.h"

#include <vector>


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


void get_point_around(ccPointCloud* points, std::vector<bool>& isSelected, std::vector<int>& pointIdSet, const int imageId, const int imageWidth,
	const int imageHeight, const int windowRange, const float disThread, const bool isDepth)
{
	int halfWindow = windowRange >> 1;
	int winY = imageId / imageWidth; int winX = imageId - winY * imageWidth;
	const CCVector3 *pointmiddle = points->getPoint(imageId);  int imageIdTemp(0);  float dis(0);
	int imageW(0), imageH(0);
	for (int windowH = -halfWindow; windowH <= halfWindow; ++windowH)
	{
		for (int windowW = -halfWindow; windowW <= halfWindow; ++windowW)
		{
			imageW = winX + windowW; imageH = winY + windowH;
			if (imageW < 0 || imageW >= imageWidth || imageH < 0 || imageH >= imageHeight)
			{
				continue;
			}
			imageIdTemp = imageWidth * imageH + imageW;
			if (isSelected[imageIdTemp] || abs(points->getPoint(imageIdTemp)->z)<1e-6)
			{
				continue;
			}
			const CCVector3 *pointAround = points->getPoint(imageIdTemp);
			if (isDepth)
			{
				dis = abs(pointmiddle->z - pointAround->z);
			}
			else
			{
				dis = sqrt((pointmiddle->x - pointAround->x) * (pointmiddle->x - pointAround->x) + (pointmiddle->y - pointAround->y) * (pointmiddle->y - pointAround->y) +
					(pointmiddle->z - pointAround->z) * (pointmiddle->z - pointAround->z));
			}

			if (dis > disThread)
			{
				continue;
			}
			pointIdSet.push_back(imageIdTemp);
			isSelected[imageIdTemp] = true;
		}
	}
}


void DeleteIsland::doAction()
{
	size_t timeBegin = clock();
	QElapsedTimer eTimer;
	eTimer.start();

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
	// 插件功能：去孤岛滤波
	ccPointCloud* pc = static_cast<ccPointCloud*>(ent);

	//设置点云长宽
	pc->m_width = 4096;
	pc->m_height = 3072;
	pc->m_flags = new VCoordFlag[pc->m_width * pc->m_height];

	int imgWidth = pc->m_width;
	int imgHeight = pc->m_height;
	int pointsNum = pc->m_width * pc->m_height;
	std::vector<bool> isSelected; //判断当前 point 是否参与聚类
	isSelected.resize(pointsNum, 0);
	std::vector<int> currIsland;//当前聚类 point 索引
	std::vector<std::vector<int>> islandRecordAll;//存储所有聚类

	int numThresh = 1000;
	int findRange = 3;
	double disThresh = 0.05;
	bool isDepth = true;
	CCVector3 invalidValue(0,0,0);
	std::vector<bool> isZero(pointsNum);


	//显示插件ui窗体
	{
		//ccDIDlg safDlg(m_app->getMainWindow());
		ccDIDlg safDlg;
		safDlg.spinBox_1->setValue(numThresh);
		safDlg.spinBox_2->setValue(findRange);
		safDlg.doubleSpinBox_1->setValue(disThresh);
		safDlg.spinBox_3->setValue(isDepth);

		if (!safDlg.exec())
		{
			return;
		}

		//存储阈值
		numThresh = safDlg.spinBox_1->value();
		findRange = safDlg.spinBox_2->value();
		disThresh = safDlg.doubleSpinBox_1->value();
		isDepth = safDlg.spinBox_3->value();
	}

	for (int ptId = 0; ptId < pointsNum; ptId++)
	{
		currIsland.clear();
		if (abs(pc->getPoint(ptId)->z)<1e-6 || isSelected[ptId] == 1)
		{
			continue;
		}
		else
		{
			currIsland.push_back(ptId);
			isSelected[ptId] = 1;
		}

		for (int i = 0; i < currIsland.size(); i++)
		{
			get_point_around(pc, isSelected, currIsland, currIsland[i], imgWidth, imgHeight, findRange, disThresh, isDepth);
		}

		if (numThresh <= 0)
		{
			islandRecordAll.push_back(currIsland);
		}
		else
		{
			if (currIsland.size() < numThresh)
			{
//#pragma omp parallel for//此处不可并行
				for (int idDelete = 0; idDelete < currIsland.size(); ++idDelete)
				{
					isZero[currIsland[idDelete]] = true;
				}
			}
		}
	}

	//复制点云
	ccPointCloud* pc_new = new ccPointCloud("filtered-cloud");
	for (int i = 0; i < pc->size(); ++i)
	{
		if (isZero[i])
		{
			pc_new->addPoint(CCVector3(60, 0, 0));
		}
		else
		{
			pc_new->addPoint(CCVector3(pc->getPoint(i)->x + 60, pc->getPoint(i)->y, pc->getPoint(i)->z));
		}
	}
		


	//需要将点云转换为generic格式，才可调用数据结构
	//ccGenericPointCloud gpc(pc_new);

	//需要利用kd树（cckdtree）或者八叉树（ccoctree）进行邻域搜索，达到邻域聚类滤波的目的



	ccHObject* cc_containers = new ccHObject("filtered-cloud");
	cc_containers->addChild(pc_new);
	m_app->addToDB(cc_containers, true, true);
	m_app->refreshAll();
	/*********************************插入代码*****************************************/

	// This is how you can output messages
	// Display a standard message in the console
	m_app->dispToConsole("[DeleteIsland] plugin run successfully!", ccMainAppInterface::STD_CONSOLE_MESSAGE);
	ccLog::Print("[DeleteIsland] Timing: %3.3f s.", eTimer.elapsed() / 1000.0);
}