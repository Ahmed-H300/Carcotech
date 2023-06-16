package com.carcotech.maze_solver;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.os.Bundle;

import android.util.Log;
import android.view.View;

import androidx.navigation.ui.AppBarConfiguration;

import com.example.maze_solver.R;
import com.example.maze_solver.databinding.ActivityMainBinding;
import com.google.android.material.floatingactionbutton.FloatingActionButton;
import com.google.android.material.snackbar.Snackbar;

import android.view.Menu;
import android.view.MenuItem;
import android.widget.TextView;

import org.opencv.android.CameraActivity;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.MatOfPoint;
import org.opencv.core.Point;
import org.opencv.core.Rect;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.imgproc.Imgproc;

import java.io.IOException;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Objects;
import java.util.UUID;

public class MainActivity extends CameraActivity {

    private AppBarConfiguration appBarConfiguration;
    private ActivityMainBinding binding;

    private BluetoothSocket socket;
    private OutputStream outputStream;
    public static int counter = 0;
    public static String testStr = "L";
    public static String prevSlowDown = "L";
    public static boolean detectLines = true;
    public static Mat lines;
    CameraBridgeViewBase cameraBridgeViewBase;
    private boolean connectedToBluetooth;

    public double takeDecision(double[] line, double x, double y) {
        String slowdown = "H";
        double x1 = line[0];
        double y1 = line[1];
        double x2 = line[2];
        double y2 = line[3];
        double minDistance = Math.abs((y2 - y1) * x - (x2 - x1) * y + x2 * y1 - y2 * x1)
                / Math.sqrt(Math.pow(y2 - y1, 2) + Math.pow(x2 - x1, 2));
        if (minDistance < 5) {
            System.out.println("Minimum distance: \n " + minDistance);
            slowdown = "L";
        }
        return minDistance;
    }

    public double[] getNearestLine(Mat lines, double x, double y) {
        double minDistance = Double.MAX_VALUE;
        double[] minDistanceLine = lines.get(0, 0);
        for (int i = 1; i < lines.rows(); i++) {
            double[] l = lines.get(i, 0);
            double x1 = l[0];
            double y1 = l[1];
            double x2 = l[2];
            double y2 = l[3];
            // Imgproc.line(src, new Point(x1, y1), new Point(x2, y2), new Scalar(0, 0,
            // 255), 3);
            double distance = Math.abs((y2 - y1) * x - (x2 - x1) * y + x2 * y1 - y2 * x1)
                    / Math.sqrt(Math.pow(y2 - y1, 2) + Math.pow(x2 - x1, 2));
            if (distance < minDistance) {
                minDistance = distance;
                minDistanceLine = l;
            }

        }
        return minDistanceLine;
    }

    public Mat getLines(Mat src) {
        Mat dst = new Mat();
        Mat lines = new Mat();
        Imgproc.Canny(src, dst, 50, 200);
        Imgproc.GaussianBlur(dst, dst, new Size(3, 3), 0);
        int threshold = 50;
        int minLineSize = 100;
        int lineGap = 10;
        double minDistance = Double.MAX_VALUE;
        Imgproc.HoughLinesP(dst, lines, 1, Math.PI / 180, threshold, minLineSize, lineGap);
        return lines;
        // for (int i = 0; i < lines.rows(); i++) {
        // double[] l = lines.get(i, 0);
        // double x1 = l[0];
        // double y1 = l[1];
        // double x2 = l[2];
        // double y2 = l[3];
        // Imgproc.line(src, new Point(x1, y1), new Point(x2, y2), new Scalar(0, 0,
        // 255), 3);
        // double distance = Math.abs((y2 - y1) * x - (x2 - x1) * y + x2 * y1 - y2 * x1)
        // / Math.sqrt(Math.pow(y2 - y1, 2) + Math.pow(x2 - x1, 2));
        // if (distance < minDistance) {
        // minDistance = distance;
        // }
        // }
        //
        // if (minDistance < 5) {
        // System.out.println("Minimum distance: \n " + minDistance);
        // Imgproc.circle(src, new Point(x, y), 5, new Scalar(0, 0, 255), 2);
        // }
        // return src;
    }

