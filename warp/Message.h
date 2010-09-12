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
	LEFT_DOWN = 0,
	RIGHT_DOWN = 1,
	LEFT_UP = 2,
	RIGHT_UP = 3,
	KEY_DOWN = 4,
	MOUSE_MOVED = 5,
	LEFT_DRAGGED = 6,
	RIGHT_DRAGGED = 7,
	FLAGS_CHANGED = 8,
	SCROLL_WHEEL = 9,
	LEFT_DOUBLE_CLICK = 10
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