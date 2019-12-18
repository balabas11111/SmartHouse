package com.interview;


public class CorrectCompare {

	public static final String EXP_VALUE = "value";
	
	// we have received value from remote service
	public static void main(String[] args) {
		String valueFromAnotherClass = args[0];
		
		CorrectCompare compare = new CorrectCompare();
		System.out.println(compare.compareValue(valueFromAnotherClass));
	}
	
	public boolean compareValue(String valueFromRemote) {
		//return ???;
		return false;
	}
	
}
