package com.balabas.smarthouse.telegram.bot.service;

public enum Emoji {
    
	REFRESH('\uD83D','\uDD03',"Refresh"),
	FACE_WITH_TONGUE_AND_CLOSED_ONE_EYE('\uD83D', '\uDE1C',"smile"),
	CHECK_MARK('\u2705', null,"check"),
	HOUSE_BUILDING('\uD83C','\uDFE2',"Home"),
	OUTBOX_TRAY('\uD83D','\uDCE4',"outbox"),
	WAVING_HAND_SIGN('\uD83D','\uDC4B'),
	HAPPY_PERSON_RAISING_ONE_HAND('\uD83D','\uDE4B'),
	WARNING('\u26A0',null),
	SETTINGS('\uD83D','\uDD27'),
	STAR('\u2B50',null),
	BRIEFCASE('\uD83D','\uDCBC'),
	RIGHT_HAND('\uD83D','\uDC49');
	
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