    public int countIntersectingSides(Rect rect, double[] line) {
        Point topLeft = new Point(rect.x, rect.y);
        Point topRight = new Point(rect.x + rect.width, rect.y);
        Point bottomLeft = new Point(rect.x, rect.y + rect.height);
        Point bottomRight = new Point(rect.x + rect.width, rect.y + rect.height);

        double a = line[0];
        double b = line[1];
        double c = line[2];

        int count = 0;
        if (doLinesIntersect(topLeft, topRight, a, b, c))
            count++;
        if (doLinesIntersect(topRight, bottomRight, a, b, c))
            count++;
        if (doLinesIntersect(bottomRight, bottomLeft, a, b, c))
            count++;
        if (doLinesIntersect(bottomLeft, topLeft, a, b, c))
            count++;

        return count;
    }

    public int countIntersectingSides(Rect rect, Mat lines) {
        Point topLeft = new Point(rect.x, rect.y);
        Point topRight = new Point(rect.x + rect.width, rect.y);
        Point bottomLeft = new Point(rect.x, rect.y + rect.height);
        Point bottomRight = new Point(rect.x + rect.width, rect.y + rect.height);

        int count = 0;
        for (int i = 0; i < lines.rows(); i++) {
            double[] line = lines.get(i, 0);
            double a = line[0];
            double b = line[1];
            double c = line[2];

            if (doLinesIntersect(topLeft, topRight, a, b, c))
                count++;
            if (doLinesIntersect(topRight, bottomRight, a, b, c))
                count++;
            if (doLinesIntersect(bottomRight, bottomLeft, a, b, c))
                count++;
            if (doLinesIntersect(bottomLeft, topLeft, a, b, c))
                count++;
        }

        return count;
    }

    private boolean doLinesIntersect(Point p1, Point p2, double a, double b, double c) {
        double d1 = a * p1.x + b * p1.y + c;
        double d2 = a * p2.x + b * p2.y + c;

        if (d1 > 0 && d2 < 0)
            return true;
        if (d1 < 0 && d2 > 0)
            return true;

        // additional check for parallel lines
        double threshold = 1e-6;
        double dist1 = Math.abs(d1) / Math.sqrt(a * a + b * b);
        double dist2 = Math.abs(d2) / Math.sqrt(a * a + b * b);
        if (dist1 < threshold && dist2 < threshold)
            return true;

        return false;
    }

    public Rect detectCar(Mat rgba) {
        Mat hsv = new Mat();
        Imgproc.cvtColor(rgba, hsv, Imgproc.COLOR_RGB2HSV);

        // define range of red color in HSV
        Scalar lower_red = new Scalar(0, 100, 50);
        Scalar upper_red = new Scalar(9, 255, 255);

        // threshold the HSV image to get only red colors
        Mat mask = new Mat();
        Core.inRange(hsv, lower_red, upper_red, mask);

        // find contours in the binary image
        List<MatOfPoint> contours = new ArrayList<>();
        Mat hierarchy = new Mat();
        Imgproc.findContours(mask, contours, hierarchy, Imgproc.RETR_TREE, Imgproc.CHAIN_APPROX_SIMPLE);
        // find the largest contour
        double maxArea = 0;
        Rect maxRect = null;

        for (MatOfPoint contour : contours) {
            Rect rect = Imgproc.boundingRect(contour);
            double area = rect.width * rect.height;
            if (area > maxArea) {
                maxArea = area;
                maxRect = rect;
            }
        }
        return maxRect;
    }

    @SuppressLint("MissingPermission")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        // binding = ActivityMainBinding.inflate(getLayoutInflater());

        setContentView(R.layout.activity_main);
        FloatingActionButton fab = findViewById(R.id.fab);
        TextView txt = findViewById(R.id.textView);

        connectedToBluetooth = (boolean) getIntent().getSerializableExtra("connected");
        if (connectedToBluetooth) {
            UUID uuid = (UUID) getIntent().getSerializableExtra("uuid");
            BluetoothDevice device = getIntent().getParcelableExtra("device");

            try {
                socket = device.createRfcommSocketToServiceRecord(uuid);
                socket.connect();
                outputStream = socket.getOutputStream();
                outputStream.write("L".getBytes());

                Snackbar.make(fab, "Connected to " + device.getName(), Snackbar.LENGTH_SHORT).setAction("Action", null)
                        .show();
                // Intent i = new Intent(BluetoothActivity.this, MainActivity.class);
                // startActivity(i);

            } catch (IOException e) {
                e.printStackTrace();
            }

        }

        cameraBridgeViewBase = (CameraBridgeViewBase) findViewById(R.id.cameraView);

