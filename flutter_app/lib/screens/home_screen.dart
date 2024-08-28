import 'dart:async';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:tmp_flutter/notifiers/alarm_notifier.dart';
import 'package:tmp_flutter/notifiers/task_notifier.dart';
import 'package:tmp_flutter/screens/add_edit_screen.dart';
import 'package:tmp_flutter/screens/sync_screen.dart';
import 'package:tmp_flutter/services/firebase_service.dart';
import 'package:intl/intl.dart';
import 'package:tmp_flutter/notifiers/esp32_status_notifier.dart';


class HomeScreen extends StatefulWidget {
  @override
  _HomeScreenState createState() => _HomeScreenState();
}

class _HomeScreenState extends State<HomeScreen> {
  final FirebaseService _firebaseService = FirebaseService();
  String _esp32Status = 'checking';
  Timer? _timer;
  static const Duration _checkInterval = Duration(seconds: 2);
  static const Duration _statusThreshold = Duration(minutes: 2);

  @override
  void initState() {
    super.initState();
    _fetchESP32StatusPeriodically();
  }

  void _fetchESP32StatusPeriodically() {
    _timer = Timer.periodic(_checkInterval, (timer) async {
      final lastTimestamp = await _firebaseService.getESP32LastTimestamp();
      if (mounted) {
        final now = DateTime.now();
        final newStatus = (lastTimestamp != null && now.difference(lastTimestamp) < _statusThreshold)
            ? 'connected'
            : 'disconnected';
        Provider.of<ESP32StatusNotifier>(context, listen: false).updateStatus(newStatus);
      }
    });
  }

