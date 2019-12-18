package com.interview.inheritance;

public class Base {

	public void runFun() {
		this.fun();
	}
	
	private void fun() {
		System.out.println("Base fun private");
	}
	
	public void runPublicFun() {
		System.out.println("Base fun public");
	}
	
	public static void runStaticFun() {
		System.out.println("Base fun static");
	}
}