        cameraBridgeViewBase.setCvCameraViewListener(new CameraBridgeViewBase.CvCameraViewListener2() {
            @Override
            public void onCameraViewStarted(int width, int height) {

            }

            @Override
            public void onCameraViewStopped() {

            }

            @Override
            public Mat onCameraFrame(CameraBridgeViewBase.CvCameraViewFrame frame) {
                Mat rgba = frame.rgba();
                if (detectLines) {
                    lines = getLines(rgba);

                    detectLines = false;
                }
                Rect maxRect = detectCar(rgba);

                // draw a green border around the largest red rectangle and calculate car center
                double center_x = -1;
                double center_y = -1;
                String slowDown = "L";

                if (maxRect != null) {
                    double area = maxRect.height * maxRect.width;
                    // Log.d("area", "area:"+area);
                    if (area > 11000.0) {
                        slowDown = "H";
                        center_x = maxRect.x + maxRect.width / 2.0;
                        center_y = maxRect.y + maxRect.height / 2.0;
                        Imgproc.rectangle(rgba, new Point(maxRect.x, maxRect.y),
                                new Point(maxRect.x + maxRect.width, maxRect.y + maxRect.height), new Scalar(0, 255, 0),
                                3);

                        // get nearest line to the car center
                        // double[] nearestLine = getNearestLine(lines,center_x,center_y);
                        // calculate the distance between car and line
                        // double s = takeDecision(nearestLine,center_x,center_y);
                        // int sidesIntersecting = countIntersectingSides(maxRect, nearestLine);
                        // int sidesIntersecting = countIntersectingSides(maxRect, lines);
                    }

                }
                txt.setText("Slow Down: " + slowDown);
                if (connectedToBluetooth) {

                    if (!Objects.equals(prevSlowDown, slowDown)) {
                        prevSlowDown = slowDown;
                        try {
                            outputStream.write(slowDown.getBytes());
                        } catch (IOException e) {
                            throw new RuntimeException(e);
                        }
                    }
                }

                for (int i = 0; i < lines.rows(); i++) {
                    double[] l = lines.get(i, 0);
                    double x1 = l[0];
                    double y1 = l[1];
                    double x2 = l[2];
                    double y2 = l[3];
                    Imgproc.line(rgba, new Point(x1, y1), new Point(x2, y2), new Scalar(0, 0, 255), 3);

                }

                // send data each 30 frame

                return rgba;
            }
        });
        if (OpenCVLoader.initDebug()) {
            Log.d("Loaded", "OpenCV");
            cameraBridgeViewBase.enableView();
        }

        // NavController navController = Navigation.findNavController(this,
        // R.id.nav_host_fragment_content_main);
        // appBarConfiguration = new
        // AppBarConfiguration.Builder(navController.getGraph()).build();
        //
        // binding.fab.setOnClickListener(new View.OnClickListener() {
        // @Override
        // public void onClick(View view) {
        // Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
        // .setAction("Action", null).show();
        // }
        // });

        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                detectLines = true;
                Snackbar.make(view, "Detecting track", Snackbar.LENGTH_SHORT)
                        .setAction("Action", null).show();
                // try {
                // outputStream.write("H".getBytes());
                // Log.i("TAG", "SENT H TO BLUETOOTH");
                //
                // } catch (IOException e) {
                // Log.i("TAG", "ENTERED CATCH");
                //// throw new RuntimeException(e);
                // }
            }
        });
    }

    // void getPermission(){
    // if(checkSelfPermission(Manifest.permission.CAMERA) !=
    // PackageManager.PERMISSION_GRANTED){
    // requestPermissions(new String []{Manifest.permission.CAMERA},101);
    // }
    // }
    @Override
    protected List<? extends CameraBridgeViewBase> getCameraViewList() {
        return Collections.singletonList(cameraBridgeViewBase);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        // noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    // private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {
    // @Override
    // public void onManagerConnected(int status) {
    // switch (status) {
    // case LoaderCallbackInterface.SUCCESS:
    // {
    // Log.i("TAG", "OpenCV loaded successfully");
    // cameraBridgeViewBase.enableView();
    // } break;
    // default:
    // {
    // super.onManagerConnected(status);
    // } break;
    // }
    // }
    // };
    @Override
    public void onPause() {
        super.onPause();
        // if (mOpenCvCameraView != null)
        // mOpenCvCameraView.disableView();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        // if (mOpenCvCameraView != null)
        // mOpenCvCameraView.disableView();
        try {
            outputStream.close();
        } catch (IOException e) {

        }
        try {
            socket.close();
        } catch (IOException e) {

        }
    }

    @Override
    public void onResume() {
        super.onResume();
        // OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_6, this,
        // mLoaderCallback);
    }
}