#ifndef _TOUCHPAD_H_
#define _TOUCHPAD_H_

#include "s3ePointer.h"
#include "Desktop.h"

#define MAX_TOUCHES	3

struct Touch {
		int		x, y;
		bool	isActive, isPressed, isMoved;
		int		id;	
};

class TouchPad {
	private:
		bool		IsAvailable;
		bool		IsMultiTouch;
		Touch		Touches[MAX_TOUCHES];
		Touch*		findTouch(int id);								
		static      void HandleMultiTouchButton(s3ePointerTouchEvent* event);
		static      void HandleMultiTouchMotion(s3ePointerTouchMotionEvent* event);
	public:
        static bool isTouchDown(int eventCode);
        static bool isTouchUp(int eventCode);
		bool		isAvailable() const { return IsAvailable; }
		bool		isMultiTouch() const { return IsMultiTouch; }
		Touch*		getTouchByID(int id);
		Touch*		getTouch(int index) { return &Touches[index]; }	
		Touch*		getTouchPressed();
		int			getTouchCount() const;

		bool		init();
		void		release();
		void		update();
		void		clear();
};

extern TouchPad touchPad;

#endif	// _TOUCHPAD_H_
