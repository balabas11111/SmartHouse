package com.balabas.smarthouse.server.entity.model.descriptor;

import java.util.Map;

import com.google.common.collect.Maps;

public enum Emoji {
    
	ALEMBIC('\u2697',null,"2697"),
	ARROW_DOWN('\u2B07',null,"2B07"),
	BATTERY('\uD83D','\uDD0B',"1F50B"),
	BELL('\uD83D','\uDD14',"1F514"),
	BELL_CANCEL('\uD83D','\uDD15',"1F515"),
	BOOKMARK_TABS('\uD83D','\uDCD1',"1F4D1"),
	BAR_CHART('\uD83D','\uDCCA',"1F4CA"),
	BRIEFCASE('\uD83D','\uDCBC',"1F4BC"),
	BULB('\uD83D','\uDCA1',"1F4A1"),
	CANDLE('\uD83D','\uDD6F',"1F56F"),
	CARD_FILE_BOX('\uD83D','\uDDC3',"1F5C3"),
	CHECK_MARK('\u2705',null,"2705"),
	CROSS_MARK_RED('\u274C',null,"274C"),
	CYCLONE('\uD83C','\uDF00',"1F300"),
	DIM_BUTTON('\uD83D','\uDD05',"1F505"),
	DROPLET('\uD83D','\uDCA7',"1F4A7"),
	TRACKBALL('\uD83D','\uDDB2',"1F5B2"),
	EIGHT_POINTED_STAR('\u2734',null, "2734"),
	ELECTRIC_PLUG('\uD83D','\uDD0C',"1F50C"),
	ERROR('\u274C',null, "274C"),
	FACE_WITH_TONGUE_AND_CLOSED_ONE_EYE('\uD83D', '\uDE1C', "1F61C"),
	FLOPPY('\uD83D', '\uDCBE', "1F4BE"),
	GEAR('\u2699', null, "2699"),
	GHOST('\uD83D', '\uDC7B', "1F47B"),
	HAND_RIGHT('\uD83D','\uDC49', "1F449"),
	HAND_OK('\uD83D','\uDC4D', "1F44D"),
	HAMMER_AND_WRENCH('\uD83D', '\uDEE0', "1F6E0"),
	HOURGLASS('\u23F3',null,"23F3"),
	HOUSE_BUILDING('\uD83C','\uDFE2', "1F3E2"),
	JAPANESE_SYMBOL_FOR_BEGINNER('\uD83D','\uDD30',"1F530"),
	JOYSTICK('\uD83D','\uDD79',"1F579"),
	LEVEL_SLIDER('\uD83C','\uDF9A',"1F39A"),
	MEMO('\uD83D','\uDCDD',"1F4DD"),
	PAGER('\uD83D','\uDCDF',"1F4DF"),
	OUTBOX_TRAY('\uD83D','\uDCE4',"1F4E4"),
	PERSON_RAISING_ONE_HAND('\uD83D','\uDE4B',"1F64B"),
	PHONE_ON('\uD83D','\uDCF1',"1F4F1"),
	PHONE_OFF('\uD83D','\uDCF4',"1F4F4"),
	PHONE_WITH_ARROW('\uD83D','\uDCF2',"1F4F2"),
	RADIO_BUTTON('\uD83D','\uDD18',"1F518"),
	RED_CIRCLE('\u2B55',null,"2B55"),
	RECYCLING('\u267B',null,"267B"),
	REFRESH('\uD83D','\uDD03',"1F503"),
	STAR('\u2B50',null,"2B50"),
	STOP('\u26D4',null, "26D4"),
	SKULL('\uD83D', '\uDC80', "1F480"),
	SUN('\u2600',null, "2600"),
	SUN_CLOUD_RAIN('\uD83C','\uDF26',"1F326"),
	SUN_CLOUD('\u26C5',null,"26C5"),
	SUN_WITH_FACE('\uD83C','\uDF1E',"1F31E"),
	THERMOMETER('\uD83C','\uDF21',"1F321"),
	TRIANGLE('\uD83D','\uDCD0',"1F4D0"),
	UMBRELLA_WITH_DROPLETS('\u2614',null,"2614"),
	
	
	WARNING('\u26A0', null, "26A0"),
	WAVING_HAND_SIGN('\uD83D','\uDC4B', "1F44B"),
	EMPTY_EMOJI(null,null, " ");
	
	
    Character chr1;
    Character chr2;
    String code;

    private static final Map<String, Emoji> codeMap =
    		Maps.newHashMapWithExpectedSize(Emoji.values().length);
    static {
        for (Emoji emoji : Emoji.values()) {
        	codeMap.put(emoji.code, emoji);
        }
    }
    
    Emoji(Character chr1,Character chr2,String code) {
        this.chr1 = chr1;
        this.chr2 = chr2;
        this.code = code;
    }
    
    Emoji(Character chr1,Character chr2) {
        this.chr1 = chr1;
        this.chr2 = chr2;
    }
    
    Emoji(Character chr1) {
        this.chr1 = chr1;
    }
    
    public String getCode(){
    	return this.code;
    }
    
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder("");

        if (this.chr1 != null) {
            sb.append(this.chr1);
        }
        if (this.chr2 != null) {
            sb.append(this.chr2);
        }

        return sb.toString();
    }
    
    public static Emoji getByCode(String code) {
    	if(code == null) {
    		return null;
    	}
    	return codeMap.getOrDefault(code, null);
    }
    
}
