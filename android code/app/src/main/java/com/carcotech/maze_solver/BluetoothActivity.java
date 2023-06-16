package com.carcotech.maze_solver;


import androidx.annotation.NonNull;
import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothSocket;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;


import com.example.maze_solver.R;

import java.io.IOException;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.UUID;

public class BluetoothActivity extends AppCompatActivity {
    //    private BluetoothAdapter mBluetoothAdapter;
//    private Set<BluetoothDevice> mPairedDevices;
//    private BroadcastReceiver mReceiver;
//    private BluetoothSocket mSocket;
//    private OutputStream mOutputStream;
    private List<BluetoothDevice> mDevices = new ArrayList<>();
    //    private DeviceAdapter mAdapter;
//    private ListView listView;
    private BluetoothAdapter bluetoothAdapter;
    private Set<BluetoothDevice> pairedDevices;
    private ListView listView;
    private ArrayAdapter<String> arrayAdapter;
    private ArrayList<BluetoothDevice> devices;
    private BluetoothSocket socket;
    private OutputStream outputStream;
    private static final int REQUEST_BLUETOOTH_PERMISSIONS = 1;

    @RequiresApi(api = Build.VERSION_CODES.S)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bluetooth);
//        // Get the default adapter
//        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
//
//        // Set up the ListView
//        ListView listView = findViewById(R.id.list_view);
//        mAdapter = new DeviceAdapter();
//        listView.setAdapter(mAdapter);
//
//        // Register for broadcasts when a device is discovered
//        IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
//        mReceiver = new BroadcastReceiver() {
//            public void onReceive(Context context, Intent intent) {
//                String action = intent.getAction();
//                // When discovery finds a device
//                if (BluetoothDevice.ACTION_FOUND.equals(action)) {
//                    // Get the BluetoothDevice object from the Intent
//                    BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
//                    mDevices.add(device);
//                    mAdapter.notifyDataSetChanged();
//                }
//            }
//        };
//        registerReceiver(mReceiver, filter);
//
//        // Start discovery
//        if (ActivityCompat.checkSelfPermission(this, android.Manifest.permission.BLUETOOTH_SCAN) != PackageManager.PERMISSION_GRANTED) {
//            // TODO: Consider calling
//            //    ActivityCompat#requestPermissions
//            // here to request the missing permissions, and then overriding
//            //   public void onRequestPermissionsResult(int requestCode, String[] permissions,
//            //                                          int[] grantResults)
//            // to handle the case where the user grants the permission. See the documentation
//            // for ActivityCompat#requestPermissions for more details.
//            return;
//        }
//        mBluetoothAdapter.startDiscovery();
        listView = findViewById(R.id.list_view);
        devices = new ArrayList<>();
        arrayAdapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1);
        listView.setAdapter(arrayAdapter);
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        if (bluetoothAdapter == null) {
            Toast.makeText(this, "Bluetooth not supported", Toast.LENGTH_SHORT).show();
            Log.d("TAG", "onCreate: null adapter");
            finish();
        } else if (!bluetoothAdapter.isEnabled()) {
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, 0);
        }

        if (ActivityCompat.checkSelfPermission(this, android.Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED || ActivityCompat.checkSelfPermission(this, android.Manifest.permission.BLUETOOTH) != PackageManager.PERMISSION_GRANTED||ActivityCompat.checkSelfPermission(this, android.Manifest.permission.BLUETOOTH_ADMIN) != PackageManager.PERMISSION_GRANTED) {
            // TODO: Consider calling
            //    ActivityCompat#requestPermissions
            // here to request the missing permissions, and then overriding
            //   public void onRequestPermissionsResult(int requestCode, String[] permissions,
            //                                          int[] grantResults)
            // to handle the case where the user grants the permission. See the documentation
            // for ActivityCompat#requestPermissions for more details.
            // Request Bluetooth permissions
            ActivityCompat.requestPermissions(this,
                    new String[]{android.Manifest.permission.BLUETOOTH, android.Manifest.permission.BLUETOOTH_ADMIN, android.Manifest.permission.BLUETOOTH_SCAN, android.Manifest.permission.BLUETOOTH_CONNECT},
                    REQUEST_BLUETOOTH_PERMISSIONS);
//            return;
        }

        pairedDevices = bluetoothAdapter.getBondedDevices();
        if (pairedDevices.size() > 0) {
            for (BluetoothDevice device : pairedDevices) {
                devices.add(device);
                arrayAdapter.add(device.getName() + "\n" + device.getAddress());
                Log.d("blue",device.getName());
            }
        }

        listView.setOnItemClickListener((parent, view, position, id) -> {
            BluetoothDevice device = devices.get(position);


            UUID uuid = device.getUuids()[0].getUuid();
            Intent i = new Intent(BluetoothActivity.this, MainActivity.class);
            i.putExtra("connected", true);
            i.putExtra("uuid", uuid);
            i.putExtra("device", device);
            startActivity(i);



//            try {
//                UUID uuid = device.getUuids()[0].getUuid();
//                socket = device.createRfcommSocketToServiceRecord(uuid);
//                socket.connect();
//                outputStream = socket.getOutputStream();
//                outputStream.write("H".getBytes());
//                Intent i = new Intent(BluetoothActivity.this, MainActivity.class);
//                startActivity(i);
//
//            } catch (IOException e) {
//                e.printStackTrace();
//            }
        });
    }

    // Override onRequestPermissionsResult to handle the user's response to the permission request
    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions,
                                           @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == REQUEST_BLUETOOTH_PERMISSIONS) {
            if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                // Permission granted, do something here...
            } else {
                // Permission denied, handle it here...
            }
        }
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();

        // Don't forget to unregister the ACTION_FOUND receiver.
//        unregisterReceiver(mReceiver);
        // Close the socket and output stream
        try {
            outputStream.close();
        } catch (IOException e) {

        }
        try {
            socket.close();
        } catch (IOException e) {

        }
    }

    private class DeviceAdapter extends BaseAdapter {
        @Override
        public int getCount() {
            return mDevices.size();
        }

        @Override
        public Object getItem(int position) {
            return mDevices.get(position);
        }

        @Override
        public long getItemId(int position) {
            return position;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {
            if (convertView == null) {
                convertView = getLayoutInflater().inflate(R.layout.list_item_device, parent, false);
            }

            // Get the device and its info
            final BluetoothDevice device = (BluetoothDevice) getItem(position);

            @SuppressLint("MissingPermission") String deviceName = device.getName();
            String deviceHardwareAddress = device.getAddress(); // MAC address

            // Set the device name and address
            TextView nameTextView = convertView.findViewById(R.id.text_view_name);
            nameTextView.setText(deviceName);
            TextView addressTextView = convertView.findViewById(R.id.text_view_address);
            addressTextView.setText(deviceHardwareAddress);

            // Set up the connect button
            Button connectButton = convertView.findViewById(R.id.button_connect);
            connectButton.setOnClickListener(new View.OnClickListener() {
                @SuppressLint("MissingPermission")
                @Override
                public void onClick(View v) {
                    // Connect to the device and send a letter
                    try {
                        // Get a BluetoothSocket to connect with the given BluetoothDevice

                        @SuppressLint("MissingPermission") BluetoothSocket socket = device.createRfcommSocketToServiceRecord(UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"));
                        socket.connect();

                        // Get the output stream
                        OutputStream outputStream = socket.getOutputStream();

                        // Send a letter
                        outputStream.write('A');

                        // Close the socket and output stream
                        outputStream.close();
                        socket.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            });

            return convertView;
        }
    }
}
