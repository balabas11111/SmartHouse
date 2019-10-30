package com.multithread;

public class DeadlockIfIntrinsicLocksNotRelevant {

	public static void main(String[] params) {
		WidgetChild w = new WidgetChild();
		
		w.doSomething();
	}
}
