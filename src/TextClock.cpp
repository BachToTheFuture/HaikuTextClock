/*
 * Copyright 2018, Bach Nguyen <bach5000@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 */

#include "TextClock.h"

#include <time.h>
#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>

#include <Catalog.h>
#include <Font.h>
#include <StringView.h>
#include <View.h>
#include <LayoutBuilder.h>

BScreenSaver*
instantiate_screen_saver(BMessage* message, image_id image)
{
	srand(time(NULL));
	return new TextClock(message, image);
}


TextClock::TextClock(BMessage* archive, image_id id)
	:
	BScreenSaver(archive, id)
{
	IT = "IT";
	IS = "IS";
	HALF = "HALF";
	TEN1 = "TEN";
	QUARTER = "QUARTER";
	TWENTY = "TWENTY";
	FIVE1 = "FIVE";
	MINUTES = "MINUTES";
	TO = "TO";
	PAST = "PAST";
	TWO = "TWO";
	THREE = "THREE";
	ONE = "ONE";
	FOUR = "FOUR";
	FIVE2 = "FIVE";
	SIX = "SIX";
	SEVEN = "SEVEN";
	EIGHT = "EIGHT";
	NINE = "NINE";
	TEN2 = "TEN";
	ELEVEN = "ELEVEN";
	TWELVE = "TWELVE";
	OCLOCK = "O'CLOCK";

	HALF_ON = false;
	TEN1_ON = false;
	QUARTER_ON = false;
	TWENTY_ON = false;
	FIVE1_ON = false;
	MINUTES_ON = false;
	TO_ON = false;
	PAST_ON = false;
	TWO_ON = false;
	THREE_ON = false;
	ONE_ON = false;
	FOUR_ON = false;
	FIVE2_ON = false;
	SIX_ON = false;
	SEVEN_ON = false;
	EIGHT_ON =false;
	NINE_ON = false;
	TEN2_ON = false;
	ELEVEN_ON = false;
	TWELVE_ON = false;
	OCLOCK_ON = false;
}


void TextClock::StartConfig(BView* view)
{
	// Uh, not sure how I make this not centered...
	BStringView* v1 = new BStringView("name_and_author", "TextClock by Bach Nguyen");
	v1->SetFont(be_bold_font);

	BLayoutBuilder::Group<>(view, B_VERTICAL)
		.SetInsets(B_USE_WINDOW_INSETS)
		.Add(v1)
		.End();
}


void
TextClock::Reset()
{
	HALF_ON = false;
	TEN1_ON = false;
	QUARTER_ON = false;
	TWENTY_ON = false;
	FIVE1_ON = false;
	MINUTES_ON = false;
	TO_ON = false;
	PAST_ON = false;
	TWO_ON = false;
	THREE_ON = false;
	ONE_ON = false;
	FOUR_ON = false;
	FIVE2_ON = false;
	SIX_ON = false;
	SEVEN_ON = false;
	EIGHT_ON =false;
	NINE_ON = false;
	TEN2_ON = false;
	ELEVEN_ON = false;
	TWELVE_ON = false;
	OCLOCK_ON = false;
}

void
TextClock::CheckTime()
{
	Reset();
	bool* hours[22] = {
		&OCLOCK_ON, &ONE_ON, &TWO_ON,
		&THREE_ON, &FOUR_ON, &FIVE2_ON,
		&SIX_ON, &SEVEN_ON, &EIGHT_ON,
		&NINE_ON, &TEN2_ON, &ELEVEN_ON,
		&TWELVE_ON
	};

	fNow = BDateTime::CurrentDateTime(B_LOCAL_TIME);
	int hr = fNow.Time().Hour();

	int min = fNow.Time().Minute();
	if (min < 5 || min == 60) OCLOCK_ON = true;
	if (min >= 5 && min < 10)
	{
		PAST_ON = true;
		MINUTES_ON = true;
		FIVE1_ON = true;
	}
	else if (min >= 10 && min < 15)
	{
		PAST_ON = true;
		MINUTES_ON = true;
		TEN1_ON = true;
	}
	else if (min >= 15 && min < 30)
	{
		PAST_ON = true;
		QUARTER_ON = true;
	}
	else if (min >= 30 && min < 45)
	{
		HALF_ON = true;
		PAST_ON = true;
	}
	else if (min >= 45 && min < 50)
	{
		TO_ON = true;
		QUARTER_ON = true;
		hr++;
	}
	else if (min >= 50 && min < 55)
	{
		TO_ON = true;
		TEN1_ON = true;
		MINUTES_ON = true;
		hr++;
	}
	else if (min >= 55 && min < 60)
	{
		TO_ON = true;
		FIVE1_ON = true;
		MINUTES_ON = true;
		hr++;
	}

	if (fNow.Time().Hour() >= 12)
		*hours[hr-12] = true;
	else
		*hours[hr] = true;
}

