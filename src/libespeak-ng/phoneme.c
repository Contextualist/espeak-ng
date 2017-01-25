/*
 * Copyright (C) 2016 Reece H. Dunn
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see: <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <errno.h>

#include <espeak-ng/espeak_ng.h>
#include <espeak-ng/speak_lib.h>

#include "phoneme.h"
#include "speech.h"
#include "error.h"

// See docs/phonemes.md for the list of supported features.
enum feature_t {
	// invalid phoneme feature name
	inv, // Not in docs/phonemes.md. This is used to signal an unknown feature name.
	// manner of articulation
	nas,
	stp,
	afr,
	frc,
	flp,
	trl,
	apr,
	clk,
	ejc,
	imp,
	vwl,
};

static MNEM_TAB features[] = {
	// manner of articulation
	{ "nas", nas },
	{ "stp", stp },
	{ "frc", frc },
	{ "afr", afr },
	{ "flp", flp },
	{ "trl", trl },
	{ "apr", apr },
	{ "clk", clk },
	{ "ejc", ejc },
	{ "imp", imp },
	{ "vwl", vwl },
	// invalid phoneme feature
	{ NULL,  inv },
};

espeak_ng_STATUS
phoneme_add_feature(PHONEME_TAB *phoneme,
                    const char *feature,
                    espeak_ng_ERROR_CONTEXT *context)
{
	if (!phoneme || !feature) return EINVAL;

	switch (LookupMnem(features, feature))
	{
	// manner of articulation
	case nas:
		phoneme->type = phNASAL;
		break;
	case stp:
	case afr: // FIXME: eSpeak treats 'afr' as 'stp'.
		phoneme->type = phSTOP;
		break;
	case frc:
	case apr: // FIXME: eSpeak is using this for [h], with 'liquid' used for [l] and [r].
		phoneme->type = phFRICATIVE;
		break;
	case flp: // FIXME: Why is eSpeak using a vstop (vcd + stp) for this?
		phoneme->type = phVSTOP;
		break;
	case trl: // FIXME: 'trill' should be the type; 'liquid' should be a flag (phoneme files specify both).
		phoneme->phflags |= phTRILL;
		break;
	case clk:
	case ejc:
	case imp:
		// Not supported by eSpeak.
		break;
	case vwl:
		phoneme->type = phVOWEL;
		break;
	// invalid phoneme feature
	default:
		return create_name_error_context(context, ENS_UNKNOWN_PHONEME_FEATURE, feature);
	}
	return ENS_OK;
}
