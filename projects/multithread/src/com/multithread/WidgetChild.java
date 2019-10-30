package com.multithread;

public class WidgetChild extends Widget {

	public synchronized void doSomething() {
		super.doSomething();
		System.out.println(toString() + ": calling doSomething");
		super.doSomething();
	}
}
