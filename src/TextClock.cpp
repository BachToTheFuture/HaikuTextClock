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

#include <Slider.h>
#include <Alignment.h>
#include <String.h>
#include <Catalog.h>
#include <Font.h>
#include <StringView.h>
#include <View.h>
#include <LayoutBuilder.h>
#include <Window.h>


static const int32 kMinRed = 'minr';
static const int32 kMaxRed = 'maxr';
static const int32 kMinBlue = 'minb';
static const int32 kMaxBlue = 'maxb';
static const int32 kMinGreen = 'ming';
static const int32 kMaxGreen = 'maxg';
static const int32 kTickSpeed = 'tick';


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

	fMinRed = 49;
	fMaxRed = 255;
	fMinBlue = 49;
	fMaxBlue = 255;
	fMinGreen = 49;
	fMaxGreen = 255;
	fTickSpeed = 10000;

	fMinRed = archive->GetInt32("min_red", fMinRed);
	fMaxRed = archive->GetInt32("max_red", fMaxRed);
	fMinBlue = archive->GetInt32("min_blue", fMinBlue);
	fMaxBlue = archive->GetInt32("max_blue", fMaxBlue);
	fMinGreen = archive->GetInt32("min_green", fMinGreen);
	fMaxGreen = archive->GetInt32("max_green", fMaxGreen);
	fTickSpeed = archive->GetInt32("tick_speed", fTickSpeed);

}


void TextClock::StartConfig(BView* view)
{
	// Thank you, PulkoMandy, for your Neonlights screensaver!
	// This function is a modified version of Neonlight's
	// StartConfig function.
	BSlider* s0;
	BSlider* s1;
	BSlider* s2;
	BSlider* s3;
	BSlider* s4;
	BSlider* s5;
	BSlider* s6;
	BSlider* s7;

	BWindow* win = view->Window();
	if (win)
		win->AddHandler(this);

	BStringView* v1 = new BStringView("name", "Haiku TextClock");
	v1->SetFont(be_bold_font);
	BStringView* v2 = new BStringView("author", "by Bach Nguyen");

	BStringView* v3 = new BStringView("tick_speed", "Tick speed");
	v3->SetFont(be_bold_font);

	s0 = new BSlider("tick_speed", "", new BMessage(kTickSpeed), 1000, 60000, B_HORIZONTAL);
	s0->SetValue(fTickSpeed);
	s0->SetTarget(this);
	s0->SetLimitLabels("Fast", "Slow");


	s2 = new BSlider("max_red", "Red", new BMessage(kMaxRed), 50, 254, B_HORIZONTAL);
	s2->SetValue(fMaxRed);
	s2->SetTarget(this);
	s2->SetLimitLabels("0", "255");

	s4 = new BSlider("max_green", "Green", new BMessage(kMaxGreen), 50, 254, B_HORIZONTAL);
	s4->SetValue(fMaxGreen);
	s4->SetTarget(this);
	s4->SetLimitLabels("0", "255");

	s6 = new BSlider("max_blue", "Blue", new BMessage(kMaxBlue), 50, 254, B_HORIZONTAL);
	s6->SetValue(fMaxBlue);
	s6->SetTarget(this);
	s6->SetLimitLabels("0", "255");


	BLayoutBuilder::Group<>(view, B_VERTICAL, B_USE_ITEM_SPACING)
		.SetInsets(B_USE_WINDOW_INSETS)
		.SetExplicitAlignment(BAlignment(B_ALIGN_HORIZONTAL_CENTER, B_ALIGN_TOP))
		.AddGroup(B_HORIZONTAL)
			.Add(v1)
			.Add(v2)
			.AddGlue()
		.End()
		.Add(v3)
		.Add(s0)
		.AddGlue()
		.Add(s2)
		.AddGlue()
		.Add(s4)
		.AddGlue()
		.Add(s6)
		.AddGlue()
		.End();
}


void
TextClock::NewColor()
{
	fTextColor1 = {fMinRed+rand()%fMaxRed, fMinGreen+rand()%fMaxGreen, fMinBlue+rand()%fMaxBlue};
	fTextColor2 = {fMinRed+rand()%fMaxRed, fMinGreen+rand()%fMaxGreen, fMinBlue+rand()%fMaxBlue};
	fTextColor3 = {fMinRed+rand()%fMaxRed, fMinGreen+rand()%fMaxGreen, fMinBlue+rand()%fMaxBlue};
	// The time duration (in frames)
	fColor1Duration = 100+rand()%400;
	fColor2Duration = 100+rand()%400;
	fColor3Duration = 100+rand()%400;
}


