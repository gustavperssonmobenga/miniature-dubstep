/**
 * @file
 * @author Victor Lundberg <lundberg.victor@gmail.com>
 * @version 1.0
 * Copyright (�) A-Team.
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 */

#ifndef GUI_SUB_FRAME_TEST_H
#define GUI_SUB_FRAME_TEST_H
#include "GUIAnimatedImage.h"
#include "GUIText.h"

namespace gfx
{
	class GUISubFrameTest : public GUIImage
	{
		public:
			/**
			 * Default constructor.
			 * @param pBaseInstance Pointer to the instance base of a sprite.
			 * @param clientWidth Width of the client area.
			 * @param clientHeight Height of the client area.
			 * @param parentDepth Depth level of the parent.
			 */
			GUISubFrameTest(InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth);

			/**
			 * Subframe constructor.
			 * @param pParent Pointer to the parent.
			 */
			GUISubFrameTest(GUIFrame *pParent);

			/**
			 * Default copy constructor.
			 */
			GUISubFrameTest(const GUISubFrameTest &frame);

			/**
			 * Default destructor.
			 */
			virtual ~GUISubFrameTest();

			/**
			 * Default assignment operator.
			 */
			const GUISubFrameTest &operator=(const GUISubFrameTest &frame);

			/**
			 * Updates the GUI frame and all sub frames.
			 * @param deltaTime Time difference since the last frame.
			 */
			virtual void update(float deltaTime);

			/**
			 * Method that is called when the mouse pointer enters
			 * the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseEnter(long mX, long mY);

			/**
			 * Method that is called when the mouse pointer exits
			 * the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseExit(long mX, long mY);

			/**
			 * Method that is called when the mouse pointer moves within the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseMove(long mX, long mY);

			/**
			 * Method that is called when the left mouse button
			 * is pressed down within the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseLeftDown(long mX, long mY);

			/**
			 * Method that is called when the middle mouse button
			 * is pressed down within the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseMiddleDown(long mX, long mY);

			/**
			 * Method that is called when the right mouse button
			 * is pressed down within the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseRightDown(long mX, long mY);

			/**
			 * Method that is called when the left mouse button
			 * is released within the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseLeftUp(long mX, long mY);

			/**
			 * Method that is called when the middle mouse button
			 * is released within the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseMiddleUp(long mX, long mY);

			/**
			 * Method that is called when the right mouse button
			 * is released within the frame.
			 * @param mX X coordinate of the mouse pointer.
			 * @param mY Y coordinate of the mouse pointer.
			 */
			virtual void onMouseRightUp(long mX, long mY);

			/**
			 * Method that is called when a keyboard key is pressed.
			 * @param letterKeys Integer containing letter key status information.
			 * @param keys Integer containing key status information.
			 */
			virtual void onKeyDown(int letterKeys, int keys);

		protected:
			/**
			 * Initializes the frame.
			 */
			virtual void init();

		private:
	};
}

#endif