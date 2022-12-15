/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include "viewer.h"

namespace vcl {

inline Viewer::~Viewer()
{
	// delete all the DrawableObjects
	for (DrawableObject* obj : drawList) {
		delete obj;
	}
}

/**
 * @brief Pushes the DrawableObject to the viewer. This function creates a **copy** of the given
 * argument and inserts it into the viewer.
 * @param obj
 * @return
 */
inline uint Viewer::pushDrawableObject(const DrawableObject& obj)
{
	drawList.push_back(obj.clone());
	return drawList.size();
}

inline uint Viewer::pushDrawableObject(const DrawableObject* obj)
{
	if (obj == nullptr) return -1;
	drawList.push_back(obj->clone());
	return drawList.size();
}

DrawableObject& Viewer::object(uint i)
{
	assert(i < drawList.size());
	return *drawList[i];
}

const DrawableObject& Viewer::object(uint i) const
{
	assert(i < drawList.size());
	return *drawList[i];
}

inline void Viewer::fitScene()
{
	Box3d bb = fullBB();
	Point3d sceneCenter = bb.center();
	double sceneRadius = bb.diagonal();

	setSceneCenter(qglviewer::Vec(sceneCenter.x(), sceneCenter.y(), sceneCenter.z()));
	setSceneRadius(sceneRadius);
	showEntireScene();
}

inline void Viewer::draw()
{
	for (const DrawableObject* obj : drawList)
		obj->draw();
}

inline void Viewer::init()
{
	// Restore previous viewer state.
	restoreStateFromFile();

	// Opens help window
	help();
}

inline QString Viewer::helpString() const
{
	QString text("<h2>S i m p l e V i e w e r</h2>");
	text += "Use the mouse to move the camera around the object. ";
	text +=
		"You can respectively revolve around, zoom and translate with the "
		"three mouse buttons. ";
	text +=
		"Left and middle buttons pressed together rotate around the camera "
		"view direction axis<br><br>";
	text +=
		"Pressing <b>Alt</b> and one of the function keys "
		"(<b>F1</b>..<b>F12</b>) defines a camera keyFrame. ";
	text +=
		"Simply press the function key again to restore it. Several "
		"keyFrames define a ";
	text +=
		"camera path. Paths are saved when you quit the application and "
		"restored at next start.<br><br>";
	text += "Press <b>F</b> to display the frame rate, <b>A</b> for the world axis, ";
	text +=
		"<b>Alt+Return</b> for full screen mode and <b>Control+S</b> to save "
		"a snapshot. ";
	text +=
		"See the <b>Keyboard</b> tab in this window for a complete shortcut "
		"list.<br><br>";
	text +=
		"Double clicks automates single click actions: A left button double "
		"click aligns the closer axis with the camera (if close enough). ";
	text +=
		"A middle button double click fits the zoom of the camera and the "
		"right button re-centers the scene.<br><br>";
	text +=
		"A left button double click while holding right button pressed "
		"defines the camera <i>Revolve Around Point</i>. ";
	text +=
		"See the <b>Mouse</b> tab and the documentation web pages for "
		"details.<br><br>";
	text += "Press <b>Escape</b> to exit the viewer.";
	return text;
}

Box3d Viewer::fullBB() const
{
	Box3d bb(Point3d(-1,-1,-1), Point3d(1,1,1));
	if (drawList.size() > 0) {
		uint i = 0;

		while (i < drawList.size() && !drawList[i]->isVisible())
			i++;

		if (i < drawList.size()) { //i will point to the first elagible object
			Point3d sc = drawList[i]->sceneCenter();
			bb.min = sc - drawList[i]->sceneRadius();
			bb.max = sc + drawList[i]->sceneRadius();

			for (i = i+1; i < drawList.size(); i++) { //rest of the list
				Point3d sc = drawList[i]->sceneCenter();
				Point3d tmp = sc - drawList[i]->sceneRadius();
				bb.min = vcl::min(bb.min, tmp);
				tmp = sc + drawList[i]->sceneRadius();
				bb.max = vcl::max(bb.max, tmp);
			}
		}
	}
	return bb;
}

} // namespace vcl
