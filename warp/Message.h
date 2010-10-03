/*
 *  Message.h
 *  warp
 *
 *  Created by Nicholas Kostelnik on 12/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MESSAGE_TYPES_
#define MESSAGE_TYPES_

enum MessageTypes
{
	MESSAGETYPE_MIN = 0,
	LEFT_DOWN = 1,
	RIGHT_DOWN = 2,
	LEFT_UP = 3,
	RIGHT_UP = 4,
	KEY_DOWN = 5,
	MOUSE_MOVE = 6,
	LEFT_DRAGGED = 7,
	RIGHT_DRAGGED = 8,
	FLAGS_CHANGED = 9,
	SCROLL_WHEEL = 10,
	LEFT_DOUBLE_CLICK = 11,
	KEY_UP = 12,
	MESSAGETYPE_MAX = 13
};

struct Message 
{
	int type;
	int x;
	int y;
	int key_code;
	unsigned int flags;
};

#endif