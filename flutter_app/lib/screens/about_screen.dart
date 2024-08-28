import 'package:flutter/material.dart';

class AboutScreen extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('About'),
      ),
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Text(
              'Smart Alarm Clock',
              style: TextStyle(fontSize: 24, fontWeight: FontWeight.bold),
            ),
            SizedBox(height: 16),
            Text(
              'Developed by: Smart Alarm Clock Group',
              style: TextStyle(fontSize: 18),
            ),
            SizedBox(height: 8),
            Text(
              'Engine: Flutter',
              style: TextStyle(fontSize: 18),
            ),
            SizedBox(height: 8),
            Text(
              'Version: 1.0.0',
              style: TextStyle(fontSize: 18),
            ),
            SizedBox(height: 8),
            Text(
              'Description: This is a smart alarm clock application that allows users to set alarms, manage tasks, and sync with an ESP32 smart clock.',
              style: TextStyle(fontSize: 18),
            ),
            SizedBox(height: 8),
            Text(
              'Features:',
              style: TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
            ),
            Text(
              '- Set and manage alarms',
              style: TextStyle(fontSize: 18),
            ),
            Text(
              '- Add and manage tasks',
              style: TextStyle(fontSize: 18),
            ),
            Text(
              '- Sync with ESP32 smart clock',
              style: TextStyle(fontSize: 18),
            ),
          ],
        ),
      ),
    );
  }
}
