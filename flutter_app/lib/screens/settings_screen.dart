import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:tmp_flutter/notifiers/alarm_notifier.dart';
import 'package:tmp_flutter/notifiers/task_notifier.dart';
import 'package:tmp_flutter/screens/about_screen.dart';
import 'package:tmp_flutter/notifiers/theme_notifier.dart';
import 'package:tmp_flutter/services/firebase_service.dart';

class SettingsScreen extends StatefulWidget {
  @override
  _SettingsScreenState createState() => _SettingsScreenState();
}

class _SettingsScreenState extends State<SettingsScreen> {
  double _volume = 50; // Default volume value
  double _brightness = 7; // Default brightness value
  int _snoozeTime = 5; // Default snooze time value (in minutes)
  final FirebaseService firebaseService = FirebaseService();

  @override
  void initState() {
    super.initState();
    _loadSettings();
  }

  Future<void> _loadSettings() async {
    final savedVolume = await firebaseService.getVolume();
    final savedBrightness = await firebaseService.getBrightness();
    final savedSnoozeTime = await firebaseService.getSnoozeTime();

    setState(() {
      _volume = (savedVolume ?? 50).toDouble(); // Convert int? to double
      _brightness = (savedBrightness ?? 7).toDouble(); // Convert int? to double
      _snoozeTime = savedSnoozeTime ?? 5; // Convert int? to int
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Settings'),
      ),
      body: Column(
        children: [
          SwitchListTile(
            title: Text('Dark Mode'),
            value: Provider.of<ThemeNotifier>(context).isDarkMode,
            onChanged: (bool value) {
              Provider.of<ThemeNotifier>(context, listen: false).toggleTheme();
            },
          ),
          ListTile(
            title: Text('Snooze Time (minutes)'),
            subtitle: Slider(
              value: _snoozeTime.toDouble(),
              min: 1,
              max: 10,
              divisions: 9,
              label: _snoozeTime.toString(),
              onChanged: (double value) {
                setState(() {
                  _snoozeTime = value.round();
                });
              },
              onChangeEnd: (double value) {
                firebaseService.setSnoozeTime(value.round());
              },
            ),
          ),
          ListTile(
            title: Text('Volume'),
            subtitle: Slider(
              value: _volume,
              min: 1,
              max: 100,
              divisions: 99,
              label: _volume.round().toString(),
              onChanged: (double value) {
                setState(() {
                  _volume = value;
                });
              },
              onChangeEnd: (double value) {
                firebaseService.setVolume(value.round());
              },
            ),
          ),
          ListTile(
            title: Text('Brightness'),
            subtitle: Slider(
              value: _brightness,
              min: 0,
              max: 15,
              divisions: 15,
              label: _brightness.round().toString(),
              onChanged: (double value) {
                setState(() {
                  _brightness = value;
                });
              },
              onChangeEnd: (double value) {
                firebaseService.setBrightness(value.round());
              },
            ),
          ),
          ElevatedButton(
            onPressed: () async {
              await syncNow(context);
            },
            child: Text('Sync Now'),
          ),
          ElevatedButton(
            onPressed: () {
              Navigator.push(
                context,
                MaterialPageRoute(builder: (context) => AboutScreen()),
              );
            },
            child: Text('About'),
          ),
        ],
      ),
      bottomNavigationBar: BottomNavigationBar(
        items: const <BottomNavigationBarItem>[
          BottomNavigationBarItem(
            icon: Icon(Icons.home),
            label: 'Home',
          ),
          BottomNavigationBarItem(
            icon: Icon(Icons.task),
            label: 'Tasks',
          ),
          BottomNavigationBarItem(
            icon: Icon(Icons.settings),
            label: 'Settings',
          ),
        ],
        currentIndex: 2,
        selectedItemColor: Colors.purple,
        onTap: (int index) {
          switch (index) {
            case 0:
              Navigator.pushNamed(context, '/home');
              break;
            case 1:
              Navigator.pushNamed(context, '/tasks');
              break;
            case 2:
              Navigator.pushNamed(context, '/settings');
              break;
          }
        },
      ),
    );
  }

  Future<void> syncNow(BuildContext context) async {
    final alarmNotifier = Provider.of<AlarmNotifier>(context, listen: false);
    final taskNotifier = Provider.of<TaskNotifier>(context, listen: false);

    await alarmNotifier.syncAlarms();
    await taskNotifier.syncTasks();
    await firebaseService.setSnoozeTime(_snoozeTime);
    await firebaseService.setVolume(_volume.round());
    await firebaseService.setBrightness(_brightness.round());

    ScaffoldMessenger.of(context).showSnackBar(
      SnackBar(content: Text('Synced successfully')),
    );
  }
}
