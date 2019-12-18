package com.interview.inheritance;

public class Inheritance {

	 public static void main (String[] args)  {
		 Base base =new Base();
		 base.runPublicFun();
		 base.runFun();
		 base.runStaticFun();
		 System.out.println("-------------------");
		 
		 Base base2 = new Derived();
		 base2.runPublicFun();
		 base2.runFun();
		 base2.runStaticFun();
		 System.out.println("-------------------");
		 
		 Derived derived =new Derived();
		 derived.runPublicFun();
		 derived.runFun();
		 derived.runStaticFun();
		 System.out.println("-------------------");
	 }
}