void
TextClock::UpdateColor()
{
	// Randomly change colors of the text.
	rgb_color* colors[3] = {
		&fTextColor1, &fTextColor2, &fTextColor3
	};
	int durations[3] = {fColor1Duration, fColor2Duration, fColor3Duration};
	int* randoms[3] = {&fRandom1, &fRandom2, &fRandom3};

	int test;
	int n;
	for (n = 0; n < 3; n++) {
		if (fFrame % durations[n] == 0)
		{
			test = rand()%6;
			*randoms[n] = test;
		}
		else test = *randoms[n];

		if (colors[n]->red <= fMinRed && fMinRed != fMaxRed
			|| colors[n]->green <= fMinGreen && fMinGreen != fMaxGreen
			|| colors[n]->blue <= fMinBlue && fMinBlue!= fMaxBlue)
		{
			test = rand()%3;
			*randoms[n] = test;
		}
		else if (colors[n]->red >= fMaxRed && fMinRed != fMaxRed
			|| colors[n]->green >= fMaxGreen && fMinGreen != fMaxGreen
			|| colors[n]->blue >= fMaxBlue && fMinBlue!= fMaxBlue)
		{
			test = 3+rand()%3;
			*randoms[n] = test;
		}
		switch (test)
		{
			case 0: colors[n]->red++; break;
			case 1: colors[n]->green++; break;
			case 2: colors[n]->blue++; break;
			case 3: colors[n]->red--; break;
			case 4: colors[n]->green--; break;
			case 5: colors[n]->blue--; break;
		}
	}
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
	else if (min >= 15 && min < 20)
	{
		PAST_ON = true;
		QUARTER_ON = true;
	}
	else if (min >= 20 && min < 25)
	{
		PAST_ON = true;
		TWENTY_ON = true;
		MINUTES_ON = true;
	}
	else if (min >= 25 && min < 30)
	{
		PAST_ON = true;
		TWENTY_ON = true;
		FIVE1_ON = true;
		MINUTES_ON = true;
	}
	else if (min >= 30 && min < 35)
	{
		HALF_ON = true;
		PAST_ON = true;
	}
	else if (min >= 35 && min < 40)
	{
		TWENTY_ON = true;
		FIVE1_ON = true;
		TO_ON = true;
		MINUTES_ON = true;
		hr++;
	}
	else if (min >= 40 && min < 45)
	{
		TWENTY_ON = true;
		TO_ON = true;
		MINUTES_ON = true;
		hr++;
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
	temp = "IT   IS    HALF   TEN";
	// Center the text
	fDelta.nonspace = 0;
	fDelta.space = 0;
	font.GetBoundingBoxesForStrings(&temp, 1, B_SCREEN_METRIC, &fDelta,
		&stringRect);
	float y = ((viewHeight - (stringRect.Height() * 2 + viewHeight / 10)) / 2)
		+ stringRect.Height();
	fLeftCorner.Set(((viewWidth - stringRect.Width()) / 2) - stringRect.left, y/2);
	SetTickSize(fTickSpeed); // Changes every second
	return B_OK;
}


void
TextClock::Draw(BView* view, int32 frame)
{
	fFrame = frame;
	CheckTime();
	rgb_color kBlack = {15,15,15};
	rgb_color kGray = {25,25,25};
	rgb_color text;

	if (frame == 0) {
		NewColor();
		UpdateColor();
		view->SetHighColor(kBlack);
		view->SetLowColor(kBlack);
		view->FillRect(view->Bounds());
	}
	view->SetHighColor(fTextColor1);
	view->DrawString(IT, fLeftCorner, 0);
	BPoint copy = fLeftCorner;
	copy.Set(fLeftCorner.x + fSpace, fLeftCorner.y);
	view->DrawString(IS, copy, 0);
	UpdateColor();
	// Here is where things get messy...
	// I think I could use some loops here... but the spacing between words
	// are rather inconsistent...

	// The "fSpace * NUMBER" parts were a result of a painful trial and error.
	HALF_ON ? text = fTextColor2 : text = kGray;
	view->SetHighColor(text);
	copy += BPoint(fSpace, 0);
	view->DrawString(HALF, copy, 0);

	TEN1_ON ? text = fTextColor2 : text = kGray;
	view->SetHighColor(text);
	copy += BPoint(fSpace * 3.1, 0);
	view->DrawString(TEN1, copy, 0);

	copy.Set(fLeftCorner.x, copy.y + fSpace);
	QUARTER_ON ? text = fTextColor2 : text = kGray;
	view->SetHighColor(text);
	view->DrawString(QUARTER, copy, 0);

	TWENTY_ON ? text = fTextColor2 : text = kGray;
	view->SetHighColor(text);
	copy += BPoint(fSpace * 3.6, 0);
	view->DrawString(TWENTY, copy, 0);

	copy.Set(fLeftCorner.x, copy.y + fSpace);
	FIVE1_ON ? text = fTextColor2 : text = kGray;
	view->SetHighColor(text);
	view->DrawString(FIVE1, copy, 0);
	copy += BPoint(fSpace * 2, 0);

	MINUTES_ON ? text = fTextColor2 : text = kGray;
	view->SetHighColor(text);
	view->DrawString(MINUTES, copy, 0);
	copy += BPoint(fSpace * 3.5, 0);

	TO_ON ? text = fTextColor3 : text = kGray;
	view->SetHighColor(text);
	view->DrawString(TO, copy, 0);

	copy.Set(fLeftCorner.x, copy.y + fSpace);
	PAST_ON ? text = fTextColor3 : text = kGray;
	view->SetHighColor(text);
	view->DrawString(PAST, copy, 0);
	copy += BPoint(fSpace * 2, 0);

	TWO_ON ? text = fTextColor1 : text = kGray;
	view->SetHighColor(text);
	view->DrawString(TWO, copy, 0);
	copy += BPoint(fSpace * 2.2, 0);

	THREE_ON ? text = fTextColor1 : text = kGray;
	view->SetHighColor(text);
	view->DrawString(THREE, copy, 0);

	copy.Set(fLeftCorner.x, copy.y + fSpace);
	ONE_ON ? text = fTextColor2 : text = kGray;
	view->SetHighColor(text);
	view->DrawString(ONE, copy, 0);
	copy += BPoint(fSpace * 2.75, 0);

	FOUR_ON ? text = fTextColor2 : text = kGray;
	view->SetHighColor(text);
	view->DrawString(FOUR, copy, 0);
	copy += BPoint(fSpace * 2.2, 0);

	FIVE2_ON ? text = fTextColor2 : text = kGray;
	view->SetHighColor(text);
	view->DrawString(FIVE2, copy, 0);

	copy.Set(fLeftCorner.x, copy.y + fSpace);
	SIX_ON ? text = fTextColor3 : text = kGray;
	view->SetHighColor(text);
	view->DrawString(SIX, copy, 0);
	copy += BPoint(fSpace * 1.5, 0);

	SEVEN_ON ? text = fTextColor3 : text = kGray;
	view->SetHighColor(text);
	view->DrawString(SEVEN, copy, 0);
	copy += BPoint(fSpace * 2.75, 0);

	EIGHT_ON ? text = fTextColor3 : text = kGray;
	view->SetHighColor(text);
	view->DrawString(EIGHT, copy, 0);

	copy.Set(fLeftCorner.x, copy.y + fSpace);
	NINE_ON ? text = fTextColor1 : text = kGray;
	view->SetHighColor(text);
	view->DrawString(NINE, copy, 0);
	copy += BPoint(fSpace * 2, 0);

	TEN2_ON ? text = fTextColor1 : text = kGray;
	view->SetHighColor(text);
	view->DrawString(TEN2, copy, 0);
	copy += BPoint(fSpace * 1.8, 0);

	ELEVEN_ON ? text = fTextColor1 : text = kGray;
	view->SetHighColor(text);
	view->DrawString(ELEVEN, copy, 0);

	copy.Set(fLeftCorner.x, copy.y + fSpace);
	TWELVE_ON ? text = fTextColor2 : text = kGray;
	view->SetHighColor(text);
	view->DrawString(TWELVE, copy, 0);

	OCLOCK_ON ? text = fTextColor2 : text = kGray;
	view->SetHighColor(text);
	copy += BPoint(fSpace * 3.35, 0);
	view->DrawString(OCLOCK, copy, 0);
}


void
TextClock::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
	case kMinRed:
		fMinRed = msg->GetInt32("be:value", fMinRed);
		break;
	case kMaxRed:
		fMaxRed = msg->GetInt32("be:value", fMaxRed);
		break;
	case kMinGreen:
		fMinGreen = msg->GetInt32("be:value", fMinGreen);
		break;
	case kMaxGreen:
		fMaxGreen = msg->GetInt32("be:value", fMaxGreen);
		break;
	case kMinBlue:
		fMinBlue = msg->GetInt32("be:value", fMinBlue);
		break;
	case kMaxBlue:
		fMaxBlue = msg->GetInt32("be:value", fMaxBlue);
		break;
	case kTickSpeed:
		fTickSpeed = msg->GetInt32("be:value", fTickSpeed);
		break;
	default:
		BHandler::MessageReceived(msg);
	}
}


status_t
TextClock::SaveState(BMessage* into) const
{
	into->AddInt32("min_red", fMinRed);
	into->AddInt32("max_red", fMaxRed);
	into->AddInt32("min_blue", fMinBlue);
	into->AddInt32("max_blue", fMaxBlue);
	into->AddInt32("min_green", fMinGreen);
	into->AddInt32("max_green", fMaxGreen);
	into->AddInt32("tick_speed", fTickSpeed);
	return B_OK;
}
