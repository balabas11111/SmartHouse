package com.balabas.smarthouse.telegram.bot.message;

public enum Emoji {
    
	REFRESH('\uD83D','\uDD03'),
	WARNING('\u26A0'),
	ERROR('\u274C'),
	FACE_WITH_TONGUE_AND_CLOSED_ONE_EYE('\uD83D', '\uDE1C'),
	CHECK_MARK('\u2705'),
	HOUSE_BUILDING('\uD83C','\uDFE2'),
	OUTBOX_TRAY('\uD83D','\uDCE4'),
	WAVING_HAND_SIGN('\uD83D','\uDC4B'),
	HAPPY_PERSON_RAISING_ONE_HAND('\uD83D','\uDE4B'),
	SETTINGS('\uD83D','\uDD27'),
	STAR('\u2B50'),
	BRIEFCASE('\uD83D','\uDCBC'),
	RIGHT_HAND('\uD83D','\uDC49'),
	SUN('\uD83C','\uDF1E'),
	STOP('\u26D4'),
	PHONE_ON('\uD83D','\uDCF1'), 
	PHONE_OFF('\uD83D','\uDCF4'),
	PAGER('\uD83D','\uDCDF'),
	GHOST('\uD83D', '\uDC7B'),
	SKULL('\uD83D', '\uDC80'),
	RED_CIRCLE('\u2B55'),
	HOURGLASS('\u23F3') ;
	
    Character chr1;
    Character chr2;
    String key;

    Emoji(Character chr1,Character chr2,String key) {
        this.chr1 = chr1;
        this.chr2 = chr2;
        this.key = key;
    }
    
    Emoji(Character chr1,Character chr2) {
        this.chr1 = chr1;
        this.chr2 = chr2;
    }
    
    Emoji(Character chr1) {
        this.chr1 = chr1;
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();

        if (this.chr1 != null) {
            sb.append(this.chr1);
        }
        if (this.chr2 != null) {
            sb.append(this.chr2);
        }

        return sb.toString();
    }
}