status_t
TextClock::StartSaver(BView* view, bool preview)
{
	float viewWidth = view->Bounds().Width();
	float viewHeight = view->Bounds().Height();
	BFont font;

	view->GetFont(&font);
	font.SetSize(viewHeight / 20);
	view->SetFont(&font);

	const char* temp = "O";
	BRect stringRect;
	font.GetBoundingBoxesForStrings(&temp, 1, B_SCREEN_METRIC, &fDelta,
		&stringRect);
	fSpace = stringRect.Width() + stringRect.Height();
	// Get font shape + size

	temp = "AM   PM    HALF   TEN";
	// Center the text
	fDelta.nonspace = 0;
	fDelta.space = 0;

	font.GetBoundingBoxesForStrings(&temp, 1, B_SCREEN_METRIC, &fDelta,
		&stringRect);
	float y = ((viewHeight - (stringRect.Height() * 2 + viewHeight / 10)) / 2)
		+ stringRect.Height();
	fLeftCorner.Set(((viewWidth - stringRect.Width()) / 2) - stringRect.left, y/2);
	SetTickSize(60000000); // Changes every minute
	return B_OK;
}


void
TextClock::Draw(BView* view, int32 frame)
{
	CheckTime();
	rgb_color kBlack = {0,0,0};
	rgb_color kGray = {38,38,38};
	rgb_color text;

	if (frame == 0) {
		view->SetHighColor(kBlack);
		view->SetLowColor(kBlack);
		view->FillRect(view->Bounds());
		// Set text color
		text = {120+rand()%135, 120+rand()%135, 120+rand()%135};
		view->SetHighColor(text);
		view->DrawString(IT, fLeftCorner, 0);
	}
	text = {120+rand()%135, 120+rand()%135, 120+rand()%135};
	view->SetHighColor(text);
	BPoint copy = fLeftCorner;
	copy.Set(fLeftCorner.x + fSpace, fLeftCorner.y);
	view->DrawString(IS, copy, 0);
	// Here is where things get messy...
	// I think I could use some loops here... but the spacing between words
	// are rather inconsistent...

	// The "fSpace * NUMBER" parts were a result of a painful trial and error.
	HALF_ON ? text = {120+rand()%135, 120+rand()%135, 120+rand()%135} : text = kGray;
	view->SetHighColor(text);
	copy += BPoint(fSpace, 0);
	view->DrawString(HALF, copy, 0);

	TEN1_ON ? text = {120+rand()%135, 120+rand()%135, 120+rand()%135} : text = kGray;
	view->SetHighColor(text);
	copy += BPoint(fSpace * 3, 0);
	view->DrawString(TEN1, copy, 0);

	copy.Set(fLeftCorner.x, copy.y + fSpace);
	QUARTER_ON ? text = {120+rand()%135, 120+rand()%135, 120+rand()%135} : text = kGray;
	view->SetHighColor(text);
	view->DrawString(QUARTER, copy, 0);

	TWENTY_ON ? text = {120+rand()%135, 120+rand()%135, 120+rand()%135} : text = kGray;
	view->SetHighColor(text);
	copy += BPoint(fSpace * 3.5, 0);
	view->DrawString(TWENTY, copy, 0);

	copy.Set(fLeftCorner.x, copy.y + fSpace);
	FIVE1_ON ? text = {120+rand()%135, 120+rand()%135, 120+rand()%135} : text = kGray;
	view->SetHighColor(text);
	view->DrawString(FIVE1, copy, 0);
	copy += BPoint(fSpace * 2, 0);

	MINUTES_ON ? text = {120+rand()%135, 120+rand()%135, 120+rand()%135} : text = kGray;
	view->SetHighColor(text);
	view->DrawString(MINUTES, copy, 0);
	copy += BPoint(fSpace * 3.5, 0);

	TO_ON ? text = {120+rand()%135, 120+rand()%135, 120+rand()%135} : text = kGray;
	view->SetHighColor(text);
	view->DrawString(TO, copy, 0);

	copy.Set(fLeftCorner.x, copy.y + fSpace);
	PAST_ON ? text = {120+rand()%135, 120+rand()%135, 120+rand()%135} : text = kGray;
	view->SetHighColor(text);
	view->DrawString(PAST, copy, 0);
	copy += BPoint(fSpace * 2, 0);

	TWO_ON ? text = {120+rand()%135, 120+rand()%135, 120+rand()%135} : text = kGray;
	view->SetHighColor(text);
	view->DrawString(TWO, copy, 0);
	copy += BPoint(fSpace * 2.2, 0);

	THREE_ON ? text = {120+rand()%135, 120+rand()%135, 120+rand()%135} : text = kGray;
	view->SetHighColor(text);
	view->DrawString(THREE, copy, 0);

	copy.Set(fLeftCorner.x, copy.y + fSpace);
	ONE_ON ? text = {120+rand()%135, 120+rand()%135, 120+rand()%135} : text = kGray;
	view->SetHighColor(text);
	view->DrawString(ONE, copy, 0);
	copy += BPoint(fSpace * 2.75, 0);

	FOUR_ON ? text = {120+rand()%135, 120+rand()%135, 120+rand()%135} : text = kGray;
	view->SetHighColor(text);
	view->DrawString(FOUR, copy, 0);
	copy += BPoint(fSpace * 2.2, 0);

	FIVE2_ON ? text = {120+rand()%135, 120+rand()%135, 120+rand()%135} : text = kGray;
	view->SetHighColor(text);
	view->DrawString(FIVE2, copy, 0);

	copy.Set(fLeftCorner.x, copy.y + fSpace);
	SIX_ON ? text = {120+rand()%135, 120+rand()%135, 120+rand()%135} : text = kGray;
	view->SetHighColor(text);
	view->DrawString(SIX, copy, 0);
	copy += BPoint(fSpace * 1.5, 0);

	SEVEN_ON ? text = {120+rand()%135, 120+rand()%135, 120+rand()%135} : text = kGray;
	view->SetHighColor(text);
	view->DrawString(SEVEN, copy, 0);
	copy += BPoint(fSpace * 2.75, 0);

	EIGHT_ON ? text = {120+rand()%135, 120+rand()%135, 120+rand()%135} : text = kGray;
	view->SetHighColor(text);
	view->DrawString(EIGHT, copy, 0);

	copy.Set(fLeftCorner.x, copy.y + fSpace);
	NINE_ON ? text = {120+rand()%135, 120+rand()%135, 120+rand()%135} : text = kGray;
	view->SetHighColor(text);
	view->DrawString(NINE, copy, 0);
	copy += BPoint(fSpace * 2, 0);

	TEN2_ON ? text = {120+rand()%135, 120+rand()%135, 120+rand()%135} : text = kGray;
	view->SetHighColor(text);
	view->DrawString(TEN2, copy, 0);
	copy += BPoint(fSpace * 1.8, 0);

	ELEVEN_ON ? text = {120+rand()%135, 120+rand()%135, 120+rand()%135} : text = kGray;
	view->SetHighColor(text);
	view->DrawString(ELEVEN, copy, 0);

	copy.Set(fLeftCorner.x, copy.y + fSpace);
	TWELVE_ON ? text = {120+rand()%135, 120+rand()%135, 120+rand()%135} : text = kGray;
	view->SetHighColor(text);
	view->DrawString(TWELVE, copy, 0);

	OCLOCK_ON ? text = {120+rand()%135, 120+rand()%135, 120+rand()%135} : text = kGray;
	view->SetHighColor(text);
	copy += BPoint(fSpace * 3.35, 0);
	view->DrawString(OCLOCK, copy, 0);
}
