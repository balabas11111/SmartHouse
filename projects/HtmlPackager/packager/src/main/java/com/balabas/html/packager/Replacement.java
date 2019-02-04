package com.balabas.html.packager;

public class Replacement {
    
    private int start;
    private int end;
    
    private String key;
    
    private boolean replaced=false;
    private boolean notFound=true;
    
    public Replacement(int start, int end, String key) {
        super();
        this.start = start;
        this.end = end;
        this.key = key;
        this.notFound=false;
    }

    public int getStart() {
        return start;
    }

    public void setStart(int start) {
        this.start = start;
    }

    public int getEnd() {
        return end;
    }

    public void setEnd(int end) {
        this.end = end;
    }

    public String getKey() {
        return key;
    }

    public void setKey(String key) {
        this.key = key;
    }

    public boolean isReplaced() {
        return replaced;
    }

    public void setReplaced(boolean replaced) {
        this.replaced = replaced;
    }

    public boolean isNotFound() {
        return notFound;
    }

    public void setNotFound(boolean notFound) {
        this.notFound = notFound;
    }
    
    
    
}
