package tv.youi.clientapp;

import java.util.Map;
import android.app.Activity;
import android.content.Context;
import android.util.Log;

import com.newrelic.agent.android.NewRelic;

public class NewRelicBridge {
    private final static String TAG = "NewRelicBridge";
    private final static String EVENT_TYPE = "DFW_Mobile";
    private static NewRelic newRelic = null;

    static void startWithApplicationToken(final String token, final Context context) {
        setMaxEventBufferTime(60);
        //NewRelic.enableFeature(FeatureFlag.NetworkRequests);
        if (newRelic == null) {
            newRelic = NewRelic.withApplicationToken(token);
            newRelic.withDefaultInteractions(false)
                    .withCrashReportingEnabled(true)
                    .start(((Activity) context).getApplication());
        }
    }

    static void setAttribute(final String name, final String value) {
        Log.d(TAG, "setAttribute -> " + name + ", " + value);
        boolean result = NewRelic.setAttribute(name, value);
        if (!result) {
            Log.e(TAG, "setAttribute -> name:  " + name + " and value: " + value + " failed.");
        }
    }

    static void recordCustomEvent(final String eventName, final Map attributes)
    {
        Log.d(TAG, String.format("recordCustomEvent -> %s",eventName));
        boolean result = NewRelic.recordCustomEvent(EVENT_TYPE, eventName, attributes);
        if (!result) {
            Log.e(TAG, "recordCustomEvent -> name: " + eventName + " failed.");
        }
    }

    static void setApplicationVersion(final String version)
    {
        Log.d(TAG, String.format("try setApplicationVersion -> %s",version));
        if (newRelic != null) {
            Log.d(TAG, String.format("setApplicationVersion -> %s",version));
            newRelic.withApplicationVersion(version);
        }
    }

    static void setMaxEventBufferTime(int time)
    {
        Log.d(TAG, String.format("setMaxEventBufferTime -> %d",time));
        NewRelic.setMaxEventBufferTime(time);
    }
    static void setMaxEventPoolSize(int size)
    {
        Log.d(TAG, String.format("setMaxEventPoolSize -> %d",size));
        NewRelic.setMaxEventPoolSize(size);
    }

    static void setLogLevels(int level)
    {
        Log.d(TAG, String.format("try setLogLevels -> %d",level));
        if (newRelic != null) {
            Log.d(TAG, String.format("setLogLevels -> %d",level));
            newRelic.withLogLevel(level);
        }
    }

    static void crashNow()
    {
        if (newRelic != null) {
            Log.d(TAG, String.format("crashNow"));
            newRelic.crashNow("This is a test crash on android.");
        }
    }
}
