#include "TouchPad.h"

TouchPad touchPad;

bool TouchPad::isTouchDown(int eventCode) {
    return (eventCode & emtTouchMask) == emtTouchDown;
}
 
bool TouchPad::isTouchUp(int eventCode) {
    return (eventCode & emtTouchMask) == emtTouchUp;
}

void TouchPad::HandleMultiTouchButton(s3ePointerTouchEvent* event) {
	Touch* touch = touchPad.findTouch(event->m_TouchID);
    if (touch != NULL) {
        touch->isPressed = event->m_Pressed != 0; 
        touch->isActive  = true;
        touch->x  = event->m_x;
        touch->y  = event->m_y;
		touch->id = event->m_TouchID;
    }
}

void TouchPad::HandleMultiTouchMotion(s3ePointerTouchMotionEvent* event) {
	Touch* touch = touchPad.findTouch(event->m_TouchID);
    if (touch != NULL) {
		if (touch->isActive) {
			touch->isMoved = true;
		}
        touch->isActive  = true;
        touch->x = event->m_x;
        touch->y = event->m_y;
    }
}

void HandleSingleTouchButton(s3ePointerEvent* event) {
	Touch* touch = touchPad.getTouch(0);
    touch->isPressed = event->m_Pressed != 0;
    touch->isActive  = true;
    touch->x  = event->m_x;
    touch->y  = event->m_y;
	touch->id = 0;
}

void HandleSingleTouchMotion(s3ePointerMotionEvent* event) {
	Touch* touch = touchPad.getTouch(0);
	if (touch->isActive) {
		touch->isMoved = true;
	}
    touch->isActive  = true;
    touch->x = event->m_x;
    touch->y = event->m_y;
}

Touch* TouchPad::getTouchByID(int id) {
	for (int i = 0; i < MAX_TOUCHES; i++) {
		if (Touches[i].isActive && Touches[i].id == id)
			return &Touches[i];
	}
	return NULL;
}

Touch* TouchPad::getTouchPressed() {
	for (int i = 0; i < MAX_TOUCHES; i++) {
		if (Touches[i].isPressed && Touches[i].isActive)
			return &Touches[i];
	}
	return NULL;
}

Touch* TouchPad::findTouch(int id) {
	if (!IsAvailable)
		return NULL;
	for (int i = 0; i < MAX_TOUCHES; i++) {
		if (Touches[i].id == id)
			return &Touches[i];
    }
	for (int i = 0; i < MAX_TOUCHES; i++) {
		if (!Touches[i].isActive)	{
            Touches[i].id = id;
			return &Touches[i];
		}
	}
	return NULL;
}

int	TouchPad::getTouchCount() const {
	if (!IsAvailable)
		return 0;
	int r = 0;
	for (int i = 0; i < MAX_TOUCHES; i++) {
		if (Touches[i].isActive) {
            r++;
		}
	}
	return r;
}

void TouchPad::update() {
	for (int i = 0; i < MAX_TOUCHES; i++) {
		Touches[i].isMoved = false;
	}
	if (IsAvailable) {
		s3ePointerUpdate();
	}
}

void TouchPad::clear() {
	for (int i = 0; i < MAX_TOUCHES; i++) {
		if (!Touches[i].isPressed) {
			Touches[i].isActive = false;
		}
		Touches[i].isMoved = false;
	}
}

bool TouchPad::init() {
    IsAvailable = s3ePointerGetInt(S3E_POINTER_AVAILABLE) ? true : false;
	if (!IsAvailable) return false;
	for (int i = 0; i < MAX_TOUCHES; i++) {
		Touches[i].isPressed = false;
		Touches[i].isActive = false;
		Touches[i].isMoved = false;
		Touches[i].id = 0;
	}
    IsMultiTouch = s3ePointerGetInt(S3E_POINTER_MULTI_TOUCH_AVAILABLE) ? true : false;
    if (IsMultiTouch) {
        s3ePointerRegister(S3E_POINTER_TOUCH_EVENT, (s3eCallback)HandleMultiTouchButton, NULL);
        s3ePointerRegister(S3E_POINTER_TOUCH_MOTION_EVENT, (s3eCallback)HandleMultiTouchMotion, NULL);
    } else {
        s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleSingleTouchButton, NULL);
        s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleSingleTouchMotion, NULL);
    }
	return true;
}

void TouchPad::release() {
	if (IsAvailable) {
		if (IsMultiTouch) {
			s3ePointerUnRegister(S3E_POINTER_TOUCH_EVENT, (s3eCallback)HandleMultiTouchButton);
			s3ePointerUnRegister(S3E_POINTER_TOUCH_MOTION_EVENT, (s3eCallback)HandleMultiTouchMotion);
		} else {
			s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleSingleTouchButton);
			s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleSingleTouchMotion);
		}
	}
}