  @override
  void dispose() {
    _timer?.cancel();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Smart Alarm Clock'),
        actions: [
          Consumer<AlarmNotifier>(
            builder: (context, alarmNotifier, child) {
              return Row(
                children: [
                  Icon(
                    Icons.circle,
                    color: alarmNotifier.isSynced ? Colors.green : Colors.red,
                  ),
                  IconButton(
                    icon: Icon(Icons.sync),
                    onPressed: () async {
                      await Provider.of<AlarmNotifier>(context, listen: false).syncAlarms();
                      Provider.of<TaskNotifier>(context, listen: false).syncTasks();
                      ScaffoldMessenger.of(context).showSnackBar(
                        SnackBar(content: Text('Alarms and Tasks synced')),
                      );
                    },
                  ),
                ],
              );
            },
          ),
        ],
      ),
      body: Consumer2<ESP32StatusNotifier, AlarmNotifier>(
        builder: (context,esp32StatusNotifier, alarmNotifier, child) {
          final esp32Status = esp32StatusNotifier.status;
          final alarms = alarmNotifier.alarms;

          return Column(
            children: [
              Padding(
                padding: const EdgeInsets.all(16.0),
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                    Icon(
                      esp32Status == 'connected' ? Icons.wifi : Icons.wifi_off,
                      color: esp32Status == 'connected' ? Colors.green : Colors.red,
                      size: 24,
                    ),
                    SizedBox(width: 10),
                    Flexible(
                      child: Text(
                        'ESP32 Status: $esp32Status',
                        style: TextStyle(
                          fontSize: 20,
                          color: esp32Status == 'connected' ? Colors.green : Colors.red,
                          fontWeight: FontWeight.bold,
                        ),
                        maxLines: 1,
                        overflow: TextOverflow.ellipsis,
                      ),
                    ),
                  ],
                ),
              ),
              Expanded(
                child: alarms.isEmpty
                    ? Center(
                  child: Column(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      Text(
                        'No alarms set',
                        style: TextStyle(
                          fontSize: 24,
                          color: Colors.black54,
                        ),
                      ),
                      SizedBox(height: 16),
                      ElevatedButton(
                        style: ElevatedButton.styleFrom(
                          padding: EdgeInsets.symmetric(horizontal: 32, vertical: 16),
                          shape: RoundedRectangleBorder(
                            borderRadius: BorderRadius.circular(30),
                          ),
                        ),
                        onPressed: () {
                          Navigator.pushNamed(context, '/alarms');
                        },
                        child: Text('View Alarms'),
                      ),
                      SizedBox(height: 16),
                      ElevatedButton(
                        style: ElevatedButton.styleFrom(
                          padding: EdgeInsets.symmetric(horizontal: 32, vertical: 16),
                          shape: RoundedRectangleBorder(
                            borderRadius: BorderRadius.circular(30),
                          ),
                        ),
                        onPressed: () {
                          Navigator.pushNamed(context, '/settings');
                        },
                        child: Text('Settings'),
                      ),
                    ],
                  ),
                )
                    : ListView.builder(
                  itemCount: alarms.length,
                  padding: EdgeInsets.all(16.0),
                  itemBuilder: (context, index) {
                    final alarmId = alarms.keys.elementAt(index);
                    final alarmData = alarms[alarmId]!;

                    return Card(
                      margin: EdgeInsets.symmetric(vertical: 10),
                      child: ListTile(
                        title: Text(
                          alarmData['label'],
                          style: TextStyle(fontSize: 20, fontWeight: FontWeight.bold),
                        ),
                        subtitle: Column(
                          crossAxisAlignment: CrossAxisAlignment.start,
                          children: [
                            Text(
                              'Date and Time: \n${_formatDateTime(alarmData['time'])}',
                            ),
                            Text('Repeat: ${alarmData['repeat']}'),
                            Text('Sound: ${alarmData['sound']}'),
                            if (_hasSelectedDays(alarmData['days']))
                              Text('Days: ${_formatDays(alarmData['days'])}'),
                          ],
                        ),
                        trailing: Wrap(
                          spacing: 12,
                          children: <Widget>[
                            Switch(
                              value: alarmData['enabled'] ?? true,
                              onChanged: (value) {
                                final updatedAlarmData = Map<String, dynamic>.from(alarmData);
                                updatedAlarmData['enabled'] = value;
                                Provider.of<AlarmNotifier>(context, listen: false)
                                    .addOrUpdateAlarm(alarmId, updatedAlarmData);
                                Provider.of<AlarmNotifier>(context, listen: false).markAsUnsynced();
                              },
                            ),
                            IconButton(
                              icon: Icon(Icons.delete),
                              onPressed: () {
                                Provider.of<AlarmNotifier>(context, listen: false).deleteAlarm(alarmId);
                                Provider.of<AlarmNotifier>(context, listen: false).markAsUnsynced();
                              },
                            ),
                          ],
                        ),
                        onTap: () {
                          Navigator.push(
                            context,
                            MaterialPageRoute(
                              builder: (context) => AddEditScreen(alarmId: alarmId, alarmData: alarmData),
                            ),
                          );
                        },
                      ),
                    );
                  },
                ),
              ),
            ],
          );
        },
      ),
      floatingActionButton: FloatingActionButton(
        onPressed: () {
          Navigator.push(
            context,
            MaterialPageRoute(builder: (context) => AddEditScreen()),
          );
        },
        child: Icon(Icons.add),
        backgroundColor: Colors.purple,
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
        currentIndex: 0,
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

  String _formatDateTime(String dateTimeStr) {
    DateTime dateTime = DateTime.parse(dateTimeStr);
    return DateFormat('dd-MM-yyyy | HH:mm').format(dateTime);
  }

  bool _hasSelectedDays(List<dynamic> days) {
    return days.contains(true);
  }

  String _formatDays(List<dynamic> days) {
    const dayLabels = ['Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun'];
    List<String> selectedDays = [];
    for (int i = 0; i < days.length; i++) {
      if (days[i]) {
        selectedDays.add(dayLabels[i]);
      }
    }
    return selectedDays.join(', ');
  }
}
