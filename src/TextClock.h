/*
 * Copyright 2018, Bach Nguyen <bach5000@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */

#ifndef TCSS_H
#define TCSS_H


#include <Point.h>
#include <ScreenSaver.h>
#include <TimeFormat.h>
#include <DateFormat.h>
#include <DurationFormat.h>


class BMessage;
class BView;

class TextClock : public BScreenSaver {
public:
								TextClock(BMessage* archive, image_id);

			void				Draw(BView* view, int32 frame);
			void				StartConfig(BView *view);
			status_t			StartSaver(BView *view, bool preview);
			void				CheckTime();
			void				Reset();
			int32 				fSpace;

private:
			const char* 		IT;
			const char*			IS;
			const char*			HALF;
			const char*			TEN1;
			const char*			QUARTER;
			const char*			TWENTY;
			const char*			FIVE1;
			const char*			MINUTES;
			const char*			TO;
			const char*			PAST;
			const char*			TWO;
			const char*			THREE;
			const char*			ONE;
			const char*			FOUR;
			const char*			FIVE2;
			const char*			SIX;
			const char*			SEVEN;
			const char*			EIGHT;
			const char*			NINE;
			const char*			TEN2;
			const char*			ELEVEN;
			const char*			TWELVE;
			const char*			OCLOCK;

			bool			HALF_ON;
			bool			TEN1_ON;
			bool			QUARTER_ON;
			bool			TWENTY_ON;
			bool			FIVE1_ON;
			bool			MINUTES_ON;
			bool			TO_ON;
			bool			PAST_ON;
			bool			TWO_ON;
			bool			THREE_ON;
			bool			ONE_ON;
			bool			FOUR_ON;
			bool			FIVE2_ON;
			bool			SIX_ON;
			bool			SEVEN_ON;
			bool			EIGHT_ON;
			bool			NINE_ON;
			bool			TEN2_ON;
			bool			ELEVEN_ON;
			bool			TWELVE_ON;
			bool			OCLOCK_ON;

			BDateTime 			fNow;

			BPoint				fLeftCorner;
			escapement_delta	fDelta;

			// What the screensaver looks like:
			/*
			AM PM  HALF TEN
			QUARTER  TWENTY
			FIVE MINUTES TO
			PAST  TWO THREE
			ONE  FOUR  FIVE
			SIX SEVEN EIGHT
			NINE TEN ELEVEN
			TWELVE  O'CLOCK
			*/

};


#endif	// TCSS_H
