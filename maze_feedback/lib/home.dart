import 'package:flutter/material.dart';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';

class MyHomePage extends StatefulWidget {
  const MyHomePage({super.key});

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  Widget mainbody() {
    return Center(
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: [
          ElevatedButton(
              onPressed: () => sendToArduino(),
              child: const Text('Send to arduino'))
        ],
      ),
    );
  }

  sendToArduino() {}

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Maze Solver'),
      ),
      body: FutureBuilder(
          future: FlutterBluetoothSerial.instance.requestEnable(),
          builder: (context, future) {
            if (future.connectionState == ConnectionState.waiting) {
              return const Center(
                child: Icon(
                  Icons.bluetooth_disabled,
                  size: 200,
                ),
              );
            } else {
              return mainbody();
            }
          }),
    );
  }
}
