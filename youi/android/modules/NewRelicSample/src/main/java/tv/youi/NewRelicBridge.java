package tv.youi;

import java.util.Map;
import android.app.Activity;
import android.content.Context;
import android.util.Log;

import com.newrelic.agent.android.NewRelic;
import com.newrelic.agent.android.util.NetworkFailure;

public class NewRelicBridge {

    private final static String TAG = "NewRelicBridge";
    private final static String EVENT_TYPE = "DFW_Mobile";
    private static NewRelic newRelic = null;

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

    static void noticeHttpTransaction(String url, String httpMethod, int statusCode, long startTime, long endTime,long bytesSend,long bytesReceived){
        Log.d(TAG, String.format("noticeHttpTransaction -> %s", url));
        NewRelic.noticeHttpTransaction(
            url,
            httpMethod,
            statusCode,
            startTime,
            endTime,
            bytesSend,
            bytesReceived
        );
    }

    /**
     * 
     * @param url
     * @param httpMethod
     * @param startTime
     * @param endTime
     * @param errorCode Unknown(-1), BadURL(-1000), TimedOut(-1001), CannotConnectToHost(-1004), DNSLookupFailed(-1006), BadServerResponse(-1011), SecureConnectionFailed(-1200);
     * @param message
     */
    static void noticeNetworkFailure(String url, String httpMethod, long startTime, long endTime, int errorCode, String message){
        Log.d(TAG, String.format("noticeNetworkFailure -> %s", url));
        NewRelic.noticeNetworkFailure(
            url,
            httpMethod,
            startTime,
            endTime,
            NetworkFailure.fromErrorCode(errorCode),
            message
        );
    }

    static void crashNow()
    {
        Log.d(TAG, String.format("crashNow"));
        NewRelic.crashNow("This is a test crash on android.");
    }

    /**
     * Sample code just for PoC
     */
    static void sampleNoticeHttpTransaction(){
        long endTime = System.nanoTime();
        long startTime = endTime - 230450028; 
        noticeHttpTransaction("http://jsonplaceholder.typicode.com/posts/1","GET",200,startTime,endTime,0,345);
    }
}
