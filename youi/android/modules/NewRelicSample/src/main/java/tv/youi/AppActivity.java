package tv.youi;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.os.SystemClock;
import android.util.Log;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import tv.youi.youiengine.CYIActivity;

import com.newrelic.agent.android.NewRelic;

public class AppActivity extends CYIActivity {
    private static String LOG_TAG = "AppActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        NewRelic.withApplicationToken(
            "AAa51587e146cddf3ea149e2d1edc98d242071eafe-NRMA"
        ).start(this.getApplication());
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }
}
