package com.aexolgl.android.app;

@SuppressWarnings({"unused", "WeakerAccess"})
public interface LibDependencyMapFactory {
    LibDependencyMap createDependecyMap();
    LibDependencyMap createDependecyMap(String fileName);
}
