package com.aexolgl.android.app;

import java.util.List;
import java.util.Map;

@SuppressWarnings({"WeakerAccess", "unused"})
public interface LibDependencyMap {
    List<String> getSortedDeps();
    Map<String, List<String>> getDeps();
    LibDependencyMap join(LibDependencyMap m);
}

