package com.interview.inheritance;

public class Derived extends Base {

	private void fun() {
		System.out.println("Derived fun private");
	}
	
	public void runPublicFun() {
		System.out.println("Derived fun public");
	}
	
	public static void runStaticFun() {
		System.out.println("Derived fun static");
	}
}
