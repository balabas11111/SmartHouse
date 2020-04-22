package com.balabas.smarthouse.server.util;

import java.math.BigDecimal;
import java.math.RoundingMode;
import java.util.Collection;
import java.util.List;

import org.json.JSONArray;
import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Value;
import org.thymeleaf.util.StringUtils;

import com.balabas.smarthouse.server.entity.model.entityfields.IEntityFieldValue;
import com.balabas.smarthouse.server.entity.model.entityfields.IFloatProvider;

import lombok.experimental.UtilityClass;

@UtilityClass

public class MathUtil {

	public static final Float DEF_VAL_FLOAT = -2F;
	private static final String MAX = "max";
	private static final String MIN = "min";
	private static final String VAL = "val";
	private static Integer precision = 2;

	@Value("${smarthouse.server.entityfield.float.precision:#{null}}")
	public void setPresision(Integer pr) {
		precision = pr;
	}

	public static Float precise(Float value) {
		if (value == null) {
			return value;
		}
		return (new BigDecimal(value).setScale(precision, RoundingMode.HALF_UP)).floatValue();
	}
	
	public static boolean isValidFloat(String value) {
		try {
			getAsFloat(value);
		} catch(Exception e) {
			return false;
		}
		return true;
	}
	

	public static Float getAsFloat(String parameter) {
		return Float.valueOf(parameter);
	}
	
	public static<T extends IFloatProvider> Float getAverageFloat(Collection<T> sourceFloats) {
		Float total = 0F;
		int count = 0;
		
		for(IFloatProvider f : sourceFloats) {
			try {
				Float val = f.getValueAsFloat();
				
				if(val!=null) {
					total = total + val.floatValue();
					count++;
				}
				
			}catch(Exception e) {
				
			}
		}
		
		total = total / count;
		
		return total;
	}
	
	public static <T extends IFloatProvider> Number getFirstNonNullNumber(Collection<T> sourceFloats) {
		Float flt = getFirstNonNullFloat(sourceFloats);
		
		if(flt==null) {
			return null;
		}
		
		return (Number) flt;
	}
	
	public static <T extends IFloatProvider> Float getFirstNonNullFloat(Collection<T> sourceFloats) {
		for(IFloatProvider flt : sourceFloats) {
			if(flt.getValueAsFloat()!=null) {
				return flt.getValueAsFloat();
			}
		}
		
		return null;
	}
	
	public static <T extends IFloatProvider> Float splitByMinMaxRange(Collection<T> sourceFloats, String parameter) {
		Float targetValue = DEF_VAL_FLOAT;
		
		Float currentValue = getFirstNonNullFloat(sourceFloats);
		JSONArray jsonArr = new JSONArray(parameter);
		
		for(Object obj : jsonArr) {
			JSONObject jsonObj = (JSONObject) obj;
			
			Float minValue = jsonObj.getFloat(MIN);
			Float maxValue = jsonObj.getFloat(MAX);
			Float value = jsonObj.getFloat(VAL);
			
			if(currentValue>=minValue && currentValue<maxValue) {
				targetValue = value;
			}
		}
		
		return targetValue;
	}
	

	public static Float getY3(long x1, Float y1, long x2, Float y2, long x3) {
		float a = (y2-y1) / (x2-x1);
		float b = y1 - a*x1;
		
		return x3*a + b;
	}
	
	public static boolean getAsBoolean(String parameter) {
		if(parameter==null || StringUtils.isEmpty(parameter)) {
			return false;
		}
		if("1".equalsIgnoreCase(parameter.trim()) || "true".equalsIgnoreCase(parameter.trim())) {
			return true;
		}
		
		return false;
	}
	
	public static float findDiffFloat(List<IEntityFieldValue> vals) {
		float result = DEF_VAL_FLOAT;
		
		if(vals.size()>=2) {
			IEntityFieldValue val1 = vals.get(0);
			IEntityFieldValue val2 = vals.get(1);
			
			if(val1!=null && val2!=null && val1.getValueAsFloat()!=null && val2.getValueAsFloat()!=null) {
				result = Math.abs(val2.getValueAsFloat() - val1.getValueAsFloat());
			}
		}
		
		return result;
	}
}
